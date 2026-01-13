

add_library(bix_graphics OBJECT
        color.cpp
)

bix_module_setup(bix_graphics)
bix_module_add_headers(bix_graphics
        "assert.h" "graphics/color.h" "graphics/colors.h"
)


if (BIX_RENDERER_D2D)

    target_link_libraries(bix_graphics PRIVATE d2d1.lib dwrite.lib)
endif ()