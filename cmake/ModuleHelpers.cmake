

add_library(bix_internal_link_pool INTERFACE)

function(bix_module_setup NAME)
    target_link_libraries(${NAME} PRIVATE bix_build_config)
    target_link_libraries(${NAME} PUBLIC ${ARGN})
    target_link_libraries(bix_internal_link_pool INTERFACE ${ARGN})
endfunction()