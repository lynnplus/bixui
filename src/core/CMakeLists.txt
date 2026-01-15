


add_library(bix_core OBJECT
        length.cpp
        interface.cpp
)

bix_module_setup(bix_core)
bix_module_add_headers(bix_core
        core/insets.h core/length.h core/scene.h core/widget_host.h
        core/layout_types.h
)