/* device.h
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#ifndef __DEVICE_H__
#define __DEVICE_H__
#include "string.h"

typedef struct device_t device;

// Structure defining read/write/flush functions for peripherals
typedef struct func_t{
	int (*read)(device*, char*, const int, const unsigned char);
	int (*write)(device*, const char*, const int, const unsigned char);
	int (*flush)();
} func_t;

// Interface for peripherals, contains status flags
// This was intended to provide additional context for return values
// (ie, why an error was thrown or fewer bytes where read/written than
//  expected), however we ran out of time before fully implementing it.
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
