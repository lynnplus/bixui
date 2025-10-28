

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG e424e3f2e607da02742f73db84873b8084fc714c) # 12.0.0
FetchContent_MakeAvailable(fmt)