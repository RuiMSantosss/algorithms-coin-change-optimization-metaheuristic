cmake_minimum_required(VERSION 3.29)
project(TP2_TREPA_COLINS C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(TP2_TREPA_COLINS
    algorithm.c
    algorithm.h
    main.c
    utils.c
    utils.h)
