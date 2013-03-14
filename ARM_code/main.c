#include "io_experiment.h"

char password[512];
//int password_length;
int password_set;
int password_state;

void lpc_init(){
	GPIO_enable();
	UART_enable();

	GPIO0_dir_output(GPIO_P7);
	GPIO0_dir_output(GPIO_P4);
	GPIO0_dir_output(GPIO_P2);

	GPIO0_dir_input(GPIO_P6); // we don't actually need this
	GPIO0_interrupt_enable(GPIO_P6);

	GPIO0_output_high(GPIO_P7);
	GPIO0_output_low(GPIO_P4);	
	GPIO0_output_high(GPIO_P2);
	
	GPIO_interrupt_enable();
	UART_interrupt_enable();
	// if we use this often enough, it might be best to make another file
	//   for interrupt abstraction 
}

int main(){
	int i;
	char pass[HASH_LENGTH];

	lpc_init();
	memset(pass, 0, HASH_LENGTH);
	password_set = 0;
	password_state = 0;

	while(1){
		while (UART_done == 0) {}

#if DEBUG
		{
			UART_data_write_string("password currently set to ");
			UART_data_write_nstring(pass, HASH_LENGTH);
			UART_data_write_string("\r\n");
		}
#endif 

		/* below is a test parser */

		if (password_state == 1) {
			if (strlen(UART_buffer) != 0) {
				UART_data_write_string("new password supplied, changing\r\n");
				setpass(UART_buffer, pass);
				password_set = 1;
				UART_data_write_string("password changed\r\n");
			} else { //strlen(UART_buffer) == 0
				UART_data_write_string("no new password, unsetting pass\r\n");
				password_set = 0;
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
				if (((password_set == 0) && (UART_buffer[4] == '\0')) ||
					((UART_buffer[4] == ' ') && checkpass((char *)&pass, UART_buffer + 5))) {
					
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
				if (((password_set == 0) && (UART_buffer[5] == '\0')) ||
					((UART_buffer[5] == ' ') && checkpass((char *)&pass, UART_buffer + 6))) {
					
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
	#if DEBUG
				{
					UART_data_write_string("password is ");
					UART_data_write_nstring(pass, HASH_LENGTH);
					//UART_data_write_string("password length is ");
					//UART_data_write('0' + (password_length/10));
					//UART_data_write('0' + (password_length%10));
					UART_data_write_string("\r\n");
				}
	#endif			
				if ((password_set == 0) && (UART_buffer[3] == ' ')) {
					UART_data_write_string("password is currently null, setting to ");
					UART_data_write_string(UART_buffer + 4);
					UART_data_write_string("\r\n");
					setpass(UART_buffer + 4, pass);
					password_set = 1;
	#if DEBUG
					{
						UART_data_write_string("password set to ");
						UART_data_write_nstring(pass, HASH_LENGTH);
						UART_data_write_string("\r\n");
					}
	#endif
					UART_data_write_string("password set\r\n");
				} else if (password_set == 0 && UART_buffer[3] == '\0') {
					UART_data_write_string("password null, no new password provided\r\n");
					UART_data_write_string("transitioning into password change state\r\n");
					password_state = 1;
				} else if (password_set == 0) {
					UART_data_write_string("command not recognized\r\n");
				} else if ((UART_buffer[3] == ' ') && checkpass(pass, UART_buffer + 4)) {

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

