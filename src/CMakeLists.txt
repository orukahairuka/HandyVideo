cmake_minimum_required(VERSION 3.25)
project(HandyVideo C)

set(CMAKE_C_STANDARD 11)

add_executable(HandyVideo main.c Input/VideoInput.c ImageDraw/ImageDraw.c)
