cmake_minimum_required(VERSION 3.18)
project(BNM_Spawner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Android NDK specifics
if(ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL")
endif()

# BNM submodule
add_subdirectory(BNM)

# ImGui submodule (add with: git submodule add https://github.com/ocornut/imgui imgui)
# You can add backend .cpp files here later if needed
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
)
target_include_directories(imgui PUBLIC imgui)

# Main library
add_library(BNM_Spawner SHARED
    src/main.cpp
    src/menu.cpp
)

target_include_directories(BNM_Spawner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/BNM/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

target_link_libraries(BNM_Spawner PRIVATE
    BNM
    imgui
    log
    dl
    EGL
    GLESv2
)

# Strip symbols in release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(BNM_Spawner PRIVATE -s)
endif()

set_target_properties(BNM_Spawner PROPERTIES
    OUTPUT_NAME "BNM_Spawner"
    PREFIX "lib"
    SUFFIX ".so"
)
