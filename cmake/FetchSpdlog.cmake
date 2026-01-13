
include_guard()

find_package(spdlog QUIET)

if (NOT spdlog_FOUND)
    add_ext_dependency(spdlog
            URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz"
            URL_HASH "SHA256=d8862955c6d74e5846b3f580b1605d2428b11d97a410d86e2fb13e857cd3a744"
            CMAKE_ARGS "-DSPDLOG_BUILD_EXAMPLE=OFF;-DSPDLOG_FMT_EXTERNAL=ON"
    )
    find_package(spdlog CONFIG REQUIRED HINTS "${spdlog_INSTALL_DIR}")
endif ()