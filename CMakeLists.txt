cmake_minimum_required(VERSION 3.24)
project(PictureDetector C)

set(CMAKE_C_STANDARD 99)

add_executable(PictureDetector class.h class.c bmpFile.h bmpFile.c main.c)
