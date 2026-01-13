include_guard()

find_package(Doxygen REQUIRED)
if (DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_HTML_COLORSTYLE "TOGGLE")
    set(DOXYGEN_INCLUDE_PATH include)
    set(DOXYGEN_STRIP_FROM_INC_PATH include/)
    set(DOXYGEN_STRIP_FROM_PATH include/)
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
    set(DOXYGEN_GROUP_NESTED_COMPOUNDS YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md")
    set(DOXYGEN_SORT_BRIEF_DOCS NO)
    set(DOXYGEN_SORT_MEMBER_DOCS NO)
    #USE_STAMP_FILE
    doxygen_add_docs(bix_docs ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/README.md COMMENT "Generating Doxygen documentation")
else ()
    message(WARNING
            "Doxygen not found. Documentation will not be generated.
To use doxygen please set the cmake variable 'Doxygen_ROOT' or the system environment variable PATH
")
endif ()