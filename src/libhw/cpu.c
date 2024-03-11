#include "cpu.h"
#include "isa.h"
#include "ram.h"

void cpu_init(CPU *cpu)
{
    struct _6502_registers *reg = &cpu->regs;

    memset(cpu, 0, sizeof(CPU));

    cpu->dev.name = "CPU";

    reg->sp = 0xfd;

    isa_init();
}

void cpu_loop(CPU *cpu, void *ram_ptr)
{
    RAM *ram = (RAM *)ram_ptr;
    cpu->regs.pc = ram->read(ram, 0xfffc) | (ram->read(ram, 0xfffc | 1) << 8);

    isa_exec(cpu, (RAM *)ram);
}
