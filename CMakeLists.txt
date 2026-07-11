cmake_minimum_required(VERSION 3.18)
project(BNM_Spawner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Android NDK specifics
if(ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL")
endif()

# BNM include
add_subdirectory(BNM)

# Our sources
add_library(BNM_Spawner SHARED
    src/main.cpp
)

target_include_directories(BNM_Spawner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/BNM/include
)

target_link_libraries(BNM_Spawner PRIVATE
    BNM
    log  # for __android_log_print
    dl   # for dlopen/dlsym if needed
)

# Strip symbols in release for smaller .so
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(BNM_Spawner PRIVATE -s)
endif()

# Output name
set_target_properties(BNM_Spawner PROPERTIES
    OUTPUT_NAME "BNM_Spawner"
    PREFIX "lib"
    SUFFIX ".so"
)
```

Now, the main.cpp with the actual mod logic.