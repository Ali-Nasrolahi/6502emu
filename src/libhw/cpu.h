/**
 * @file cpu.h
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @brief Main CPU structures
 * @date 2024-02-27
 */
#ifndef _CPU_H_
#define _CPU_H_

#include "device.h"
union _process_status_reg_ {
    struct flags {
        _u8 cf : 1;      /* Carry Flag */
        _u8 zf : 1;      /* Zero Flag */
        _u8 id : 1;      /* Interrupt Disable */
        _u8 dm : 1;      /* Decimal Mode */
        _u8 bc : 1;      /* Break Command */
        _u8 of : 1;      /* Overflow Flag */
        _u8 nf : 1;      /* Negative Flag */
        _u8 padding : 1; /* Just to keep it aligned to single byte */
    } flags;

    _u8 reg;
};

struct _6502_registers {
    _u16 pc;                       /* Program counter */
    _u8 sp;                        /* Stack pointer */
    _u8 acc;                       /* Accumulator */
    _u8 ix;                        /* Index X */
    _u8 iy;                        /* Index Y */
    union _process_status_reg_ ps; /* Process Status */
};

struct _6502_cpu {
    struct device dev;
    struct _6502_registers regs;
};

typedef struct _6502_cpu CPU;

struct _6502_ram {
    struct device dev;
    _u8 memory[64 * 1024];

    _u8 (*read)(struct _6502_ram *ram, _u16 addr);
    void (*write)(struct _6502_ram *ram, _u16 addr, _u8 data);
};

typedef struct _6502_ram RAM;

#endif /* _CPU_H_ */