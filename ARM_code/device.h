#ifndef __DEVICE_H__
#define __DEVICE_H__
#include "string.h"

typedef struct device_t device;

typedef struct func_t{
	int (*read)(device*, char*, const int, const unsigned char);
	int (*write)(device*, const char*, const int, const unsigned char);
	int (*flush)();
} func_t;
struct device_t{
	char flags;
	func_t* function;
};

int dadd(const char*, func_t*);
int dinit(device*, const char*);
int dread(device*, char*, const int, const unsigned char);
int dwrite(device*, const char*, const int, const unsigned char);
int dwrite_string(device*, const char*, const unsigned char);
int dflush(device*);
void dclear(device*);

#endif
