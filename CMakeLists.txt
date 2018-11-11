cmake_minimum_required(VERSION 3.0)

project(fractalSDL)

file(GLOB SDL_INCLUDES includes/*.h)

file(GLOB SDL_SOURCES sources/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SDL_SOURCES} ${SDL_INCLUDES})

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} includes)

set(CMAKE_VERBOSE_MAKEFILE on)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBDIR}/libSDL2-2.0.so.0.9.0)
