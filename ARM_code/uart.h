/* uart.h
 * 
 * provides an abstract interface to the UART device (in this case, bluetooth)
 */

#ifndef __UART_H__
#define __UART_H__

#include "LPC11xx.h"
#include "string.h"

extern int UART_done;
extern char UART_buffer[];
extern int UART_index;

void UART_enable(void);
void UART_disable(void);
#define UART_interrupt_enable() NVIC_EnableIRQ((IRQn_Type) 21ul)
#define UART_interrupt_disable() NVIC_DisableIRQ((IRQn_Type) 21ul)

#define UART_data_read() (LPC_UART->RBR)

int UART_recv(char*, int);
void UART_data_write(char c);
void UART_data_write_string(char *string);
void UART_data_write_nstring(char*, int);

#endif
