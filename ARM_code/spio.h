#ifndef __SPIO_H__
#define __SPIO_H__

#include "LPC11xx.h"

void SPIO_enable(void);

int SPIO_send(void);

int SPIO_recv(void);

#endif
