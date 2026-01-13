cmake_minimum_required(VERSION 3.26)

project(bixlib VERSION 0.0.1 LANGUAGES CXX)

include(CMakeDependentOption)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BIX_BUILD_SHARED "Build bixlib as shared library" ON)
option(BIX_BUILD_DOCS "Generate HTML documentation using Doxygen" ON)
option(BIX_BUILD_EXAMPLES "Build example projects" ON)
option(BIX_BUILD_TESTS "Build unit tests" ON)

set(BIX_OUTPUT_NAME "bix" CACHE STRING "The output name of the generated library file")

cmake_dependent_option(BIX_RENDERER_D2D "Enable Direct2D graphics backend" ON "WIN32" OFF)
cmake_dependent_option(BIX_RENDERER_GDI "Enable GDI+ graphics backend." ON "WIN32" OFF)
#cmake_dependent_option(BIX_RENDERER_METAL "Enable Metal" ON "APPLE" OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

if (ANDROID)
    list(APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif ()

include(cmake/ModuleHelpers.cmake)

set(BIX_TARGET_NAME "bixlib")

set(BIX_INCLUDE_BASE "${PROJECT_SOURCE_DIR}/include")
set(BIX_HEADER_ROOT "${BIX_INCLUDE_BASE}/bixlib")

if (BIX_BUILD_DOCS)
    include(cmake/DoxygenConfig.cmake)
endif ()


include(cmake/utils/AddExtDependency.cmake)
include(cmake/FetchFmt.cmake)
include(cmake/FetchSpdlog.cmake)


add_subdirectory(src)
if (BIX_BUILD_EXAMPLES)
    add_subdirectory(example)
endif ()
if (BIX_BUILD_TESTS)
    add_subdirectory(tests)
endif ()
