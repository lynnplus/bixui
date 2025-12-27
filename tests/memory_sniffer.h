
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

#pragma once
#include <atomic>
#include <mutex>

/**
 * @namespace bix::test
 *
 * Namespace for memory diagnostic and testing utilities.
 */
namespace bix::test {

/**
 * A snapshot of memory allocation statistics.
 */
struct MemoryStats {
    size_t allocCount = 0;     ///< Total number of successful allocations
    size_t freeCount = 0;      ///< Total number of successful deallocations
    size_t totalAllocated = 0; ///< Cumulative physical bytes allocated (includes alignment padding).
    size_t currentUsage = 0;   ///< Current live physical bytes on the heap.
    size_t peakUsage = 0;      ///< Historical maximum value of currentUsage recorded.
};

/**
 * Formats memory statistics into a human-readable string.
 * @param stats The statistics snapshot to format.
 * @param tag An optional label for the report (e.g., "After Initialization").
 * @return A formatted multi-line string report.
 */
std::string formatMemoryStats(const MemoryStats& stats, const std::string& tag);

enum class HookState {
    None,
    Created,
    Enabled,
    Disabled
};

/**
 * Container for metadata associated with a specific hooked function.
 */
struct HookTarget {
    HookState state = HookState::None; ///< Current state of the hook.
    void* targetAddr = nullptr;        ///< Address of the target function (e.g., malloc).
    void* origAddr = nullptr;          ///< Address of the trampoline to call the original function.
};

/**
 * Singleton class that hooks CRT allocation functions to monitor memory usage.
 *
 * @note This class uses MinHook to intercept ucrtbase.dll/msvcrt.dll.
 * @warning Do not use complex STL containers inside recording methods to avoid recursion.
 */
class MemorySniffer {
public:
    // Disable copy/move
    MemorySniffer(const MemorySniffer& other) = delete;
    MemorySniffer(MemorySniffer&& other) noexcept = delete;
    MemorySniffer& operator=(const MemorySniffer& other) = delete;
    MemorySniffer& operator=(MemorySniffer&& other) noexcept = delete;

    /**
     * @brief Enables memory interception and increments the session reference count.
     * * If this is the first call (count transitions from 0 to 1), it physically installs
     * the MinHook patches into the target DLL functions.
     * * @note This operation is thread-safe.
     * @return A MemoryStats snapshot captured at the moment monitoring was enabled.
     */
    static MemoryStats enable();
    /**
     * Disables memory interception logically by decrementing the reference count.
     * When the count reaches zero, the sniffer stops recording data.
     * @return A final MemoryStats snapshot after the count is decremented.
     *
     * @warning This method does NOT physically remove the hooks to avoid the high overhead
     * of patching memory (VirtualProtect) frequently. The detour functions remain in
     * memory but will skip recording logic.
     * @see release() to physically remove hooks.
     */
    static MemoryStats disable();

    /**
     * Physically removes all hooks and cleans up internal resources.
     * This method restores the original function bytes and **resets all
     * counters to zero** (calls reset()).
     * @note After calling this, the sniffer is returned to its initial pristine state.
     */
    static void uninstall();
    /**
     * Resets all internal statistics counters to zero.
     * @note This does not stop the hooks; it only clears the data.
     */
    static void reset();

    /** Returns true if at least one requester has called start() without a matching stop(). */
    static bool isActive();

    /**
     * Captures a thread-safe snapshot of current statistics.
     * @return A copy of the current MemoryStats values.
     */
    static MemoryStats getStatsSnapshot();

private:
    std::atomic_int mRefCount{0};
    std::mutex mHookMutex;
    bool mIsHooked{false};

    // Statistics (Atomic for thread-safety)
    std::atomic<size_t> mAllocCount{0};
    std::atomic<size_t> mFreeCount{0};
    std::atomic<size_t> mTotalBytes{0};
    std::atomic<size_t> mCurrentUsage{0};
    std::atomic<size_t> mPeakUsage{0};

    HookTarget mMallocTarget{}, mCallocTarget{}, mReallocTarget{}, mFreeTarget{};

    MemorySniffer();
    ~MemorySniffer();

    /**
     * Access the singleton instance.
     *
     * @return Reference to the MemorySniffer instance.
     */
    static MemorySniffer& getInstance();

    /** * @name Hook Entry Points
     * Static members serve as raw function pointers for MinHook.
     * Must use __cdecl to match CRT calling convention.
     * @{
     */
    static void* __cdecl hookedMalloc(size_t size);
    static void* __cdecl hookedCalloc(size_t count, size_t size);
    static void* __cdecl hookedRealloc(void* ptr, size_t size);
    static void __cdecl hookedFree(void* ptr);
    /** @} */

    /** @name Internal Recording Methods
     * Logic for updating atomic counters.
     */
    ///@{
    void recordAllocation(void* ptr);
    void recordFree(void* ptr);
    void recordRealloc(size_t oldSize, void* newPtr);
    void updatePeak(size_t current);
    ///@}

    void applyHook();
    void removeHook();
};

} // namespace bix::test
