cmake_minimum_required(VERSION 3.27)
project(GitCraft C)

set(CMAKE_C_STANDARD 11)

add_executable(gitcraftd src/main.c
        src/main.h)