

set(BIX_SUB_MODULES utils core window graphics)

add_library(bix_build_config INTERFACE)
add_library(bix::build_config ALIAS bix_build_config)

bix_module_add_headers(bix_build_config "export_macro.h" "macro_define.h")

if (BIX_BUILD_SHARED)
    add_library(${BIX_TARGET_NAME} SHARED)
    target_compile_definitions(bix_build_config INTERFACE "BIX_EXPORTS")
else ()
    add_library(${BIX_TARGET_NAME} STATIC)
    target_compile_definitions(bix_build_config INTERFACE "BIX_STATIC_DEFINE")
endif ()


target_compile_definitions(bix_build_config INTERFACE "BIX_FMT_HEADER=<fmt/format.h>")

target_include_directories(bix_build_config INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>")

target_link_libraries(bix_build_config INTERFACE
        fmt::fmt spdlog::spdlog
)
target_compile_definitions(bix_build_config INTERFACE
        $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
        $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>
        $<$<CONFIG:RelWithDebInfo>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>
        $<$<CONFIG:MinSizeRel>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_WARN>
)

if (WIN32)
    target_compile_definitions(bix_build_config INTERFACE "UNICODE" "_UNICODE" "NOMINMAX" "WIN32_LEAN_AND_MEAN")
endif ()

if (MSVC)
    target_compile_options(bix_build_config INTERFACE "/utf-8" "/W4" "/WX" "/permissive-" "/sdl")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (NOT MSVC)
        target_compile_options(bix_build_config INTERFACE "-Wall" "-Wextra" "-Werror" "-pedantic-errors" "-pedantic")
    endif ()
    target_compile_options(bix_build_config INTERFACE
            "-Wfloat-equal" "-Wshadow" "-Wconversion" "-Wsign-conversion"
            "-Wold-style-cast" "-Wcast-qual" "-Wcast-align" "-Wundef" "-Wwrite-strings" "-Wpointer-arith"
            "-Woverloaded-virtual" "-Wmissing-include-dirs" "-Wdisabled-optimization" "-Wredundant-decls"
            "-Wzero-as-null-pointer-constant" "-Wdeprecated" "-Wdouble-promotion"
            "-Wnull-dereference" "-Wweak-vtables"
            "-Wmissing-declarations" "-Wunreachable-code" "-Wimplicit-fallthrough"
    )
endif ()

set_target_properties(${BIX_TARGET_NAME} PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        EXPORT_NAME ${BIX_OUTPUT_NAME}
        DEBUG_POSTFIX "_d"
)


add_library(bix_geometry INTERFACE)
bix_module_setup(bix_geometry)
bix_module_add_headers(bix_geometry "geometry.h"
        "geometry/corner_radii.h" "geometry/round_rect.h"
        "geometry/line.h" "geometry/point.h" "geometry/rect.h"
        "geometry/shape.h" "geometry/size.h"
)

target_include_directories(${BIX_TARGET_NAME} PUBLIC
        $<BUILD_INTERFACE:${BIX_INCLUDE_BASE}>
        $<INSTALL_INTERFACE:include>
)
#target_link_libraries(${BIX_TARGET_NAME} PRIVATE bix::build_config bix_internal_link_pool)

foreach (mod ${BIX_SUB_MODULES})
    add_subdirectory(${mod})
endforeach ()


install(TARGETS ${BIX_TARGET_NAME}
        EXPORT bixlib-targets
        FILE_SET HEADERS DESTINATION include
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(EXPORT bixlib-targets
        NAMESPACE bix::
        FILE bixlib-targets.cmake
        DESTINATION lib/cmake/bixlib)