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
	char input[512];

	lpc_init();
	password_state = 0;

	readpass();

	while(1){
		UART_recv(input, 512);

		/* below is a test parser */
		
		if (password_state == 1) {
			set_step_2(input);
		} else { // password_state == 0
			
			/* test */
			if (strncmp(input, "test", 4) == 0) {
				test();
			}

			/* open [password] */
			if (strncmp(input, "open", 4) == 0) { 
				open(input);
			}

			/* close [password]	*/
			else if (strncmp(input, "close", 5) == 0) {
				close(input);
			}

			/* set [oldpass] [newpass] 
			 * oldpass is a paramater iff a password is currently set*/

			else if (strncmp(input, "set", 3) == 0) {
				set_step_1(input);
			}
			
			else {
				UART_data_write_string("command not recognized\r\n");
			}
		}
		
		UART_interrupt_enable();
	}
//	return 0;	
}

void test() {
	
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

void set_step_2(char* input) {
	
	if (strlen(input) != 0) {
		UART_data_write_string("new password supplied, changing\r\n");
		setpass(input);
		UART_data_write_string("password changed\r\n");
	} else { //strlen(input) == 0
		UART_data_write_string("no new password, unsetting pass\r\n");
		unsetpass();
		UART_data_write_string("password reset\r\n");
	}
	password_state = 0;
	
}

void open(char* input) {
	
	int i;
	
	if (((passisset()) && (input[4] == '\0')) ||
		((input[4] == ' ') && checkpass(input + 5))) {
		
		UART_data_write_string("open solenoid activated\r\n");
		//for (i = 0; i < 0x0007FFFF; i++) {}
		GPIO0_output_low(GPIO_P7);
		//GPIO0_output_high(GPIO_P4);
	} else if ((input[4] == '\0') || (input[4] == ' ')) {
		UART_data_write_string("password not recognized\r\n");
	} else {
		UART_data_write_string("command not recognized\r\n");
	}
	
}

void close(char* input) {
	
	int i;
	
	if (((passisset()) && (input[5] == '\0')) ||
		((input[5] == ' ') && checkpass(input + 6))) {
		
		UART_data_write_string("close solenoid activated\r\n");
		//for (i = 0; i < 0x0007FFFF; i++) {}
		GPIO0_output_high(GPIO_P7);
		//GPIO0_output_low(GPIO_P4);
	} else if ((input[5] == '\0') || (input[5] == ' ')) {
		UART_data_write_string("password not recognized\r\n");
	} else {
		UART_data_write_string("command not recognized\r\n");
	}
	
}

void set_step_1(char* input) {
	
	if ((passisset()) && (input[3] == ' ')) {
		UART_data_write_string("password is currently null, setting to ");
		UART_data_write_string(input + 4);
		UART_data_write_string("\r\n");
		setpass(input + 4);
		UART_data_write_string("password set\r\n");
	} else if ((passisset()) && (input[3] == '\0')) {
		UART_data_write_string("password null, no new password provided\r\n");
		UART_data_write_string("transitioning into password change state\r\n");
		password_state = 1;
	} else if (passisset()) {
		UART_data_write_string("command not recognized\r\n");
	} else if ((input[3] == ' ') && checkpass(input + 4)) {

		UART_data_write_string("old password matches\r\n");
		UART_data_write_string("transitioning into password change state\r\n");
		password_state = 1;
		
	} else if (input[3] == ' ') {
		UART_data_write_string("password not recognized\r\n");
	} else {
		UART_data_write_string("command not recognized\r\n");
	}
	
}
