#ifndef __SPIO_H__
#define __SPIO_H__

#include "LPC11xx.h"
#include "string.h"

/* SPIO_TFE
 * Inputs: None
 * Outputs:
 *   Returns a value greater than 0 if the transmit FIFO is
 *   empty, 0 otherwise.
 * Side-Effects: None
 */
#define SPIO_TFE() (LPC_SSP0->SR & 0x01)

/* SPIO_TNF
 * Inputs: None
 * Outputs:
 *   Returns a value greater than 0 if the transmit FIFO is
 *   not full, 0 if it is.
 * Side-Effects: None
 */
#define SPIO_TNF() (LPC_SSP0->SR & 0x02)

/* SPIO_RNE
 * Inputs: None
 * Outputs:
 *   Returns a value greater than 0 if the receive FIFO is
 *   not empty, 0 otherwise.
 * Side-Effects: None
 */
#define SPIO_RNE() (LPC_SSP0->SR & 0x04)

/* SPIO_RFF
 * Inputs: None
 * Outputs:
 *   Returns a value greater than 0 if the receive FIFO is
 *   full, 0 otherwise.
 * Side-Effects: None
 */
#define SPIO_RFF() (LPC_SSP0->SR & 0x08)

/* SPIO_BSY
 * Inputs: None
 * Outputs:
 *   Returns a value greater than 1 if the SPIO block is
 *   actively sending or receiving, 0 otherwise.
 * Side-Effects: None
 */
#define SPIO_BSY() (LPC_SSP0->SR & 0x10)

void SPIO_enable(void);

int SPIO_send(const char*, const int);

int SPIO_recv(char*, const int);

void SPIO_flush(void);

#endif
