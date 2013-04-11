#include "io_experiment.h"

extern void PIOINT0_IRQHandler(){

	GPIO0_output_toggle(GPIO_P4);
	GPIO0_interrupt_clear(GPIO_P6);

}



/*extern void SSP0_IRQHANDLER(){
	UART_data_write_string("SPP0 Interrupt Called\r\n");
	SPIO_index = 0;
	while(LPC_SSP0->SR & 0x04 != 1){
		SPIO_buff[SPIO_index++] = (char) (LPC_SSP0->DR & 0x00FF);
	}
	SPIO_done = 1;
} */
