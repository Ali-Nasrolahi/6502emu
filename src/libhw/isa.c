#include "isa.h"

#include <stdarg.h>

#define ISA_TABLE_SIZE (0xFF + 1)

void (*isa_table[ISA_TABLE_SIZE])(CPU *, RAM *);
_u8 isa_addrmode_table[ISA_TABLE_SIZE];

/* #### Address Modes #### */
static void isa_addrmode_acc(CPU *cpu, RAM *ram)
{
    /* Setting cpu 'unused' status flag to determine
     * whether address mode is Accumulator.
     */
    cpu->regs.ps.flags.un = 1;
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
        addrmode_f = (opcode & 0b11) == 0b01 ? isa_addrmode_imm : isa_addrmode_acc;
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

static void isa_ldx(CPU *cpu, RAM *ram)
{
    cpu->regs.ix = ram->read(ram, cpu->active_addr);
    cpu->regs.ps.flags.zf |= !cpu->regs.ix;
    cpu->regs.ps.flags.nf |= cpu->regs.ix & (1 << (7 - 1));
    cpu->regs.pc++;
}

static void isa_ldy(CPU *cpu, RAM *ram)
{
    cpu->regs.iy = ram->read(ram, cpu->active_addr);
    cpu->regs.ps.flags.zf |= !cpu->regs.iy;
    cpu->regs.ps.flags.nf |= cpu->regs.iy & (1 << (7 - 1));
    cpu->regs.pc++;
}

static void isa_sta(CPU *cpu, RAM *ram)
{
    ram->write(ram, cpu->active_addr, cpu->regs.acc);
    cpu->regs.pc++;
}

static void isa_stx(CPU *cpu, RAM *ram)
{
    ram->write(ram, cpu->active_addr, cpu->regs.ix);
    cpu->regs.pc++;
}
static void isa_sty(CPU *cpu, RAM *ram)
{
    ram->write(ram, cpu->active_addr, cpu->regs.iy);
    cpu->regs.pc++;
}

static void isa_tax(CPU *cpu, RAM *ram)
{
    cpu->regs.ix = cpu->regs.acc;
    cpu->regs.ps.flags.zf |= !cpu->regs.ix;
    cpu->regs.ps.flags.nf |= cpu->regs.ix & (1 << (7 - 1));
}

static void isa_tay(CPU *cpu, RAM *ram)
{
    cpu->regs.iy = cpu->regs.acc;
    cpu->regs.ps.flags.zf |= !cpu->regs.iy;
    cpu->regs.ps.flags.nf |= cpu->regs.iy & (1 << (7 - 1));
}

static void isa_txa(CPU *cpu, RAM *ram)
{
    cpu->regs.acc = cpu->regs.ix;
    cpu->regs.ps.flags.zf |= !cpu->regs.acc;
    cpu->regs.ps.flags.nf |= cpu->regs.acc & (1 << (7 - 1));
}

static void isa_tya(CPU *cpu, RAM *ram)
{
    cpu->regs.acc = cpu->regs.iy;
    cpu->regs.ps.flags.zf |= !cpu->regs.acc;
    cpu->regs.ps.flags.nf |= cpu->regs.acc & (1 << (7 - 1));
}

static void isa_new_instruction(void (*isa_f)(CPU *, RAM *), _u16 opcode, ...)
{
    va_list vl;

    isa_table[opcode] = isa_f;

    va_start(vl, opcode);

    while (opcode = (_u16)va_arg(vl, int)) {
        isa_table[opcode] = isa_f;
        isa_addrmode_table[opcode] = 0;
    }

    va_end(vl);
}

void isa_init()
{
    for (int i = 0; i < ISA_TABLE_SIZE; i++) {
        isa_table[i] = isa_nop;
        isa_addrmode_table[i] = 1;
    }

    isa_table[0x00] = isa_brk;

    /* Load/Store Operations */
    isa_new_instruction(isa_lda, 0xA9, 0xA5, 0xB5, 0xAD, 0xBD, 0xB9, 0xA1, 0xB1, NULL);
    isa_new_instruction(isa_ldx, 0xA2, 0xA6, 0xB6, 0xAE, 0xBE, NULL);
    isa_new_instruction(isa_ldy, 0xA0, 0xA4, 0xB4, 0xAC, 0xBC, NULL);
    isa_new_instruction(isa_sta, 0x85, 0x95, 0x8D, 0x9D, 0x99, 0x81, 0x91, NULL);
    isa_new_instruction(isa_stx, 0x86, 0x96, 0x8E, NULL);
    isa_new_instruction(isa_sty, 0x84, 0x94, 0x8C, NULL);

    /* Register Transfers (IMPLICIT) */
    isa_table[0xAA] = isa_tax;
    isa_table[0xA8] = isa_tay;
    isa_table[0x8A] = isa_txa;
    isa_table[0x98] = isa_tya;

}

void isa_exec(CPU *cpu, RAM *ram)
{
    if (isa_addrmode_table[cpu->regs.pc])
        isa_translate_addrmode(cpu, ram);

    isa_table[cpu->regs.pc](cpu, ram);
}
