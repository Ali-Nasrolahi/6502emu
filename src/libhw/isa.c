#include "isa.h"

isa isa_table[ISA_TABLE_SIZE];

/* #### Address Modes #### */
static void isa_addrmode_acc(CPU *cpu, RAM *ram) {}

static void isa_addrmode_imm(CPU *cpu, RAM *ram) { cpu->active_addr = cpu->regs.pc++; }

static void isa_addrmode_rel(CPU *cpu, RAM *ram) {}

static void isa_addrmode_zrp(CPU *cpu, RAM *ram)
{
    cpu->active_addr = ram->read(ram, cpu->regs.pc++) & 0x00ff;
}

static void isa_addrmode_zpx(CPU *cpu, RAM *ram)
{
    cpu->active_addr = ram->read(ram, cpu->regs.pc++ + cpu->regs.ix) & 0x00ff;
}

static void isa_addrmode_zpy(CPU *cpu, RAM *ram)
{
    cpu->active_addr = ram->read(ram, cpu->regs.pc++ + cpu->regs.iy) & 0x00ff;
}

static void isa_addrmode_abs(CPU *cpu, RAM *ram)
{
    cpu->active_addr = ram->read(ram, cpu->regs.pc++) | (ram->read(ram, cpu->regs.pc++) << 8);
}

static void isa_addrmode_abx(CPU *cpu, RAM *ram)
{
    cpu->active_addr =
        ram->read(ram, cpu->regs.pc++) | (ram->read(ram, cpu->regs.pc++) << 8) + cpu->regs.ix;
}

static void isa_addrmode_aby(CPU *cpu, RAM *ram)
{
    cpu->active_addr =
        ram->read(ram, cpu->regs.pc++) | (ram->read(ram, cpu->regs.pc++) << 8) + cpu->regs.iy;
}

static void isa_addrmode_ind(CPU *cpu, RAM *ram)
{
    /* Has bug in hardware */
    _u16 ind_addr = ram->read(ram, cpu->regs.pc++) | (ram->read(ram, cpu->regs.pc++) << 8);
    cpu->active_addr = ram->read(ram, ind_addr) | (ram->read(ram, ind_addr + 1) << 8);
}

static void isa_addrmode_inx(CPU *cpu, RAM *ram) {}
static void isa_addrmode_iny(CPU *cpu, RAM *ram) {}

static void isa_translate_addrmode(CPU *cpu, RAM *ram, _u16 opcode) {}

/* #### Instructions #### */
static void isa_brk(CPU *cpu, RAM *ram) {}

static void isa_inx(CPU *cpu, RAM *ram)
{
    ++(cpu->regs.ix);
    cpu->regs.ps.flags.zf |= (!cpu->regs.ix);
    cpu->regs.ps.flags.nf |= (cpu->regs.ix & 0x80);
}

void isa_init()
{
    memset(isa_table, 0, sizeof(isa) * ISA_TABLE_SIZE);

    isa_table[0x00] = isa_brk;
}