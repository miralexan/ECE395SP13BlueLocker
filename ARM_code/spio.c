/* spio.c
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#include "spio.h"

char SPIO_buff[512];
int SPIO_read = 0;
int SPIO_index = 0;

static func_t functions;

void SPIO_enable(void){
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 16);

	// Set SSP output pins
	LPC_IOCON->PIO0_2 &= ~0x07;
	LPC_IOCON->PIO0_2 |= 0x01;

	LPC_IOCON->PIO0_8 &= ~0x07;
	LPC_IOCON->PIO0_8 |= 0x01;

	LPC_IOCON->PIO0_9 &= ~0x07;
	LPC_IOCON->PIO0_9 |= 0x01;

	LPC_IOCON->PIO0_6 &= ~0x07;
	LPC_IOCON->PIO0_6 |= 0x02;

	LPC_IOCON->SCK_LOC |= 0x02;

	// Enable SSP0 clock
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 11);

	// Set SSP0 clock division
	LPC_SYSCON->SSP0CLKDIV = 0x04;

	LPC_SYSCON->PRESETCTRL  |= 0x01;

	// Set the SSP0 attributes:
	//   DSS: 8 bits
	//   FRF: SPI
	//   CPOL: 1
	//   CPHA: 1
	//   SCR: 0
	// 	 LBM: Disabled
	//   SPI Enable: True
	//   M/S: Master
	//   SOD: N/A
	//   CPS: 4
	LPC_SSP0->CR0 = 0x00C7;
	LPC_SSP0->CR1 |= 0x02;
	LPC_SSP0->CPSR = 0x04;

    // Empty any data in the receive FIFO
	while(SPIO_RNE()){
		int tmp = LPC_SSP0->DR;
	}
	
	functions.read = &SPIO_recv;
	functions.write = &SPIO_send;
	functions.flush = &SPIO_flush;
	dadd("SPIO", &functions);	
}

/* SPIO_send
 * Inputs:
 *   device_h - a pointer to a device structure
 *   buf - A pointer to a character buffer to transmit.
 *   size - The size of the buffer to be sent
 *   garbage - unused
 * Outputs:
 *   Returns the number of bytes sent.
 * Side-Effects: None
 */
int SPIO_send(device* device_h, const char* buf, const int size, const unsigned char garbage){
	int i = 0;

	if (((SPIO_index >= SPIO_read) ? (SPIO_read + 512) : (SPIO_read))
		<= SPIO_index + size) {

		return -1;
	}

	// Wait until any previous operations complete
	while(SPIO_BSY());
	while(i < size){
		// If the transmit FIFO gets full, wait until space opens up
		while(!SPIO_TNF());
		LPC_SSP0->DR = *(buf+i);
		// Pull any data out of the receive FIFO
		if(SPIO_RNE()){
			SPIO_buff[SPIO_index++] = (char) (LPC_SSP0->DR & 0x0FF);
			SPIO_index %= 512;
		}
		i += 1;
	}
	// Wait until all data is sent/received
	while(SPIO_BSY() || SPIO_RNE()){
		if(SPIO_RNE()){
			SPIO_buff[SPIO_index++] = (char) (LPC_SSP0->DR & 0x0FF);
			SPIO_index %= 512;
		}
	}
	return 0;
}

/* SPIO_recv
 * Inputs:
 *   device_h - a pointer to a device structure
 *   buf - A pointer to a character buffer to collect input
 *   length - The number of characters to receive.
 *   garbage - unused
 * Outputs:
 *   Returns the number of bytes received.
 * Side-Effects: None
 * Notes:
 *   SPIO_buff is implemented as a circular buffer, which is why the start/stop points
 *   and buff_size calculations are so involved.
 */
int SPIO_recv(device* device_h, char* buf, const int length, const unsigned char garbage){
	int to_read = 0;
	int buff_size = ((SPIO_index >= SPIO_read) ? (SPIO_index) : (512 + SPIO_index)) - SPIO_read;

	to_read = (length > buff_size ? buff_size : length);
	if (SPIO_read + to_read <= 512) {
		memcpy(buf, SPIO_buff+SPIO_read, to_read);
	} else {
		memcpy(buf, SPIO_buff + SPIO_read, 512 - SPIO_read);
		memcpy(buf + (512 - SPIO_read), SPIO_buff, (SPIO_read + to_read - 512));
	} 
	SPIO_read += to_read;
	SPIO_read %= 512;
 	return to_read;
}

/* SPIO_flush
 * Inputs: None
 * Outputs:	None
 * Side-Effects:
 *   Clears the SPIO buffer.
 */
int SPIO_flush(void){
	SPIO_read = SPIO_index;
	return 0;
}

