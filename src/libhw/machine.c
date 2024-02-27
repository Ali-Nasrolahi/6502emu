#include "machine.h"
#include <stdio.h>

void machine_run()
{
    if (init_cpu(&CPU_BUS, &NES.cpu) < 0) {
        printf("CPU init failed.");
        _exit(-1);
    }
}
