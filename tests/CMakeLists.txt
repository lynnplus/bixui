


option(ENABLE_MEMORY_SNIFFER "Enable memory sniffing for tests" ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()


add_library(memory_sniffer_obj OBJECT)

if (ENABLE_MEMORY_SNIFFER)
    target_sources(memory_sniffer_obj PRIVATE memory_sniffer.cpp memory_sniffer.h)
    target_compile_definitions(memory_sniffer_obj PUBLIC HAS_MEMORY_SNIFFER)
    find_package(minhook REQUIRED)

    target_link_libraries(memory_sniffer_obj PRIVATE minhook::minhook)
endif ()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(
        test_length
        length_test.cpp
)
target_link_libraries(
        test_length
        GTest::gtest_main
        memory_sniffer_obj
)


gtest_discover_tests(test_length)