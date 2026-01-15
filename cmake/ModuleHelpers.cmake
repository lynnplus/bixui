include_guard()

add_library(bix_internal_link_pool INTERFACE)

function(bix_module_setup TARGET_NAME)
    if (NOT TARGET ${TARGET_NAME})
        message(FATAL_ERROR "Target ${TARGET_NAME} must be defined before calling bix_module_setup")
    endif ()

    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)
    if (TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")
        set(LINK_KEYWORD INTERFACE)
    else ()
        set(LINK_KEYWORD PUBLIC)
        set_target_properties(${TARGET_NAME} PROPERTIES
                POSITION_INDEPENDENT_CODE ON
                CXX_VISIBILITY_PRESET hidden
                VISIBILITY_INLINES_HIDDEN ON
        )
    endif ()

    string(REPLACE "bix_" "" MODULE_NAME ${TARGET_NAME})

    add_library(bix::${MODULE_NAME} ALIAS ${TARGET_NAME})

    target_link_libraries(${TARGET_NAME} ${LINK_KEYWORD} bix::build_config)

    if (TARGET ${BIX_TARGET_NAME})
        target_link_libraries(${BIX_TARGET_NAME} PRIVATE bix::${MODULE_NAME})
    endif ()
endfunction()

function(bix_module_add_headers TARGET_NAME)
    if (NOT TARGET ${TARGET_NAME})
        message(FATAL_ERROR "Target ${TARGET_NAME} must be defined before calling bix_module_add_headers")
    endif ()

    set(header_files "")
    foreach (_hpath ${ARGN})
        if (IS_ABSOLUTE "${_hpath}")
            list(APPEND header_files "${_hpath}")
        else ()
            list(APPEND header_files "${BIX_HEADER_ROOT}/${_hpath}")
        endif ()
    endforeach ()

    target_sources(${TARGET_NAME} PUBLIC
            FILE_SET HEADERS
            BASE_DIRS "${BIX_INCLUDE_BASE}"
            FILES ${header_files}
    )

    install(TARGETS ${TARGET_NAME} FILE_SET HEADERS DESTINATION include)
endfunction()