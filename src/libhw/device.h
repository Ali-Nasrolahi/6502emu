/**
 * @file device.h
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-02-27
 */
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdint.h>

#define BUS_DEV_LIST_SIZE (1 << 4)

struct device {
    const char *name;
    int (*open)(void);
    int (*read)(void *buf, uint16_t size);
    int (*write)(void *buf, uint16_t size);
    int (*close)(void);
};

struct bus_type {
    const char *name;
    struct device *dev_list[BUS_DEV_LIST_SIZE];
};

int8_t bus_register_dev(struct bus_type *bus, struct device *dev);

#endif /* _DEVICE_H_ */