/* main.c
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#include "bluelock.h"

device flash;
device uart;

void lpc_init(){
	// Enable general purpose I/O block
	GPIO_enable();
	// Enable Asynchronous Serial block
	UART_enable();
	// Enable Synchronous Serial block
	SPIO_enable();

	storage_init();

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
	dinit(&flash, "FLASH");
	dinit(&uart, "UART");

	readpass();

	while(1){
		dread(&uart, input, 512, 0);
					
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
				dwrite_string(&uart, "command not recognized\r\n", 0);
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
	dwrite_string(&uart, "The hash on the flash was reset.\r\n", 0);
	dwrite_string(&uart, "The hash on the ARM was also reset.\r\n", 0);
	unsetpass();
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
		
		dwrite_string(&uart, "open solenoid activated\r\n", 0);

		GPIO0_output_low(GPIO_P7);
		GPIO0_output_high(GPIO_P4);
	} else if ((passisset() != 0) && (input[4] == '\0')) {
		dwrite_string(&uart, "password not supplied\r\n", 0);
	} else if ((input[4] == '\0') || (input[4] == ' ')) {
		dwrite_string(&uart, "password not recognized\r\n", 0);
	} else {
		dwrite_string(&uart, "command not recognized\r\n", 0);
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
		
		dwrite_string(&uart, "close solenoid activated\r\n", 0);

		GPIO0_output_high(GPIO_P7);
		GPIO0_output_low(GPIO_P4);
	} else if ((passisset() != 0) && (input[5] == '\0')) {
		dwrite_string(&uart, "password not supplied\r\n", 0);
	} else if ((input[5] == '\0') || (input[5] == ' ')) {
		dwrite_string(&uart, "password not recognized\r\n", 0);
	} else {
		dwrite_string(&uart, "command not recognized\r\n", 0);
	}
	
}

/* set
 * parameters - 
 *   input - The input string received from the user.
 * outputs - 
 *   none
 * side-effects - 
 *   Changes the password on the device, both in memory and in storage
 */
void set(char* input) {
	
	if ((passisset() == 0) && (input[3] == ' ')) {
		dwrite_string(&uart, "password is currently null, setting to ", 0);
		dwrite_string(&uart, input + 4, 0);
		dwrite_string(&uart, "\r\n", 0);
		setpass(input + 4);
		dwrite_string(&uart, "password set\r\n", 0);
	} else if ((passisset() == 0) && (input[3] == '\0')) {
		dwrite_string(&uart, "password null, no new password provided\r\n", 0);
		dwrite_string(&uart, "transitioning into password change state\r\n", 0);
	} else if (passisset() == 0) {
		dwrite_string(&uart, "command not recognized\r\n", 0);
	} else if ((input[3] == ' ') && checkpass(input + 4)) {

		dwrite_string(&uart, "old password matches\r\n", 0);
		dwrite_string(&uart, "transitioning into password change state\r\n", 0);
		fetch_new_pass(input);
		
	} else if (input[3] == ' ') {
		dwrite_string(&uart, "password not recognized\r\n", 0);
	} else if (input[3] == '\0') {
		dwrite_string(&uart, "password not supplied\r\n", 0);
	} else {
		dwrite_string(&uart, "command not recognized\r\n", 0);
	}
	
}

/* fetch_new_pass
 * parameters - 
 *   input - A scratch buffer used to recieve input
 * outputs - 
 *   none
 * side-effects - 
 *   Changes the password on the device, both in memory and in storage
 */
void fetch_new_pass(char* input) {
	UART_interrupt_enable();
	dread(&uart, input, 512, 0);

	if (strlen(input) != 0) {
		dwrite_string(&uart, "new password supplied, changing\r\n", 0);
		setpass(input);
		dwrite_string(&uart, "password changed\r\n", 0);
	} else {
		dwrite_string(&uart, "no new password, unsetting pass\r\n", 0);
		unsetpass();
		dwrite_string(&uart, "password reset\r\n", 0);
	}
}
