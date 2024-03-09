#include "isa.h"

#include <stdarg.h>

#define ISA_TABLE_SIZE (0xFF + 1)

void (*isa_table[ISA_TABLE_SIZE])(CPU *, RAM *);

/* #### Address Modes #### */
static void isa_addrmode_imp(CPU *cpu, RAM *ram)
{
    (void)cpu;
    (void)ram;
}

static void isa_addrmode_imm(CPU *cpu, RAM *ram) { cpu->active_addr = cpu->regs.pc++; }

static void isa_addrmode_rel(CPU *cpu, RAM *ram)
{
    cpu->active_addr = ram->read(ram, cpu->regs.pc++) + cpu->regs.pc;
}

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

static void isa_addrmode_inx(CPU *cpu, RAM *ram)
{
    _u16 ind_addr = ram->read(ram, cpu->regs.pc++);
    cpu->active_addr = (ram->read(ram, ind_addr + cpu->regs.ix) & 0x00ff) |
                       ((ram->read(ram, ind_addr + cpu->regs.ix + 1) & 0x00ff) << 8);
}

static void isa_addrmode_iny(CPU *cpu, RAM *ram)
{
    _u16 ind_addr = ram->read(ram, cpu->regs.pc++);
    cpu->active_addr = (ram->read(ram, ind_addr + cpu->regs.ix) & 0x00ff) |
                       ((ram->read(ram, ind_addr + cpu->regs.ix + 1) & 0x00ff) << 8);
}

static void isa_translate_addrmode(CPU *cpu, RAM *ram)
{
    _u16 opcode = cpu->regs.pc;
    void (*addrmode_f)(CPU *, RAM *) = NULL;

    switch ((opcode >> 2) & 0b00000111) {
    case 0:
        addrmode_f = (opcode & 0b11) == 0b01 ? isa_addrmode_inx : isa_addrmode_imm;
        break;
    case 1:
        addrmode_f = isa_addrmode_zrp;
        break;
    case 2:
        /*
         * For Accumulator addrmode,
         * implied set instead and later inside the instructions we handle it.
         */
        addrmode_f = (opcode & 0b11) == 0b01 ? isa_addrmode_imm : isa_addrmode_imp;
        break;
    case 3:
        addrmode_f = isa_addrmode_abs;
        break;
    case 4:
        addrmode_f = isa_addrmode_iny;
        break;
    case 5:
        /* For STX and LDX, "zero page,X" addressing becomes "zero page,Y" */
        addrmode_f = (opcode == 0xb6 || opcode == 0x96) ? isa_addrmode_zpy : isa_addrmode_zpx;
        break;
    case 6:
        addrmode_f = isa_addrmode_aby;
        break;
    case 7:
        /* And with LDX, "absolute,X" becomes "absolute,Y"  */
        addrmode_f = opcode == 0xbe ? isa_addrmode_aby : isa_addrmode_abx;
        break;
    }

    if (addrmode_f)
        addrmode_f(cpu, ram);
}

/* #### Instructions #### */
static void isa_nop(CPU *cpu, RAM *ram)
{
    (void)cpu;
    (void)ram;
}

static void isa_brk(CPU *cpu, RAM *ram)
{
    cpu->regs.pc++;
    cpu->regs.ps.flags.bc = 1;
}

static void isa_lda(CPU *cpu, RAM *ram)
{
    cpu->regs.acc = ram->read(ram, cpu->active_addr);
    cpu->regs.ps.flags.zf |= !cpu->regs.acc;
    cpu->regs.ps.flags.nf |= cpu->regs.acc & (1 << (7 - 1));
    cpu->regs.pc++;
}

static void isa_inx(CPU *cpu, RAM *ram)
{
    ++(cpu->regs.ix);
    cpu->regs.ps.flags.zf |= (!cpu->regs.ix);
    cpu->regs.ps.flags.nf |= (cpu->regs.ix & 0x80);
}

static void isa_new_instruction(void (*isa_f)(CPU *, RAM *), _u16 opcode, ...)
{
    va_list vl;

    isa_table[opcode] = isa_f;

    va_start(vl, opcode);

    while (opcode = (_u16)va_arg(vl, int))
        isa_table[opcode] = isa_f;

    va_end(vl);
}

void isa_init()
{
    for (int i = 0; i < ISA_TABLE_SIZE; i++)
        isa_table[i] = isa_nop;

    isa_table[0x00] = isa_brk;

    isa_new_instruction(isa_lda, 0xA9, 0xA5, 0xB5, 0xAD, 0xBD, 0xB9, 0xA1, 0xB1, NULL);
}

void isa_exec(CPU *cpu, RAM *ram)
{
    // isa_translate_addrmode(cpu, ram);
    isa_table[cpu->regs.pc](cpu, ram);
}