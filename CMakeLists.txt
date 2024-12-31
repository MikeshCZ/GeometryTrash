cmake_minimum_required(VERSION 3.14)

project(GeometryTrash)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch libs
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# don't build the examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAYGUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) 
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

# Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

# Enable Metal support on macOS
if(APPLE)
    set(USE_METAL ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(raylib)

# Raygui
FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raygui)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Přidání složky audio do výstupu
add_custom_target(CopyAudio ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/audio
    ${CMAKE_BINARY_DIR}/audio
)

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${raygui_SOURCE_DIR})
add_dependencies(${PROJECT_NAME} CopyAudio)
