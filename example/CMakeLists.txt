project(example)

add_executable(example WIN32
        main.cpp
        frame_window.h
        frame_window.cpp
        resources.h
        resources.rc
)

if (MINGW)
    target_sources(example PRIVATE manifest.rc)
elseif (MSVC)
    # Default manifest can be suppressed with the following:
    #target_link_options(example PRIVATE "LINKER:/MANIFEST:NO")
    target_sources(example PRIVATE example.manifest)
endif ()

target_link_libraries(example PRIVATE bix)

#
#add_custom_command (
#        TARGET example POST_BUILD
#        COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:example>"
#        "$<TARGET_RUNTIME_DLLS:example>" COMMAND_EXPAND_LISTS
#)

#add_custom_command(TARGET example POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:example> $<TARGET_FILE_DIR:example>
#        COMMAND_EXPAND_LISTS
#)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #    get_filename_component(binPath "${CMAKE_CXX_COMPILER}" DIRECTORY)
    #    add_custom_command (
    #            TARGET example POST_BUILD
    #            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
    #            "${binPath}/libc++.dll"
    #            "$<TARGET_FILE_DIR:example>"
    #
    #    )
endif ()
install(TARGETS example)
#install(FILES $<TARGET_RUNTIME_DLLS:example> TYPE BIN)