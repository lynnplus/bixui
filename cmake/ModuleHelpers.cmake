

add_library(bix_internal_link_pool INTERFACE)

function(bix_module_setup NAME)
    target_link_libraries(${NAME} PRIVATE bix_build_config PUBLIC ${ARGN})
    target_link_libraries(bix_internal_link_pool INTERFACE ${ARGN})

    set_target_properties(${NAME} PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON
    )
endfunction()