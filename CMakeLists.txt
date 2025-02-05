cmake_minimum_required(VERSION 3.22)
project(Game VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# Enable Clangd LSP integration with the project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type to Debug if none is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Add the DEBUG macro when building in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

# ------------------------------------------------------------------------------
# Third-party dependencies
# ------------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty")

add_subdirectory("${THIRDPARTY_DIR}/raylib")

# add_library(stb_image INTERFACE)
# target_include_directories(stb_image INTERFACE thirdparty)

# add_library(imgui 
#   thirdparty/imgui/imgui.cpp
#   thirdparty/imgui/imgui_draw.cpp
#   thirdparty/imgui/imgui_widgets.cpp
#   thirdparty/imgui/imgui_demo.cpp
#   thirdparty/imgui/imgui_tables.cpp
#   thirdparty/imgui/backends/imgui_impl_glfw.cpp
#   thirdparty/imgui/backends/imgui_impl_opengl3.cpp
# )
# target_include_directories(imgui PUBLIC thirdparty/imgui)
# target_link_libraries(imgui PRIVATE glfw)

# ------------------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------------------

set(MY_FLAGS "-std=c11" "-Wall")
set(SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/main.c"
)

add_executable(game ${SOURCE_FILES})
target_compile_options(game PUBLIC ${MY_FLAGS})

target_link_libraries(game PUBLIC raylib)
