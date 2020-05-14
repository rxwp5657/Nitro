cmake_minimum_required (VERSION 2.8)

# Project Name
project (Nitro)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 14)


add_compile_definitions(STORAGE_SSE)

#Project headers
include_directories(include/graphics/)
include_directories(include/core/)
include_directories(include/events/)
include_directories(include/input/)
include_directories(include/utils/)
#Project libraries

file(GLOB SOURCES src/*/*.cpp src/*)

# Dependencies

add_executable(nitro ${SOURCES})

add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

add_subdirectory(lib/glew)
include_directories(lib/glew/include/GL)

add_subdirectory(lib/clutch)
include_directories(lib/clutch/include)

add_subdirectory(lib/assimp)
include_directories(lib/assimp/include)

target_link_libraries(nitro clutch assimp glfw libglew_shared libglewmx_shared ${GLFW_LIBRARIES})