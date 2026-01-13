


option(ENABLE_MEMORY_SNIFFER "Enable memory sniffing for tests" ON)

include(${PROJECT_SOURCE_DIR}/cmake/FetchGTest.cmake)

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

macro(bix_test_setup TARGET_NAME)
    string(REGEX REPLACE "^bix_|(_test$)" "" MODULE_NAME ${TARGET_NAME})
    target_link_libraries(${TARGET_NAME}
            PRIVATE
            bix::${MODULE_NAME}
            bix::build_config
            GTest::gtest_main
    )
    gtest_discover_tests(${TARGET_NAME})
endmacro()


add_executable(bix_utils_test utils/numeric_test.cpp)
bix_test_setup(bix_utils_test)