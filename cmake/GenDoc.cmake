

find_package(Doxygen)
if (DOXYGEN_FOUND)
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
    doxygen_add_docs(project_doc ${includeFiles} README.md ALL USE_STAMP_FILE)
else ()
    message(WARNING
            "Doxygen not found. Documentation will not be generated.
To use doxygen please set the cmake variable 'Doxygen_ROOT' or the system environment variable PATH
")
endif ()