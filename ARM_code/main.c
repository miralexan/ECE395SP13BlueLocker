#include "io_experiment.h"

int password_state;

void lpc_init(){
	GPIO_enable();
	UART_enable();
	SPIO_enable();

	GPIO0_dir_output(GPIO_P7);
	GPIO0_dir_output(GPIO_P4);
	GPIO1_dir_output(GPIO_P4);

	GPIO0_output_high(GPIO_P7);
	GPIO0_output_low(GPIO_P4);	
	GPIO0_output_high(GPIO_P2);

	UART_interrupt_enable();
}

int main(){
	int i;

	lpc_init();
	password_state = 0;

	while(1){
		while (UART_done == 0);

		/* below is a test parser */
		
		/* test */
		if (strncmp(UART_buffer, "test", 4) == 0) {
			char buf[142];
			memset(buf, 'a', 142);
			buf[140] = '\r';
			buf[141] = '\n';
			write_storage("\02312342321323451678653421abcd\r\n", 32, 0);
			write_storage("12312342321323451678653421abcd\r\n", 32, 32);
			write_storage("12312342321323451678653421abcd\r\n", 32, 64);
			write_storage("12312342321323451678653421abcd\r\n", 32, 96);
			write_storage("12312342321323451678653421abcd\r\n", 32, 128);
			write_storage("12312342321323451678653421abcd\r\n", 32, 160);
			write_storage("12312342321323451678653421abcd\r\n", 32, 192);
			write_storage("12312342321323451678653421abcd\r\n", 32, 224);
			read_storage(buf, 140, 0);	   
			UART_data_write_nstring(buf, 142);
			read_storage(buf, 140, 50);	   
			UART_data_write_nstring(buf, 142);
			read_storage(buf, 140, 70);	   
			UART_data_write_nstring(buf, 142);
			read_storage(buf, 140, 30);	   
			UART_data_write_nstring(buf, 142);
			read_storage(buf, 140, 11);	   
			UART_data_write_nstring(buf, 142);
		}

		if (password_state == 1) {
			if (strlen(UART_buffer) != 0) {
				UART_data_write_string("new password supplied, changing\r\n");
				setpass(UART_buffer);
				UART_data_write_string("password changed\r\n");
			} else { //strlen(UART_buffer) == 0
				UART_data_write_string("no new password, unsetting pass\r\n");
				unsetpass();
				UART_data_write_string("password reset\r\n");
			}
			password_state = 0;
		} else { // password_state == 0
			
			/* test */
			//if (strncmp(UART_buffer, "test", 4) == 0) {
			//	sha1_test();
			//}
			/* this is disabled to remove the direct dependency of main on sha1.h */

			/* open [password] */
			if (strncmp(UART_buffer, "open", 4) == 0) { 
				if (((passisset()) && (UART_buffer[4] == '\0')) ||
					((UART_buffer[4] == ' ') && checkpass(UART_buffer + 5))) {
					
					UART_data_write_string("open solenoid activated\r\n");
					for (i = 0; i < 0x0007FFFF; i++) {}
					GPIO0_output_low(GPIO_P7);
					GPIO0_output_high(GPIO_P4);
				} else if ((UART_buffer[4] == '\0') || (UART_buffer[4] == ' ')) {
					UART_data_write_string("password not recognized\r\n");
				} else {
					UART_data_write_string("command not recognized\r\n");
				}
			}

			/* close [password]	*/
			else if (strncmp(UART_buffer, "close", 5) == 0) {
				if (((passisset()) && (UART_buffer[5] == '\0')) ||
					((UART_buffer[5] == ' ') && checkpass(UART_buffer + 6))) {
					
					UART_data_write_string("close solenoid activated\r\n");
					for (i = 0; i < 0x0007FFFF; i++) {}
					GPIO0_output_high(GPIO_P7);
					GPIO0_output_low(GPIO_P4);
				} else if ((UART_buffer[5] == '\0') || (UART_buffer[5] == ' ')) {
					UART_data_write_string("password not recognized\r\n");
				} else {
					UART_data_write_string("command not recognized\r\n");
				}
			}

			/* set [oldpass] [newpass] 
			 * oldpass is a paramater iff a password is currently set*/

			else if (strncmp(UART_buffer, "set", 3) == 0) {
				if ((passisset()) && (UART_buffer[3] == ' ')) {
					UART_data_write_string("password is currently null, setting to ");
					UART_data_write_string(UART_buffer + 4);
					UART_data_write_string("\r\n");
					setpass(UART_buffer + 4);
					UART_data_write_string("password set\r\n");
				} else if ((passisset()) && (UART_buffer[3] == '\0')) {
					UART_data_write_string("password null, no new password provided\r\n");
					UART_data_write_string("transitioning into password change state\r\n");
					password_state = 1;
				} else if (passisset()) {
					UART_data_write_string("command not recognized\r\n");
				} else if ((UART_buffer[3] == ' ') && checkpass(UART_buffer + 4)) {

					UART_data_write_string("old password matches\r\n");
					UART_data_write_string("transitioning into password change state\r\n");
					password_state = 1;
					
				} else if (UART_buffer[3] == ' ') {
					UART_data_write_string("password not recognized\r\n");
				} else {
					UART_data_write_string("command not recognized\r\n");
				}
			} else {
				UART_data_write_string("command not recognized\r\n");
			}
		}

		UART_done = 0;		
		UART_interrupt_enable();
	}
//	return 0;	
}

