include(FetchContent)

FetchContent_Declare(
    googletest
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()
