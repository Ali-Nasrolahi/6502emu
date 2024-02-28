/**
 * @file device.h
 * @author Ali Nasrolahi (a.nasrolahi01@gmail.com)
 * @date 2024-02-27
 */
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint16_t _u16;
typedef uint8_t _u8;

struct device {
    const char *name;
};

#endif /* _DEVICE_H_ */