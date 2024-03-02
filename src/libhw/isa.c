#include "isa.h"

static void isa_test(CPU *cpu, RAM *ram)
{
    printf("cpu: %s, ram: %s\n", cpu->dev.name, ram->dev.name);
}

const isa isa_table[] = {isa_test};
