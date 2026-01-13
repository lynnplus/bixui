
include_guard()


find_package(GTest)

if (NOT GTest_FOUND)
    add_ext_dependency(googletest
            URL "https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz"
            URL_HASH "SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c"
    )
    find_package(GTest REQUIRED HINTS "${googletest_INSTALL_DIR}")
endif ()
