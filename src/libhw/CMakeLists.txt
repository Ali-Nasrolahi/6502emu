list(APPEND src
    cpu.c
    device.c
    machine.c
)

add_library(libhw ${src})
