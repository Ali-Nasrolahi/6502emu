#include "ram.h"

static _u8 ram_read(RAM *ram, _u16 addr);
static void ram_write(RAM *ram, _u16 addr, _u8 data);

void ram_init(RAM *ram)
{
    memset(ram, 0, sizeof(RAM));
    ram->dev.name = "RAM";
    ram->read = ram_read;
    ram->write = ram_write;
}

static _u8 ram_read(RAM *ram, _u16 addr)
{
    if (RAM_OK_RANGE(addr))
        return ram->memory[addr];
    return 0;
}

static void ram_write(RAM *ram, _u16 addr, _u8 data)
{
    if (RAM_OK_RANGE(addr))
        ram->memory[addr] = data;
}