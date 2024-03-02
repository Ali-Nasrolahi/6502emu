#include "machine.h"
#include <stdio.h>
#include <unistd.h>

void machine_run()
{
    cpu_init(&NES.cpu);
    ram_init(&NES.ram);

    cpu_loop(&NES.cpu, &NES.ram);
}
