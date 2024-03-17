/**
 * @file main.cpp
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-02-25
 */

#include "libhw/machine.h"

_u16 program[] = {
    0xA9, 0x0A /* LDA #10 */

};

int main(int argc, char **argv)
{

    machine_run(program, sizeof(program) / sizeof(_u16));
    return 0;
}
