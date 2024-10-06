include(FetchContent)

# Safetyhook
FetchContent_Declare(
        safetyhook
        GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
        GIT_TAG "origin/main"
)
message("Fetching safetyhook")
FetchContent_MakeAvailable(safetyhook)
set_property(TARGET safetyhook PROPERTY CXX_STANDARD 23)
set_property(TARGET safetyhook PROPERTY SAFETYHOOK_FETCH_ZYDIS ON)