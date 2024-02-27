#include "cpu.h"
#include <unistd.h>

static int cpu_open(void);
static int cpu_read(void *buf, uint16_t size);
static int cpu_write(void *buf, uint16_t size);
static int close();

int init_cpu(struct bus_type *bus, CPU *cpu)
{
    memset(cpu, 0, sizeof(CPU));
    cpu->dev.name = "CPU";
    cpu->dev.open;
    return 0;
}
