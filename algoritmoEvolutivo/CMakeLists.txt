cmake_minimum_required(VERSION 3.27)
project(algoritmoEvolutivo)

set(CMAKE_CXX_STANDARD 17)

add_executable(algoritmoEvolutivo
        utils.c
        utils.h
        algorithm.h
        main.c
        funcao.c
        funcao.h
        algorithm.c
        algorithm.c)
