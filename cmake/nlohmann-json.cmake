include(FetchContent)

set(JSON_BuildTests OFF CACHE INTERNAL "")

FetchContent_Declare(
    json
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)
