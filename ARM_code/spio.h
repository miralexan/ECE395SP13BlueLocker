#ifndef __SPIO_H__
#define __SPIO_H__

#include "LPC11xx.h"

void SPIO_enable(void);

int SPIO_send(char*, int);

int SPIO_recv(char*);

#endif
