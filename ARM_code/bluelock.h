#ifndef __IO_EXPERIMENT_H__
#define	__IO_EXPERIMENT_H__
#include "LPC11xx.h"
#include "gpio.h"
#include "device.h"
#include "stdlib.h"
#include "storage.h"
#include "password.h"
#include "debug.h"

void test(const char*);
void open(const char*);
void close(const char*);
void set(char*);
void fetch_new_pass(char*);

#endif
