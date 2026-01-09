


option(ENABLE_MEMORY_SNIFFER "Enable memory sniffing for tests" ON)

add_ext_dependency(googletest
        URL "https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz"
        URL_HASH "SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c"

)

find_package(GTest REQUIRED HINTS "${googletest_INSTALL_DIR}")

include(GoogleTest)
enable_testing()


add_library(memory_sniffer_obj OBJECT)

if (ENABLE_MEMORY_SNIFFER)
    target_sources(memory_sniffer_obj PRIVATE memory_sniffer.cpp memory_sniffer.h)
    target_compile_definitions(memory_sniffer_obj PUBLIC HAS_MEMORY_SNIFFER)

    #    FetchContent_Declare(
    #            minhook
    #            GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    #            GIT_TAG        master
    #    )
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