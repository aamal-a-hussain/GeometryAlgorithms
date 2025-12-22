cmake_minimum_required(VERSION 3.15)
project(CppGeometry)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-g -O0 -fno-limit-debug-info)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-g -O0)
endif()

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
    GIT_TAG        v1.91.5
)
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG        v3.0
)

option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(sfml)

# Dear ImGui
FetchContent_MakeAvailable(imgui)

# ImGui-SFML
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_SFML_USE_SFML_3 ON CACHE BOOL "" FORCE)
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "" FORCE)
set(IMGUI_SFML_IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "" FORCE)

FetchContent_MakeAvailable(imgui-sfml)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)
add_subdirectory(tests)

file(GLOB INC_FILES "inc/*.hpp")
add_library(CppGeometry INTERFACE ${INC_FILES})
target_include_directories(CppGeometry INTERFACE
    ${imgui_SOURCE_DIR}
    ${imgui-sfml_SOURCE_DIR}
)

file(GLOB RENDER_INC "inc/viz/*.h")
file(GLOB RENDER_SRC "src/viz/*.cpp")
add_executable(
                Geometry
                main.cpp
                ${RENDER_INC}
                ${RENDER_SRC}
            )
target_include_directories(Geometry PRIVATE ${imgui_SOURCE_DIR})
target_link_libraries(Geometry PRIVATE CppGeometry ImGui-SFML::ImGui-SFML)
