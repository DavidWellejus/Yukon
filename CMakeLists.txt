cmake_minimum_required(VERSION 3.27)
project(Yukon C)

set(CMAKE_C_STANDARD 11)

add_executable(Yukon main.c
        deck.c
        deck.h
        file.h
        file.c
        shuffle.c
        shuffle.h
        table.c
        table.h
)
