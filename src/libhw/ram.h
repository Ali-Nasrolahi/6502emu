/**
 * @file ram.h
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-02-29
 */

#ifndef _RAM_H_
#define _RAM_H_
#include "device.h"

#define RAM_SIZE (0xffff) /* 64KB (64 * 1024) (1 << 16) */
#define RAM_OK_RANGE(_addr) (_addr > -1 && _addr <= RAM_SIZE)

struct _6502_ram {
    struct device dev;
    _u8 memory[64 * 1024];

    _u8 (*read)(struct _6502_ram *ram, _u16 addr);
    void (*write)(struct _6502_ram *ram, _u16 addr, _u8 data);
};

typedef struct _6502_ram RAM;

void ram_init(RAM *);

#endif /* _RAM_H_ */
