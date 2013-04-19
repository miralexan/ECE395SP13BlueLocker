#include "uart.h"

int UART_done = 0;
char UART_buffer[512];
int UART_index = 0;
int UART_read = 0;

void UART_enable(){

	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);	   /* Turn on IOCON Block */

	/* Pins must be set before clock enable */

	/* Select RXD Function */
	LPC_IOCON->PIO1_6 &= ~0x07;
	LPC_IOCON->PIO1_6 |= 0x01;

	/* Select TXD function */
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01;
	
	/* Enable clock to UART module */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);

	/* Set UART clock division */
	LPC_SYSCON->UARTCLKDIV = 0x04;	/* divide clock by 4 */

	/* Set data word length to 8 bits.  Enable division latch access */
	LPC_UART->LCR |= 0x83;

	LPC_UART->DLL = 0x32;
	LPC_UART->DLM = 0x00;
	LPC_UART->FDR = 0xF9;

	/* Clear division latch access bit. */
	LPC_UART->LCR &= ~(0x00000080);

	/* Enable Send/Recieve queues */
	LPC_UART->FCR |= 0x07;

	/* Enable Interrupts */
	LPC_UART->IER |= 0x05;
}

void UART_disable() {

	/* Disable clock to UART module */
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 12);

}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int UART_recv(char* buff, const int length){
	int to_read = 0;
	int buff_size;

	while(!UART_done);

	buff_size = ((UART_index > UART_read) ? (UART_index) : (512 + UART_index)) - UART_read;	

	to_read = (length > buff_size ? buff_size : length);
	if (UART_read + to_read <= 512) {
		memcpy(buff, UART_buffer+UART_read, to_read);
	} else {
		memcpy(buff, UART_buffer + UART_read, 512 - UART_read);
		memcpy(buff + (512 - UART_read), UART_buffer, (UART_read + to_read - 512));
	} 
	UART_read += to_read;
	UART_read %= 512;
	UART_done = 0;
 	return to_read;
}

void UART_data_write (const char c) {

  while (!(LPC_UART->LSR & 0x20));
  LPC_UART->THR = c;

}

void UART_data_write_string(const char *string) {
	
	int i;

	for (i = 0; string[i] != '\0'; i++) {
		while (!(LPC_UART->LSR & 0x20));
		LPC_UART->THR = string[i];
	}

}

void UART_data_write_nstring(const char *string, const int length) {
	
	int i;

	for (i = 0; i < length ; i++) {
		while (!(LPC_UART->LSR & 0x20));
		LPC_UART->THR = string[i];
	}

}

void UART_send(const char *string, const int length) {
	
	int i;

	for (i = 0; i < length; i++) {
		UART_data_write(string[i]);
	}
}

extern void UART_IRQHandler(){

	UART_buffer[UART_index++] = UART_data_read();
	UART_index %= 512;

	if (UART_buffer[(UART_index == 0 ? 511 : UART_index-1)] == '\n'){
		if (UART_buffer[(UART_index <= 1 ? 510 + UART_index : UART_index-2)] == '\r'){
			// do stuff to process the string
			UART_buffer[(UART_index <= 1 ? 510 + UART_index : UART_index-2)] = '\0';
			UART_index = (UART_index == 0) ? 511 : (UART_index - 1);
			UART_done = 1;
			UART_interrupt_disable();
		}	
	}
}
