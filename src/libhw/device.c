#include "device.h"

int8_t bus_register_dev(struct bus_type *bus, struct device *dev)
{
    uint16_t free_dev_index;
    for (; free_dev_index < BUS_DEV_LIST_SIZE && bus->dev_list[free_dev_index++];)
        ;

    return free_dev_index < BUS_DEV_LIST_SIZE && (bus->dev_list[free_dev_index] = dev) ? 0 : -1;
}