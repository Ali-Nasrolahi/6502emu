/**
 * @file isa.h
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-03-02
 */
#ifndef _ISA_H_
#define _ISA_H_

#include "cpu.h"
#include "ram.h"

#define ISA_TABLE_SIZE (0xFF + 1)

typedef void (*isa)(CPU *cpu, RAM *ram);

void isa_init(void);

#endif /* _ISA_H_ */
