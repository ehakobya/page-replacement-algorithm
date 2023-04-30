cmake_minimum_required(VERSION 3.25)
project(page_replacement_algorithm C)

set(CMAKE_C_STANDARD 99)

add_executable(page_replacement_algorithm src/Main.c headers/DataLoader.h headers/PageTable.h src/HakobyanDataLoader.c src/HakobyanPageTable.c)
