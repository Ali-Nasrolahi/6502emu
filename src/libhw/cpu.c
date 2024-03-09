#include "cpu.h"
#include "isa.h"
#include "ram.h"

extern isa isa_table[];

static void cpu_fetch_n_run(CPU *cpu, RAM *ram);

void cpu_init(CPU *cpu)
{
    struct _6502_registers *reg = &cpu->regs;

    memset(cpu, 0, sizeof(CPU));

    cpu->dev.name = "CPU";

    reg->sp = 0xfd;
    reg->ps.flags.un = 0x1;

    isa_init();
}

void cpu_loop(CPU *cpu, void *ram_ptr)
{
    RAM *ram = (RAM *)ram_ptr;
    cpu->regs.pc = ram->read(ram, 0xfffc) | (ram->read(ram, 0xfffc | 1) << 8);

    cpu_fetch_n_run(cpu, ram);
}

static void cpu_fetch_n_run(CPU *cpu, RAM *ram) { isa_table[cpu->regs.pc](cpu, ram); }
