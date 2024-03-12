#include "machine.h"
#include <stdio.h>
#include <unistd.h>

void machine_run(_u8 *m, size_t size)
{
    cpu_init(&NES.cpu);
    ram_init(&NES.ram);

    ram_load(&NES.ram, m, size);

    cpu_loop(&NES.cpu, &NES.ram);
}
