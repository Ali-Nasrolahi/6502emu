/**
 * @file isa.h
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-03-02
 */
#ifndef _ISA_H_
#define _ISA_H_

#include "cpu.h"
#include "ram.h"

void isa_init(void);
void isa_exec(CPU *cpu, RAM *ram);

#endif /* _ISA_H_ */
