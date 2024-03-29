/**
 * @file machine.h
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-02-27
 */

#ifndef _MACHINE_H_
#define _MACHINE_H_

#include "cpu.h"
#include "ram.h"

struct machine {
    CPU cpu;
    RAM ram;
};

static struct machine NES;

void machine_run(_u16 *flash, size_t size);

#endif /* _MACHINE_H_ */
