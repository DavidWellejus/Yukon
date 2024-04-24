cmake_minimum_required(VERSION 3.27)
project(Yukon C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(Yukon main.c
        deck.c
        deck.c
        deck.h
        deck.h
        file.h
        file.c
        file.c
        shuffle.c
        shuffle.h
        table.c
        table.h
        gui.c
        gui.h
        main.h
)

target_link_libraries(Yukon ${GTK3_LIBRARIES})