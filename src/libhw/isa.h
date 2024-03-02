#ifndef _ISA_H_
#define _ISA_H_

#include "cpu.h"
#include "ram.h"

typedef void (*isa)(CPU *cpu, RAM *ram);

#endif /* _ISA_H_ */
