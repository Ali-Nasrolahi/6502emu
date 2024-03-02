list(APPEND src
    device.c
    cpu.c
    isa.c
    ram.c
    machine.c
)

add_library(libhw ${src})
