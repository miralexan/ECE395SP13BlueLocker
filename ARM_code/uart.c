/* uart.c
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#include "uart.h"

int UART_done = 0;
char UART_buffer[512];
int UART_index = 0;
int UART_read = 0;

static func_t functions;

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

	functions.read = &UART_recv;
	functions.write = &UART_send;
	functions.flush = &UART_flush;
	dadd("UART", &functions);
}

void UART_disable() {

	/* Disable clock to UART module */
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 12);

}

/* UART_recv
 * Inputs:
 *   device_h - a pointer to a device structure
 *   buff - A pointer to a character buffer to collect input
 *   length - The number of characters to receive.
 *   garbage - unused
 * Outputs:
 *   Returns the number of bytes received.
 * Side-Effects:
 *   Blocks until the UART_done flag is set. Clears the UART_done flag
 *   before returning.
 * Notes:
 *   UART_buffer is implemented as a circular buffer, which is why the start/stop points
 *   and buff_size calculations are so involved.
 */
int UART_recv(device* device_h, char* buff, const int length, const unsigned char garbage){
	int to_read = 0;
	int buff_size;

	// Block until read is complete (device receives a \r\n sequence)
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

/* UART_data_write
 * Inputs:
 *   c - character to be transmitted
 * Outputs: None
 * Side-Effects: None
 */
void UART_data_write (const char c) {
  // wait for space in the transmit FIFO to become available
  while (!(LPC_UART->LSR & 0x20));
  LPC_UART->THR = c;

}

/* UART_send
 * Inputs:
 *   device_h - a pointer to a device structure
 *   string - A pointer to a character buffer to transmit.
 *   length - The size of the buffer to be sent
 *   garbage - unused
 * Outputs:
 *   Returns the number of bytes sent.
 * Side-Effects: None
 */
int UART_send(device* device_h, const char *string, const int length, const unsigned char garbage) {
	
	int i;

	for (i = 0; i < length; i++) {
		UART_data_write(string[i]);
	}

	return length;
}

/* UART_flush
 * Inputs: None
 * Outputs:	None
 * Side-Effects:
 *   Clears the SPIO buffer.
 */
int UART_flush(void){
	UART_read = UART_index;
	return 0;
}

/* UART_IRQHandler
 * Inputs: None
 * Outputs:	None
 * Side-Effects:
 *   Called when the UART peripheral issues an unmasked interrupt.
 *   Pulls characters out of the UART device buffer and stores them
 *   in UART_buffer. When the end-of-transmission sequence \r\n is detected,
 *   sets the UART_done flag and masks interrupts.
 */
extern void UART_IRQHandler(){

	UART_buffer[UART_index++] = UART_data_read();
	UART_index %= 512;

	if (UART_buffer[(UART_index == 0 ? 511 : UART_index-1)] == '\n'){
		if (UART_buffer[(UART_index <= 1 ? 510 + UART_index : UART_index-2)] == '\r'){
			// do stuff to process the string
			UART_buffer[(UART_index <= 1 ? 510 + UART_index : UART_index-2)] = '\0';
			UART_index = (UART_index == 0) ? 511 : (UART_index - 1);
			UART_done = 1;
			GPIO1_output_toggle(GPIO_P4);
			UART_interrupt_disable();
		}	
	}
}
