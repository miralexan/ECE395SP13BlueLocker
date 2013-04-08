#include "io_experiment.h"

extern void PIOINT0_IRQHandler(){

	GPIO0_output_toggle(GPIO_P4);
	GPIO0_interrupt_clear(GPIO_P6);

}

static int state = 0;

extern void UART_IRQHandler(){

	UART_buffer[UART_index++] = UART_data_read();

	if (UART_buffer[UART_index-1] == '\r') {
#if DEBUG
		UART_data_write_string("Carriage return\r\n");
#endif
		state = 1;
	} else if (state == 1) {
		if (UART_buffer[UART_index-1] == '\n'){
			// do stuff to process the string
#if DEBUG
			UART_data_write_string("I totally see a newline!\r\n");
#endif

			UART_buffer[UART_index - 2] = '\0';
#if DEBUG
			{
				UART_data_write_string("I see \"");
				UART_data_write_string(UART_buffer);
				UART_data_write_string("\"!\r\n");
			}
#endif
			UART_index = 0;
			UART_done = 1;
			UART_interrupt_disable();
			GPIO1_output_toggle(GPIO_P4);
		}
		state = 0;
	} 
}

extern void SSP0_IRQHANDLER(){
	UART_data_write_string("SPP0 Interrupt Called\r\n");
	SPIO_index = 0;
	while(LPC_SSP0->SR & 0x04 != 1){
		SPIO_buff[SPIO_index++] = (char) (LPC_SSP0->DR & 0x00FF);
	}
//	SPIO_done = 1;
}
