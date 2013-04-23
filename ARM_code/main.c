#include "io_experiment.h"

int password_state;
device flash;
device uart;

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
	char input[512];

	lpc_init();
	dinit(&flash, FLASH);
	dinit(&uart, UART);
	password_state = 0;

	readpass();

	while(1){
		dread(&uart, input, 512, 0);

		/* below is a test parser */
		
		if (password_state == 1) {
			set_step_2(input);
		} else {
			
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
				set_step_1(input);
			}
			
			else {
				dwrite_string(&uart, "command not recognized\r\n", 0);
			}
		}
		
		UART_interrupt_enable();
	}
//	return 0;	
}

void test(const char *input) {
/*	
	char buf[142];
	int i;
	
	memset(buf, 'a', 142);
	buf[140] = '\r';
	buf[141] = '\n';
*/	
	write_storage(&flash, "\02312342321323451678653421abcd\r\n", 32, 0);
/*	write_storage("0231234232132345167\0653421abcd\r\n", 32, 3);
	write_storage("This is the song that never...\r\n", 32, 35);
	write_storage("...ends for it goes on and on \r\n", 32, 67);
	write_storage("and on and on...  Streetlights\r\n", 32, 99);
	write_storage("I'm feeling your arms around  \r\n", 32, 131);
	write_storage("me on the other side.  It's...\r\n", 32, 163);
	write_storage("time to go; I'm hearing your  \r\n", 32, 195);
	write_storage("voice without words on theBOOM\r\n", 32, 227);
	
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
*/
	dwrite_string(&uart, "The hash on the flash was reset.\r\n", 0);
	if (strncmp(input + 4, " reset", 6) == 0) {
		UART_data_write_string("The hash on the ARM was also reset.\r\n");
		readpass();
	}

	//UART_data_write_string("Testing \\0: happy\0happy\r\n");
/*	
	for (i = 0; i < 35; i++) {
		read_storage(buf, 1, i);
		UART_data_write_string("The character read was 0x");
		UART_data_write((((buf[0] >> 4) & 0x0F) <= 9) ?
			(((buf[0] >> 4) & 0x0F) + '0') :
			(((buf[0] >> 4) & 0x0F) - 10 + 'A'));	
		UART_data_write(((buf[0] & 0x0F) <= 9) ?
			((buf[0] & 0x0F) + '0') :
			((buf[0] & 0x0F) - 10 + 'A'));
		UART_data_write_string(".\r\n");
	}
*/
}

void set_step_2(const char* input) {
	
	if (strlen(input) != 0) {
		dwrite_string(&uart, "new password supplied, changing\r\n", 0);
		setpass(input);
		dwrite_string(&uart, "password changed\r\n", 0);
	} else {
		dwrite_string(&uart, "no new password, unsetting pass\r\n", 0);
		unsetpass();
		dwrite_string(&uart, "password reset\r\n", 0);
	}
	password_state = 0;
	
}

void open(const char* input) {
	
	//int i;
	
	if (((passisset() == 0) && (input[4] == '\0')) ||
		((passisset() != 0) && (input[4] == ' ') && checkpass(input + 5))) {
		
		dwrite_string(&uart, "open solenoid activated\r\n", 0);
		//for (i = 0; i < 0x0007FFFF; i++) {}
		GPIO0_output_low(GPIO_P7);
		//GPIO0_output_high(GPIO_P4);
	} else if ((passisset() != 0) && (input[4] == '\0')) {
		dwrite_string(&uart, "password not supplied\r\n", 0);
	} else if ((input[4] == '\0') || (input[4] == ' ')) {
		dwrite_string(&uart, "password not recognized\r\n", 0);
	} else {
		dwrite_string(&uart, "command not recognized\r\n", 0);
	}
	
}

void close(const char* input) {
	
	//int i;
	
	if (((passisset() == 0) && (input[5] == '\0')) ||
		((passisset() != 0) && (input[5] == ' ') && checkpass(input + 6))) {
		
		dwrite_string(&uart, "close solenoid activated\r\n", 0);
		//for (i = 0; i < 0x0007FFFF; i++) {}
		GPIO0_output_high(GPIO_P7);
		//GPIO0_output_low(GPIO_P4);
	} else if ((passisset() != 0) && (input[5] == '\0')) {
		dwrite_string(&uart, "password not supplied\r\n", 0);
	} else if ((input[5] == '\0') || (input[5] == ' ')) {
		dwrite_string(&uart, "password not recognized\r\n", 0);
	} else {
		dwrite_string(&uart, "command not recognized\r\n", 0);
	}
	
}

void set_step_1(const char* input) {
	
	if ((passisset() == 0) && (input[3] == ' ')) {
		dwrite_string(&uart, "password is currently null, setting to ", 0);
		dwrite_string(&uart, input + 4, 0);
		dwrite_string(&uart, "\r\n", 0);
		setpass(input + 4);
		dwrite_string(&uart, "password set\r\n", 0);
	} else if ((passisset() == 0) && (input[3] == '\0')) {
		dwrite_string(&uart, "password null, no new password provided\r\n", 0);
		dwrite_string(&uart, "transitioning into password change state\r\n", 0);
		password_state = 1;
	} else if (passisset() == 0) {
		dwrite_string(&uart, "command not recognized\r\n", 0);
	} else if ((input[3] == ' ') && checkpass(input + 4)) {

		dwrite_string(&uart, "old password matches\r\n", 0);
		dwrite_string(&uart, "transitioning into password change state\r\n", 0);
		password_state = 1;
		
	} else if (input[3] == ' ') {
		dwrite_string(&uart, "password not recognized\r\n", 0);
	} else if (input[3] == '\0') {
		dwrite_string(&uart, "password not supplied\r\n", 0);
	} else {
		dwrite_string(&uart, "command not recognized\r\n", 0);
	}
	
}
