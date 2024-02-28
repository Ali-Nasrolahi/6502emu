#include "machine.h"
#include <stdio.h>
#include <unistd.h>

void machine_run()
{
    if (init_cpu(&NES.cpu) < 0) {
        printf("CPU init failed.");
        _exit(-1);
    }
}
