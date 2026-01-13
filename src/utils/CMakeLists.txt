

add_library(bix_utils OBJECT
        assert.cpp
)

bix_module_setup(bix_utils)
bix_module_add_headers(bix_utils
        "assert.h" "utils/flags.h" "utils/concepts.h" "utils/fmt_wrapper.h"
        "utils/numeric.h"
)



