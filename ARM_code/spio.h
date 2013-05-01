#ifndef __SPIO_H__
#define __SPIO_H__

#include "LPC11xx.h"
#include "device.h"
#include "string.h"

typedef struct device_t device;

#define SPIO_TFE() (LPC_SSP0->SR & 0x01)
#define SPIO_TNF() (LPC_SSP0->SR & 0x02)
#define SPIO_RNE() (LPC_SSP0->SR & 0x04)
#define SPIO_RFF() (LPC_SSP0->SR & 0x08)
#define SPIO_BSY() (LPC_SSP0->SR & 0x10)

void SPIO_enable(void);

int SPIO_send(device*, const char*, const int, const unsigned char);

int SPIO_recv(device*, char*, const int, const unsigned char);

int SPIO_flush(void);

#endif
