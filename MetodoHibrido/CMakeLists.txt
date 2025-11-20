cmake_minimum_required(VERSION 3.29)
project(MetdodoHibrido2 C)

set(CMAKE_C_STANDARD 99)

add_executable(MetdodoHibrido2 main.c
        algorithm.c
        funcao.c
        utils.c)
