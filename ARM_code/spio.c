#include "spio.h"

char SPIO_buff[512];
int SPIO_read = 0;
int SPIO_index = 0;

void SPIO_enable(void){
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 16);

	LPC_IOCON->PIO0_2 &= ~0x07;
	LPC_IOCON->PIO0_2 |= 0x01;

	LPC_IOCON->PIO0_8 &= ~0x07;
	LPC_IOCON->PIO0_8 |= 0x01;

	LPC_IOCON->PIO0_9 &= ~0x07;
	LPC_IOCON->PIO0_9 |= 0x01;

	LPC_IOCON->PIO0_6 &= ~0x07;
	LPC_IOCON->PIO0_6 |= 0x02;

	LPC_IOCON->SCK_LOC |= 0x02;

	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 11);

	LPC_SYSCON->SSP0CLKDIV = 0x04;

	LPC_SYSCON->PRESETCTRL  |= 0x01;

	LPC_SSP0->CR0 = 0x00C7;
	LPC_SSP0->CR1 |= 0x02;
	LPC_SSP0->CPSR = 0x04;

	UART_data_write_string("SSP0->SR: 0x");
	UART_data_write(LPC_SSP0->SR & 0x0FF);
	UART_data_write_string("\r\n");

	while(SPIO_RNE()){
		int tmp = LPC_SSP0->DR;
	}
	
}

int SPIO_send(char* buf, int size){
	int i = 0;

	// Wait until any previous operations complete
	while(SPIO_BSY());
	while(i < size){
		// If the transmit FIFO gets full, wait until space opens up
		while(!SPIO_TNF());
		LPC_SSP0->DR = *(buf+i);
		// Pull any data out of the receive FIFO
		while(SPIO_RNE()){
			SPIO_buff[SPIO_index++] = (char) (LPC_SSP0->DR & 0x0FF);
		}
		i += 1;
	}
	// Wait until all data is sent/received
	while(SPIO_BSY() || SPIO_RNE()){
		if(SPIO_RNE()){
			SPIO_buff[SPIO_index++] = (char) (LPC_SSP0->DR & 0x0FF);
		}
	}
	return 0;
}

int SPIO_recv(char* buf, int length){
//	int i;
//	while(!SPIO_done);
	memcpy(buf, SPIO_buff+SPIO_read, length);
	SPIO_read += length;
	if(SPIO_read == SPIO_index){
		SPIO_read = SPIO_index = 0;
	}
//	SPIO_done = 0;
//	SPIO_index = 0;
//	LPC_SSP0->ICR |= 0x02;
//	while(SPIO_BSY());
//	for(i = 0; i < length; i++){
//		buf[i] = (char) (LPC_SSP0->DR & 0x0FF);
//	}
 	return SPIO_index;
}


