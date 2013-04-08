#ifndef __SPIO_H__
#define __SPIO_H__

#include "LPC11xx.h"
#include "string.h"
#include "uart.h"

#define SPIO_TFE() (LPC_SSP0->SR & 0x01)
#define SPIO_TNF() (LPC_SSP0->SR & 0x02)
#define SPIO_RNE() (LPC_SSP0->SR & 0x04)
#define SPIO_RFF() (LPC_SSP0->SR & 0x08)
#define SPIO_BSY() (LPC_SSP0->SR & 0x10)

extern char SPIO_buff[];
extern int SPIO_done;
extern int SPIO_index;

void SPIO_enable(void);

int SPIO_send(char*, int);

int SPIO_recv(char*, int);

void SPIO_flush(void);

#endif
