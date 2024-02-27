cmake_minimum_required(VERSION 3.20)

project(nes-emu LANGUAGES C CXX)

list(APPEND src
    main.c
)

add_executable(${CMAKE_PROJECT_NAME} ${src})
