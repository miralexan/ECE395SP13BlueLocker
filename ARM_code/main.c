#include "io_experiment.h"

void lpc_init(){
	// Enable general purpose I/O block
	GPIO_enable();
	// Enable Asynchronous Serial block
	UART_enable();
	// Enable Synchronous Serial block
	SPIO_enable();

	// Set I/O0 port 7 to output (solenoid control/open indicator)
	GPIO0_dir_output(GPIO_P7);
	// Set I/O0 port 4 to output (closed indicator)
	GPIO0_dir_output(GPIO_P4);
	// Set I/O1 port 4 to output (end of command indicator)
	GPIO1_dir_output(GPIO_P4);

	// Set initial output levels
	GPIO0_output_high(GPIO_P7);
	GPIO0_output_low(GPIO_P4);	
	GPIO1_output_high(GPIO_P4);

	// Begin listening to UART
	UART_interrupt_enable();
}

int main(){
	char input[512];

	lpc_init();

	readpass();

	while(1){
		UART_recv(input, 512);
			
		/* test */
		if (strncmp(input, "test", 4) == 0) {
			test(input);
		}

		/* open [password] */
		else if (strncmp(input, "open", 4) == 0) { 
			open(input);
		}

		/* close [password]	*/
		else if (strncmp(input, "close", 5) == 0) {
			close(input);
		}

		/* set oldpass[\r\nnewpass]
		 * set [newpass] <- if password is unset
		 * oldpass is a paramater iff a password is currently set*/
		else if (strncmp(input, "set", 3) == 0) {
			set(input);
		}
			
		else {
			UART_data_write_string("command not recognized\r\n");
		}
		
		UART_interrupt_enable();
	}
//	return 0;	
}

/* test
 * parameters - 
 *   input - The input string received from the user.
 * outputs - 
 *   none
 * side-effects - 
 *   resets any previously set password in storage
 */
void test(const char *input) {

	write_storage("\02312342321323451678653421abcd\r\n", 32, 0);

	UART_data_write_string("The hash on the flash was reset.\r\n");
	//if (strncmp(input + 4, " reset", 6) == 0) {
		UART_data_write_string("The hash on the ARM was also reset.\r\n");
		readpass();
	//}
}

/* open
 * parameters - 
 *   input - The input string received from the user.
 * outputs - 
 *   none
 * side-effects - 
 *   Sets GPIO0_4 high and GPIO0_7 low, changing output LEDs
 *   and activating the solenoid
 */
void open(const char* input) {
	
	if (((passisset() == 0) && (input[4] == '\0')) ||
		((passisset() != 0) && (input[4] == ' ') && checkpass(input + 5))) {
		
		UART_data_write_string("open solenoid activated\r\n");
		GPIO0_output_low(GPIO_P7);
		GPIO0_output_high(GPIO_P4);
	} else if ((passisset() != 0) && (input[4] == '\0')) {
		UART_data_write_string("password not supplied\r\n");
	} else if ((input[4] == '\0') || (input[4] == ' ')) {
		UART_data_write_string("password not recognized\r\n");
	} else {
		UART_data_write_string("command not recognized\r\n");
	}
	
}

/* close
 * parameters - 
 *   input - The input string received from the user.
 * outputs - 
 *   none
 * side-effects - 
 *   Sets GPIO0_4 low and GPIO0_7 high, changing output LEDs
 *   and deactivating the solenoid
 */
void close(const char* input) {
	
	if (((passisset() == 0) && (input[5] == '\0')) ||
		((passisset() != 0) && (input[5] == ' ') && checkpass(input + 6))) {
		
		UART_data_write_string("close solenoid activated\r\n");
		GPIO0_output_high(GPIO_P7);
		GPIO0_output_low(GPIO_P4);
	} else if ((passisset() != 0) && (input[5] == '\0')) {
		UART_data_write_string("password not supplied\r\n");
	} else if ((input[5] == '\0') || (input[5] == ' ')) {
		UART_data_write_string("password not recognized\r\n");
	} else {
		UART_data_write_string("command not recognized\r\n");
	}
	
}

void set(char* input) {
	
	if ((passisset() == 0) && (input[3] == ' ')) {
		UART_data_write_string("password is currently null, setting to ");
		UART_data_write_string(input + 4);
		UART_data_write_string("\r\n");
		setpass(input + 4);
		UART_data_write_string("password set\r\n");
	} else if ((passisset() == 0) && (input[3] == '\0')) {
		UART_data_write_string("password null, no new password provided\r\n");
		UART_data_write_string("transitioning into password change state\r\n");
	} else if (passisset() == 0) {
		UART_data_write_string("command not recognized\r\n");
	} else if ((input[3] == ' ') && checkpass(input + 4)) {

		UART_data_write_string("old password matches\r\n");
		UART_data_write_string("transitioning into password change state\r\n");
		fetch_new_pass(input);
		
	} else if (input[3] == ' ') {
		UART_data_write_string("password not recognized\r\n");
	} else if (input[3] == '\0') {
		UART_data_write_string("password not supplied\r\n");
	} else {
		UART_data_write_string("command not recognized\r\n");
	}
	
}

void fetch_new_pass(char* input) {
	UART_recv(input, 512);

	if (strlen(input) != 0) {
		UART_data_write_string("new password supplied, changing\r\n");
		setpass(input);
		UART_data_write_string("password changed\r\n");
	} else {
		UART_data_write_string("no new password, unsetting pass\r\n");
		unsetpass();
		UART_data_write_string("password reset\r\n");
	}
}
