#ifndef __IO_EXPERIMENT_H__
#define	__IO_EXPERIMENT_H__
#include "LPC11xx.h"
#include "gpio.h"
#include "uart.h"
#include "string.h"
#include "stdlib.h"
#include "sha1.h"
#include "spio.h"

#define DEBUG 1

void setpass(SHA1Context*, char*);
int checkpass(SHA1Context*, char*, int);

#endif
