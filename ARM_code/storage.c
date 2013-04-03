#include "storage.h"
#include "string.h"
#include "stdlib.h"

int read_storage(char* buff, int length, char address){
	char* cmd;
	cmd = (char*) malloc((length+2)*sizeof(char));
	memset(cmd, 0, length+2);

	cmd[0] = (char) CMD_READ;
	cmd[1] = (char) address;

	SPIO_send(cmd, length+2);

	return SPIO_recv(buff);
}

/* Currently assumes writing a full page at a time */
int write_storage(char* buff, int length, char address){
	char* cmd;

	if(length < 0 || length >= 256){
		return -1;
	}

	if(length % 16 != 0 || address % 16 != 0){
		return -1;
	}

	cmd = (char*) malloc(18*sizeof(char));
	strncpy(cmd+2, buff, length);

	cmd[0] = (char) CMD_WREN;
	SPIO_send(cmd, 1);

	cmd[0] = (char) CMD_WRITE;
	switch(length / 16){
		case 15: cmd[1] = (char) address + 240;
				 strncpy(cmd+2, buff+240, 16); 
				 SPIO_send(cmd, 18);
		case 14: cmd[1] = (char) address + 224;
				 strncpy(cmd+2, buff+224, 16);
				 SPIO_send(cmd, 18);
		case 13: cmd[1] = (char) address + 208;
				 strncpy(cmd+2, buff+208, 16);
				 SPIO_send(buff + 208, 18);
		case 12: cmd[1] = (char) address + 192;
				 strncpy(cmd+2, buff+192, 16);
				 SPIO_send(buff + 192, 18);
		case 11: cmd[1] = (char) address + 176;
				 strncpy(cmd+2, buff+176, 16);
				 SPIO_send(buff + 176, 18);
		case 10: cmd[1] = (char) address + 160;
				 strncpy(cmd+2, buff+160, 16);
				 SPIO_send(buff + 160, 18);
		case  9: cmd[1] = (char) address + 144;
				 strncpy(cmd+2, buff+144, 16);
				 SPIO_send(buff + 144, 18);
		case  8: cmd[1] = (char) address + 128;
				 strncpy(cmd+2, buff+128, 16);
				 SPIO_send(buff + 128, 18);
		case  7: cmd[1] = (char) address + 112;
				 strncpy(cmd+2, buff+112, 16);
				 SPIO_send(buff + 112, 18);
		case  6: cmd[1] = (char) address + 96;
				 strncpy(cmd+2, buff+96, 16);
				 SPIO_send(buff + 96, 18);
		case  5: cmd[1] = (char) address + 80;
				 strncpy(cmd+2, buff+80, 16);
				 SPIO_send(buff + 80, 18);
		case  4: cmd[1] = (char) address + 64;
				 strncpy(cmd+2, buff+64, 16);
				 SPIO_send(buff + 64, 18);
		case  3: cmd[1] = (char) address + 48;
				 strncpy(cmd+2, buff+48, 16);
				 SPIO_send(buff + 48, 18);
		case  2: cmd[1] = (char) address + 32;
				 strncpy(cmd+2, buff+32, 16);
				 SPIO_send(buff + 32, 18);
		case  1: cmd[1] = (char) address + 16;
				 strncpy(cmd+2, buff+16, 16);
				 SPIO_send(buff + 16, 18);
		case  0: cmd[1] = (char) address;
				 strncpy(cmd+2, buff, 16);
				 SPIO_send(buff, 18);
	}
	cmd[0] = (char) CMD_WRDI;
	SPIO_send(cmd, 1);
	free(cmd);

	return 0;
}
