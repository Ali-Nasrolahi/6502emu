#include "cpu.h"
#include "ram.h"

static void cpu_fetch(CPU *cpu, RAM *ram);
static void cpu_exec(CPU *cpu);

void cpu_init(CPU *cpu)
{
    struct _6502_registers *reg = &cpu->regs;

    memset(cpu, 0, sizeof(CPU));
    cpu->dev.name = "CPU";
    reg->sp = 0xfd;
    reg->ps.flags.un = 0x1;
}

void cpu_loop(CPU *cpu, void *ram)
{
    cpu_fetch(cpu, (RAM *)ram);
    cpu_exec(cpu);
}

static void cpu_fetch(CPU *cpu, RAM *ram) {}

static void cpu_exec(CPU *cpu) {}