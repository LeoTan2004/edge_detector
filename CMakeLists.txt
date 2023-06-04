cmake_minimum_required(VERSION 3.24)
project(PictureDetector C)

set(CMAKE_C_STANDARD 99)

add_executable(PictureDetector grid.h bmpFile.c bmpFile.h main.c bmp.h bmp.c conv_std.h conv_std.c conv_support.c)
