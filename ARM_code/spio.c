#include "spio.h"

char SPIO_buff[512];
int SPIO_read = 0;
int SPIO_index = 0;

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
}

int SPIO_send(const char* buf, const int size){
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

int SPIO_recv(char* buf, const int length){
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

void SPIO_flush(void){
	SPIO_read = SPIO_index;
}

