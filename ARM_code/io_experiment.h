#ifndef __IO_EXPERIMENT_H__
#define	__IO_EXPERIMENT_H__
#include "LPC11xx.h"
#include "gpio.h"
#include "uart.h"
#include "stdlib.h"
#include "sha1.h"
#include "spio.h"
#include "storage.h"
#include "hasher.h"
#include "password.h"

#ifdef __DEBUG_H__
	#include "debug.h"
#endif

void test(const char*);
void set_step_2(const char*);
void open(const char*);
void close(const char*);
void set_step_1(const char*);

#endif
