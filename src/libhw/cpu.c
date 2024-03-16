#include "cpu.h"
#include "isa.h"
#include "ram.h"

static void cpu_reset(CPU *cpu, RAM *ram);
static void cpu_nmi(CPU *cpu, RAM *ram);
static void cpu_irq(CPU *cpu, RAM *ram);

void cpu_init(CPU *cpu) { isa_init(); }

void cpu_loop(CPU *cpu, void *ram_ptr)
{
    cpu_reset(cpu, (RAM *)ram_ptr);
    isa_exec(cpu, (RAM *)ram_ptr);
}

void cpu_reset(CPU *cpu, RAM *ram)
{
    memset(cpu, 0, sizeof(CPU));

    cpu->dev.name = "CPU";
    cpu->regs.pc = ram->read(ram, 0xFFFC) | (ram->read(ram, 0xFFFC | 1) << 8);
    cpu->regs.sp = 0xFD;
}

void cpu_irq(CPU *cpu, RAM *ram)
{
    if (cpu->regs.ps.flags.id)
        return;

    ram->write(ram, 0x0100 + cpu->regs.sp--, (cpu->regs.pc >> 8) & 0x00FF);
    ram->write(ram, 0x0100 + cpu->regs.sp--, cpu->regs.pc & 0x00FF);

    cpu->regs.ps.flags.bc = 0;
    cpu->regs.ps.flags.id = 1;

    ram->write(ram, 0x0100 + cpu->regs.sp--, cpu->regs.ps.reg);

    cpu->regs.pc = ram->read(ram, 0xFFFE) | (ram->read(ram, 0xFFFE | 1) << 8);
}

void cpu_nmi(CPU *cpu, RAM *ram)
{
    cpu_irq(cpu, ram);

    cpu->regs.pc = ram->read(ram, 0xFFFA) | (ram->read(ram, 0xFFFA | 1) << 8);
}
