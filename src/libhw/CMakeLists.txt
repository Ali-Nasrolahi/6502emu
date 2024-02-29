list(APPEND src
    device.c
    cpu.c
    ram.c
    machine.c
)

add_library(libhw ${src})
