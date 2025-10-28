cmake_minimum_required(VERSION 3.26)

project(bixlib VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

file(GLOB_RECURSE sourcesFiles RELATIVE ${PROJECT_SOURCE_DIR} modules/*.cpp modules/*.h)

add_library(bix STATIC ${sourcesFiles})

target_include_directories(bix PRIVATE modules)

file(GLOB_RECURSE includeFiles RELATIVE ${PROJECT_SOURCE_DIR} include/*.h)

target_sources(bix PRIVATE ${includeFiles})

target_compile_definitions(bix PUBLIC -DBIX_STATIC_DEFINE)
set_target_properties(bix PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        PUBLIC_HEADER "${BIX_HEADERS}"
        DEFINE_SYMBOL BIX_EXPORTS
        DEBUG_POSTFIX "_d"
)

target_compile_definitions(bix PRIVATE WIN32_LEAN_AND_MEAN)
#target_compile_definitions(bix PUBLIC WINVER=0xA00 _WIN32_WINNT=0xA00)

if (MSVC)
    target_compile_definitions(bix PUBLIC "UNICODE" "_UNICODE" PRIVATE "NOMINMAX")
    target_compile_options(bix PUBLIC "/utf-8" PRIVATE "/W4" "/WX")
else ()
    target_compile_options(bix PUBLIC "-municode" PRIVATE
            "-Wall" "-Wextra" "-pedantic" "-Wconversion" "-Wshadow" "-Wnonnull" "-Wfloat-equal" "-Werror"
            "-pedantic-errors" "-Wold-style-cast" "-Wundef" "-Wzero-as-null-pointer-constant" "-Wnull-dereference"
            "-Wdeprecated" "-Wweak-vtables" "-Woverloaded-virtual" "-Wcast-qual" "-Wwrite-strings" "-Wpointer-arith"
            "-Wmissing-include-dirs" "-Wzero-as-null-pointer-constant"
            "-Wpre-c++20-compat-pedantic" "-Wreserved-identifier" "-Wmissing-field-initializers" "-Wuninitialized"
            #            -Weverything
    )
endif ()


target_include_directories(bix PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(cmake/FetchFmt.cmake)
target_link_libraries(bix PUBLIC fmt::fmt)

target_precompile_headers(bix PUBLIC <fmt/base.h> <fmt/std.h>)

target_link_libraries(bix PRIVATE d2d1.lib dwrite.lib)

add_subdirectory(example)