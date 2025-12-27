
/*
 * Copyright (c) 2025 Lynn <lynnplus90@gmail.com>.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "memory_sniffer.h"

#include <MinHook.h>
#include <iomanip>
#include <ios>
#include <sstream>

namespace bix::test {

typedef void*(__cdecl* MallocFuncPtr)(size_t);
typedef void*(__cdecl* CallocFuncPtr)(size_t, size_t);
typedef void*(__cdecl* ReallocFuncPtr)(void*, size_t);
typedef void(__cdecl* FreeFuncPtr)(void*);

static thread_local bool gIsInHook = false;

std::string formatMemoryStats(const MemoryStats& stats, const std::string& tag) {
    std::stringstream ss;
    auto diff = static_cast<long long>(stats.allocCount) - static_cast<long long>(stats.freeCount);

    ss << "\n========================================\n";
    ss << " MEMORY REPORT [" << (tag.empty() ? "SNAPSHOT" : tag) << "]\n";
    ss << "----------------------------------------\n";
    ss << " Operations:  Alloc(" << stats.allocCount << ") | Free(" << stats.freeCount << ")\n";
    ss << " Balance:     " << (diff == 0 ? "PERFECT" : std::to_string(diff) + " leaks detected") << "\n";
    ss << "----------------------------------------\n";
    ss << " Live Usage:  " << std::fixed << std::setprecision(2) << stats.currentUsage / 1024.0 << " KB\n";
    ss << " Peak Usage:  " << stats.peakUsage / 1024.0 << " KB\n";
    ss << " Total Flow:  " << stats.totalAllocated / 1024.0 << " KB\n";
    ss << "========================================\n";
    return ss.str();
}

MemoryStats MemorySniffer::enable() {
    auto& sniffer = getInstance();
    auto ret = getStatsSnapshot();
    if (sniffer.mRefCount.fetch_add(1) == 0) {
        sniffer.applyHook();
    }
    return ret;
}

MemoryStats MemorySniffer::disable() {
    auto& sniffer = getInstance();
    int expected = sniffer.mRefCount.load();
    while (expected > 0) {
        if (sniffer.mRefCount.compare_exchange_strong(expected, expected - 1)) {
            break;
        }
    }
    return getStatsSnapshot();
}

void MemorySniffer::uninstall() {
    auto& sniffer = getInstance();
    sniffer.mRefCount = 0;
    sniffer.removeHook();
    sniffer.reset();
}

void MemorySniffer::reset() {
    auto& sniffer = getInstance();
    sniffer.mAllocCount = 0;
    sniffer.mFreeCount = 0;
    sniffer.mTotalBytes = 0;
    sniffer.mCurrentUsage = 0;
    sniffer.mPeakUsage = 0;
}

bool MemorySniffer::isActive() { return getInstance().mRefCount.load() > 0; }

MemoryStats MemorySniffer::getStatsSnapshot() {
    auto& sniffer = getInstance();
    return {sniffer.mAllocCount.load(), sniffer.mFreeCount.load(), sniffer.mTotalBytes.load(),
            sniffer.mCurrentUsage.load(), sniffer.mPeakUsage.load()};
}

MemorySniffer::MemorySniffer() {
    if (MH_Initialize() != MH_OK) {}
}

MemorySniffer::~MemorySniffer() {
    removeHook();
    MH_Uninitialize();
}

MemorySniffer& MemorySniffer::getInstance() {
    static MemorySniffer instance;
    return instance;
}

void* __cdecl MemorySniffer::hookedMalloc(size_t size) {
    auto& sniffer = getInstance();
    void* ptr = reinterpret_cast<MallocFuncPtr>(sniffer.mMallocTarget.origAddr)(size);
    if (ptr && !gIsInHook && sniffer.isActive()) {
        gIsInHook = true;
        sniffer.recordAllocation(ptr);
        gIsInHook = false;
    }
    return ptr;
}

void* __cdecl MemorySniffer::hookedCalloc(size_t count, size_t size) {
    auto& sniffer = getInstance();
    void* ptr = reinterpret_cast<CallocFuncPtr>(sniffer.mCallocTarget.origAddr)(count, size);
    if (ptr && !gIsInHook && sniffer.isActive()) {
        gIsInHook = true;
        sniffer.recordAllocation(ptr);
        gIsInHook = false;
    }
    return ptr;
}

void* __cdecl MemorySniffer::hookedRealloc(void* ptr, size_t size) {
    // Capture old size before realloc potentially invalidates the pointer
    size_t oldSize = ptr && !gIsInHook ? _msize(ptr) : 0;
    auto& sniffer = getInstance();
    void* newPtr = reinterpret_cast<ReallocFuncPtr>(sniffer.mReallocTarget.origAddr)(ptr, size);

    if (newPtr && !gIsInHook && sniffer.isActive()) {
        gIsInHook = true;
        sniffer.recordRealloc(oldSize, newPtr);
        gIsInHook = false;
    }
    return newPtr;
}

void __cdecl MemorySniffer::hookedFree(void* ptr) {
    auto& sniffer = getInstance();
    if (ptr && !gIsInHook && sniffer.isActive()) {
        gIsInHook = true;
        sniffer.recordFree(ptr);
        gIsInHook = false;
    }
    reinterpret_cast<FreeFuncPtr>(sniffer.mFreeTarget.origAddr)(ptr);
}

void MemorySniffer::recordAllocation(void* ptr) {
    size_t actualSize = _msize(ptr);
    mAllocCount.fetch_add(1);
    mTotalBytes.fetch_add(actualSize);
    size_t current = mCurrentUsage.fetch_add(actualSize) + actualSize;
    updatePeak(current);
}

void MemorySniffer::recordFree(void* ptr) {
    mFreeCount.fetch_add(1);
    mCurrentUsage.fetch_sub(_msize(ptr));
}

void MemorySniffer::recordRealloc(size_t oldSize, void* newPtr) {
    size_t newSize = _msize(newPtr);
    mCurrentUsage.fetch_sub(oldSize);
    size_t current = mCurrentUsage.fetch_add(newSize) + newSize;
    if (newSize > oldSize)
        mTotalBytes.fetch_add(newSize - oldSize);
    updatePeak(current);
}

void MemorySniffer::updatePeak(size_t current) {
    size_t peak = mPeakUsage.load();
    while (current > peak && !mPeakUsage.compare_exchange_weak(peak, current))
        ;
}

void MemorySniffer::applyHook() {
    std::lock_guard lock(mHookMutex);
    if (mIsHooked) {
        return;
    }
    HMODULE hMod = GetModuleHandleA("ucrtbase.dll");
    if (!hMod) {
        hMod = GetModuleHandleA("ucrtbased.dll");
    }
    if (!hMod) {
        hMod = GetModuleHandleA("msvcrt.dll");
    }
    if (!hMod) {
        return;
    }

    auto installHook = [&](const char* name, HookTarget& target, LPVOID detour) {
        if (target.state == HookState::Enabled) {
            return;
        }
        if (!target.targetAddr) {
            target.targetAddr = reinterpret_cast<LPVOID>(GetProcAddress(hMod, name));
            if (!target.targetAddr) {
                return;
            }
        }
        if (target.state == HookState::None) {
            if (MH_CreateHook(target.targetAddr, detour, &target.origAddr) != MH_OK) {
                return;
            }
            target.state = HookState::Created;
        }
        if (MH_EnableHook(target.targetAddr) == MH_OK) {
            target.state = HookState::Enabled;
        }
    };
    installHook("malloc", mMallocTarget, reinterpret_cast<LPVOID>(&hookedMalloc));
    installHook("calloc", mCallocTarget, reinterpret_cast<LPVOID>(&hookedCalloc));
    installHook("realloc", mReallocTarget, reinterpret_cast<LPVOID>(&hookedRealloc));
    installHook("free", mFreeTarget, reinterpret_cast<LPVOID>(&hookedFree));
    mIsHooked = true;
}

void MemorySniffer::removeHook() {
    std::lock_guard lock(mHookMutex);
    if (!mIsHooked) {
        return;
    }

    auto disableHook = [&](HookTarget& target) {
        if (target.state != HookState::Enabled) {
            return;
        }
        if (MH_DisableHook(target.targetAddr) == MH_OK) {
            target.state = HookState::Disabled;
        }
    };

    disableHook(mMallocTarget);
    disableHook(mCallocTarget);
    disableHook(mReallocTarget);
    disableHook(mFreeTarget);
    mIsHooked = false;
}
} // namespace bix::test
