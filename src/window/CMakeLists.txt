

set(_window_headers
        "window/screen.h"
        "window/window.h"
)
if (WIN32)
    list(APPEND _window_headers "window/window_win.h")
endif ()


add_library(bix_window OBJECT
        window.cpp
        window_private.h window_private.cpp
        native_window.h native_window.cpp
)

bix_module_setup(bix_window)
bix_module_add_headers(bix_window ${_window_headers})


if (WIN32)
    target_sources(bix_window PRIVATE
            backends/win32/class_manager.cpp
            backends/win32/class_manager.h
            backends/win32/win32_encoding.cpp
            backends/win32/win32_encoding.h
            backends/win32/win32_screen.cpp
            backends/win32/win32_screen.h
            backends/win32/win32_utils.cpp
            backends/win32/win32_utils.h
            backends/win32/win32_window.cpp
            backends/win32/win32_window.h
    )
endif ()

#    target_compile_definitions(bix_build_config INTERFACE WINVER=0xA00 _WIN32_WINNT=0xA00)