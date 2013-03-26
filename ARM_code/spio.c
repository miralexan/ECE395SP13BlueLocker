#include "spio.h"

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

	LPC_SSP0->CR0 = 0xFF0F;
	LPC_SSP0->CR1 |= 0x02;
	LPC_SSP0->CPSR = 0x02;

	while(LPC_SSP0->SR & 0x04 == 1){
		int tmp = LPC_SSP0->DR;
	}
}

int SPIO_send(void){
	while(LPC_SSP0->SR & 0x01 == 0);
	LPC_SSP0->DR = 0x0001;
	return 0;
}

int SPIO_recv(void){
 	return 0;
}
