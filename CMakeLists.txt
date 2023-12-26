cmake_minimum_required(VERSION 3.26)
project(CSV C)

set(CMAKE_C_STANDARD 23)

add_executable(CSV main.c
        csv.c
        csv.h
        csvUtils.c
        csvUtils.h
        )
