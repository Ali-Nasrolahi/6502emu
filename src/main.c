/**
 * @file main.cpp
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-02-25
 */

#include "libhw/machine.h"

_u8 program[] = {0};

int main(int argc, char **argv)
{
    machine_run(program, sizeof(program) / sizeof(_u8));
    return 0;
}
