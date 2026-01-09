#
# Copyright (c) 2026 Lynn <lynnplus90@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

include_guard()
include(CMakeParseArguments)

set(EXT_DEP_BUILD_DIR "${CMAKE_BINARY_DIR}/external" CACHE PATH "Directory for building external dependencies")
set(EXT_DEP_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/.cache" CACHE PATH "Directory for storing downloaded dependency archives")
if (NOT EXISTS "${EXT_DEP_DOWNLOAD_DIR}")
    file(MAKE_DIRECTORY "${EXT_DEP_DOWNLOAD_DIR}")
endif ()

macro(init_ext_dependency_env)
    set(EXT_ENV_BASE_TEMPLATE_FILE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/env_base.cmake.in")
    #Shadow projects share the basic environment
    set(EXT_ENV_BASE_FILE "${EXT_DEP_BUILD_DIR}/env_base.cmake")

    if (MSVC)
        set(EXT_OTHER_CMD "set(CMAKE_MSVC_RUNTIME_LIBRARY ${CMAKE_MSVC_RUNTIME_LIBRARY} CACHE STRING \"\" FORCE)")
    elseif (ANDROID)
        set(EXT_OTHER_CMD "
set(ANDROID_ABI \"${ANDROID_ABI}\" CACHE STRING \"\" FORCE)
set(ANDROID_PLATFORM \"${ANDROID_PLATFORM}\" CACHE STRING \"\" FORCE)
set(ANDROID_STL \"${ANDROID_STL}\" CACHE STRING \"\" FORCE)
        ")
    endif ()

    get_property(ENV_BASE_CONFIGURED GLOBAL PROPERTY PROPERTY_ENV_BASE_DONE)
    #Only initialize the environment once
    if (NOT ENV_BASE_CONFIGURED)
        if (NOT EXT_QUIET)
            message(CHECK_START "Initializing external environment")
        endif ()
        configure_file("${EXT_ENV_BASE_TEMPLATE_FILE}" "${EXT_ENV_BASE_FILE}" @ONLY)
        set_property(GLOBAL PROPERTY PROPERTY_ENV_BASE_DONE TRUE)
        if (NOT EXT_QUIET)
            message(CHECK_PASS "done (env_base.cmake)")
        endif ()
    endif ()
endmacro(init_ext_dependency_env)

macro(purify_external_install manifest)
    file(STRINGS "${manifest}" _OLD_INSTALLED_FILES)
    foreach (_FILE_PATH ${_OLD_INSTALLED_FILES})
        if (EXISTS "${_FILE_PATH}")
            file(REMOVE "${_FILE_PATH}")
        endif ()
    endforeach ()
endmacro()


#[[.rst:
add_ext_dependency
------------------

Automates the build and installation of external dependencies using a "Shadow Project"
approach based on ExternalProject_Add.

Main Features:
* Downloads source code via Git or URL.
* Builds and installs the dependency in a separate binary directory (Shadow Build).
* Supports cache mechanism via stamp files to skip rebuilds if configuration is unchanged.
* Aggregates all build/config logs into a single file for easy debugging.

Arguments:
  NAME                Name of the external dependency (used as prefix for variables).
  GIT_REPOSITORY      URL of the git repository.
  GIT_TAG             Specific tag, branch, or commit hash to checkout.
  URL                 Remote URL for downloading source (e.g., .zip, .tar.gz).
  URL_HASH            Hash for URL download verification (e.g., SHA256=...).
  INSTALL_DIR         Optional custom installation path (defaults to <build>/external/<NAME>/install).
  DOWNLOAD_NAME       File name to use for the downloaded file. If not given, the end of the URL is used to determine the file name.
  CMAKE_ARGS          Additional arguments passed to the external project's CMake configuration.
  LOG_DOWNLOAD        If set, logs the download process to the aggregated log file.
  LOG_CONFIGURE       If set, logs the configuration process.
  LOG_BUILD           If set, logs the build process.
  QUIET               If set, Suppress status messages and print only errors.
  TIMEOUT             <seconds> Maximum time allowed for file download operations.
  KEEP_BUILD          If set, preserves the intermediate build and proxy directories
                      after a successful installation. By default, these directories
                      are deleted to save disk space. Enabling this allows for
                      faster [Incremental Builds] at the cost of significant
                      disk usage (often several GBs for large libraries).

Output Variables:
  <NAME>_INSTALL_DIR  Exported to PARENT_SCOPE, containing the path to the installed artifacts.

Example:
  add_ext_dependency(googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        v1.14.0
      LOG_BUILD      ON
      CMAKE_ARGS     -DBUILD_GMOCK=OFF
  )
  # Use the output variable to find headers/libraries
  add_library(GTest::GTest STATIC IMPORTED)
  set_target_properties(GTest::GTest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${googletest_INSTALL_DIR}/include"
    IMPORTED_LOCATION "${googletest_INSTALL_DIR}/lib/libgtest.a"
  )
  target_link_libraries(my_app PRIVATE GTest::gtest)
  #or use find_package
  find_package(GTest REQUIRED HINTS "${googletest_INSTALL_DIR}")
  target_link_libraries(my_app PRIVATE GTest::gtest)
#]]
function(add_ext_dependency NAME)
    set(options LOG_DOWNLOAD LOG_BUILD LOG_CONFIGURE QUIET KEEP_BUILD)
    set(oneValueArgs GIT_REPO GIT_TAG URL URL_HASH
            INSTALL_DIR DOWNLOAD_NAME TIMEOUT)
    set(multiValueArgs CMAKE_ARGS)
    cmake_parse_arguments(EXT "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

    macro(println_msg text)
        if (NOT EXT_QUIET)
            message(STATUS "[${NAME}] ${text}")
        endif ()
    endmacro()
    string(TIMESTAMP _TIME_START "%s")

    set(EXT_ROOT "${EXT_DEP_BUILD_DIR}/${NAME}")
    set(EXT_STAMP_DIR "${EXT_ROOT}/stamp")
    set(EXT_SOURCE_DIR "${EXT_ROOT}/src")
    set(EXT_BINARY_DIR "${EXT_ROOT}/build")
    set(STAMP_FILE "${EXT_ROOT}/build_success.stamp")
    set(SHADOW_BUILD_LOG_FILE "${EXT_ROOT}/build_out.log")
    set(LOCAL_DOWNLOAD_DIR "${EXT_DEP_DOWNLOAD_DIR}/${NAME}")

    if (NOT EXT_INSTALL_DIR)
        set(EXT_INSTALL_DIR "${EXT_ROOT}/install")
    endif ()
    init_ext_dependency_env()

    if (EXT_URL)
        set(DOWNLOAD_SOURCE "URL ${EXT_URL} \n    URL_HASH ${EXT_URL_HASH}")
        if (EXT_URL MATCHES "^(http|https|ftp)://")
            println_msg("Remote URL: ${EXT_URL}")
            set(DOWNLOAD_SOURCE "${DOWNLOAD_SOURCE}\n    DOWNLOAD_DIR \"${LOCAL_DOWNLOAD_DIR}\"")
        elseif (EXISTS "${EXT_URL}")
            println_msg("Local Source: ${EXT_URL}")
        endif ()
    elseif (EXT_GIT_REPO)
        set(DOWNLOAD_SOURCE "GIT_REPOSITORY ${EXT_GIT_REPO} \n    GIT_TAG ${EXT_GIT_TAG}\n    GIT_SHALLOW ON")
        println_msg("Git Source: ${EXT_GIT_REPO} (${EXT_GIT_TAG})")
    endif ()
    set(EXT_LOG_SETTING "")
    if (EXT_LOG_DOWNLOAD)
        set(EXT_LOG_SETTING "${EXT_LOG_SETTING}\n    LOG_DOWNLOAD ON")
    endif ()
    if (EXT_LOG_CONFIGURE)
        set(EXT_LOG_SETTING "${EXT_LOG_SETTING}\n    LOG_CONFIGURE ON")
    endif ()
    if (EXT_LOG_BUILD)
        set(EXT_LOG_SETTING "${EXT_LOG_SETTING}\n    LOG_BUILD ON")
    endif ()


    configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/shadow_project.cmake.in" "${EXT_ROOT}/CMakeLists.txt" @ONLY)

    #return argv xxx_INSTALL_DIR
    set(${NAME}_INSTALL_DIR "${EXT_INSTALL_DIR}" PARENT_SCOPE)

    if (EXISTS "${STAMP_FILE}" AND EXISTS "${EXT_INSTALL_DIR}/include")
        if ("${STAMP_FILE}" IS_NEWER_THAN "${EXT_ENV_BASE_FILE}" AND "${STAMP_FILE}" IS_NEWER_THAN "${EXT_ROOT}/CMakeLists.txt")
            println_msg("Cache hit. Skipping build.")
            println_msg("Found at: ${EXT_INSTALL_DIR}")
            return()
        endif ()
    endif ()

    macro(output_process_fail_msg step_name code)
        file(SIZE "${SHADOW_BUILD_LOG_FILE}" _LOG_SIZE)
        if (_LOG_SIZE LESS 1000)
            file(READ "${SHADOW_BUILD_LOG_FILE}" _LOG_TAIL)
        else ()
            math(EXPR _START_OFFSET "${_LOG_SIZE} - 1000")
            file(READ "${SHADOW_BUILD_LOG_FILE}" _LOG_TAIL OFFSET _START_OFFSET)
        endif ()
        message("
======================== EXTERNAL BUILD FAILED ========================
 Dependency : ${NAME}
 Exit Code  : ${code}
 Log File   : file:///${SHADOW_BUILD_LOG_FILE}
-----------------------------------------------------------------------
RECENT LOG:
${_LOG_TAIL}
=======================================================================
")
        message(FATAL_ERROR "[${NAME}] External dependency ${step_name} failed.")
    endmacro()

    if (EXISTS "${EXT_BINARY_DIR}/install_manifest.txt")
        println_msg("Purging previous installation files via manifest...")
        purify_external_install("${EXT_BINARY_DIR}/install_manifest.txt")
    endif ()
    file(REMOVE_RECURSE "${EXT_BINARY_DIR}")

    println_msg("Configuring shadow project...")
    set(EXT_EP_PROXY_BUILD_NAME "ep_proxy")
    execute_process(
            COMMAND ${CMAKE_COMMAND} -S . -B ${EXT_EP_PROXY_BUILD_NAME} -G "${CMAKE_GENERATOR}"
            WORKING_DIRECTORY "${EXT_ROOT}"
            RESULT_VARIABLE res1
            OUTPUT_FILE "${SHADOW_BUILD_LOG_FILE}"
            ERROR_FILE "${SHADOW_BUILD_LOG_FILE}"
    )

    if (NOT res1 EQUAL 0)
        output_process_fail_msg("Config" ${res1})
    endif ()
    println_msg("Building and installing...")
    execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${EXT_EP_PROXY_BUILD_NAME} --config ${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY "${EXT_ROOT}"
            RESULT_VARIABLE res1
            OUTPUT_VARIABLE build_log
            ERROR_VARIABLE build_log
    )
    file(APPEND "${SHADOW_BUILD_LOG_FILE}" "\n\n--- Build Step Output ---\n" "${build_log}")
    if (NOT res1 EQUAL 0)
        output_process_fail_msg("Build/Installing" ${res1})
    endif ()

    file(TOUCH "${STAMP_FILE}")

    string(TIMESTAMP _TIME_END "%s")
    math(EXPR _DUR "${_TIME_END} - ${_TIME_START}")
    println_msg("Completed(${_DUR}.0s). Artifacts Location: ${EXT_INSTALL_DIR}")

    if (NOT EXT_KEEP_BUILD)
        # Delete the actual library build directory (third layer)
        file(REMOVE_RECURSE "${EXT_BINARY_DIR}")
        # Delete the shadow project proxy directory (second layer)
        file(REMOVE_RECURSE "${EXT_ROOT}/${EXT_EP_PROXY_BUILD_NAME}")
    endif ()
endfunction(add_ext_dependency)