add_subdirectory(libhw)

list(APPEND src
    main.c
)

add_executable(${CMAKE_PROJECT_NAME} ${src})
target_link_libraries(${CMAKE_PROJECT_NAME} libhw)
