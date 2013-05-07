/* uart.h
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#ifndef __UART_H__
#define __UART_H__

#include "LPC11xx.h"
#include "string.h"
#include "gpio.h"
#include "device.h"

extern int UART_done;
extern char UART_buffer[];
extern int UART_index;

void UART_enable(void);
void UART_disable(void);
#define UART_interrupt_enable() NVIC_EnableIRQ((IRQn_Type) 21ul)
#define UART_interrupt_disable() NVIC_DisableIRQ((IRQn_Type) 21ul)

#define UART_data_read() (LPC_UART->RBR)

int UART_recv(device*, char*, const int, const unsigned char);
int UART_send(device*, const char *, const int, const unsigned char);
void UART_data_write(const char c);
int UART_flush(void);

#endif
