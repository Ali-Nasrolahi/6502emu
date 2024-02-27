/**
 * @file machine.h
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-02-27
 */

#ifndef _MACHINE_H_
#define _MACHINE_H_

#include "cpu.h"

struct machine {
    CPU cpu;
};

static struct bus_type CPU_BUS;
static struct machine NES;

void machine_run(void);

#endif /* _MACHINE_H_ */