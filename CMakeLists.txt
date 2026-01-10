cmake_minimum_required(VERSION 3.26)

project(bixlib VERSION 0.0.1 LANGUAGES CXX)

include(CMakeDependentOption)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BIX_BUILD_SHARED "Build bixlib as shared library" ON)
option(BIX_BUILD_DOCS "Generate HTML documentation using Doxygen" ON)

set(BIX_OUTPUT_NAME "bix" CACHE STRING "The output name of the generated library file")

cmake_dependent_option(BIX_RENDERER_D2D "Enable Direct2D graphics backend" ON "WIN32" OFF)
cmake_dependent_option(BIX_RENDERER_GDI "Enable GDI+ graphics backend." ON "WIN32" OFF)
#cmake_dependent_option(BIX_RENDERER_METAL "Enable Metal" ON "APPLE" OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

if (BIX_BUILD_DOCS)
    include(cmake/GenDoc.cmake)
endif ()

if (ANDROID)
    list(APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif ()

include(cmake/ModuleHelpers.cmake)

set(BIX_TARGET_NAME "bixlib")
set(BIX_HEADER_ROOT "${PROJECT_SOURCE_DIR}/include/bixlib")

if (BIX_BUILD_SHARED)
    add_library(${BIX_TARGET_NAME} SHARED)
else ()
    add_library(${BIX_TARGET_NAME} STATIC)
endif ()

target_include_directories(${BIX_TARGET_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(${BIX_TARGET_NAME} PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        OUTPUT_NAME "${BIX_OUTPUT_NAME}"
        DEBUG_POSTFIX "_d"
)

include(cmake/utils/AddExtDependency.cmake)
include(cmake/FetchFmt.cmake)
include(cmake/FetchSpdlog.cmake)


add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(tests)