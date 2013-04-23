#ifndef __DEVICE_H__
#define __DEVICE_H__
#include "uart.h"
#include "spio.h"
#include "storage.h"
#include "string.h"

#define UART 0
#define SPIO 1
#define FLASH 2

typedef struct device_t device;
struct device_t{
	char flags;
	int (*read)(device*, char*, const int, const unsigned char);
	int (*write)(device*, const char*, const int, const unsigned char);
	int (*flush)();
};

int dinit(device*, const int);
int dread(device*, char*, const int, const unsigned char);
int dwrite(device*, const char*, const int, const unsigned char);
int dwrite_string(device*, const char*, const unsigned char);
int dflush(device*);
void dclear(device*);

#endif
