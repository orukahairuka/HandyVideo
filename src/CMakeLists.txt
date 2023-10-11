cmake_minimum_required(VERSION 3.25)
project(HandyVideo C)

set(CMAKE_C_STANDARD 11)

find_package(FFMPEG REQUIRED)

add_executable(HandyVideo main.c Input/VideoInput.c Draw/PixFrameDraw.c)

target_include_directories(HandyVideo PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(HandyVideo PRIVATE ${FFMPEG_LIBRARY_DIRS})

target_link_libraries(HandyVideo PRIVATE ${FFMPEG_LIBRARIES})
