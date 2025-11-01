

include(FetchContent)
FetchContent_Declare(
        tinyxml
        GIT_REPOSITORY https://github.com/leethomason/tinyxml2
        GIT_TAG 9148bdf719e997d1f474be6bcc7943881046dba1) # 11.0.0
FetchContent_MakeAvailable(tinyxml)