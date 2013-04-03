#include "storage.h"
#include "string.h"
//#include "stdlib.h"

int read_storage(char* buff, int length, char address){
	char cmd[258];
//	char* cmd;
//	cmd = (char*) malloc((length+2)*sizeof(char));
	memset(cmd, 0, 258);

	cmd[0] = (char) CMD_READ;
	cmd[1] = (char) address;

	SPIO_send(cmd, length+2);

	return SPIO_recv(buff);
}

/* Currently assumes writing a full page at a time, and that address are page-aligned */
int write_storage(char* buff, int length, char address){
//char* cmd;
	char cmd[18];

	if(length < 0 || length > 255){
		return -1;
	}
	
	if(length == 0){
		return 0;
	}

	if(length % 16 != 0 || address % 16 != 0){
		return -1;
	}

//	cmd = (char*) malloc(18*sizeof(char));
//	strncpy(cmd+2, buff, length);

	cmd[0] = (char) CMD_WRITE;
	switch((length-1) / 16){
		case 15: cmd[1] = (char) address + 240;
				 strncpy(cmd+2, buff+240, 16);
				 storage_write_enable();
				 SPIO_send(cmd, 18);
				 while(storage_read_status(STATUS_WIP));
		case 14: cmd[1] = (char) address + 224;
				 strncpy(cmd+2, buff+224, 16);
				 storage_write_enable();
				 SPIO_send(cmd, 18);
				 while(storage_read_status(STATUS_WIP));
		case 13: cmd[1] = (char) address + 208;
				 strncpy(cmd+2, buff+208, 16);
				 storage_write_enable();
				 SPIO_send(buff + 208, 18);
				 while(storage_read_status(STATUS_WIP));
		case 12: cmd[1] = (char) address + 192;
				 strncpy(cmd+2, buff+192, 16);
				 storage_write_enable();
				 SPIO_send(buff + 192, 18);
				 while(storage_read_status(STATUS_WIP));
		case 11: cmd[1] = (char) address + 176;
				 strncpy(cmd+2, buff+176, 16);
				 storage_write_enable();
				 SPIO_send(buff + 176, 18);
				 while(storage_read_status(STATUS_WIP));
		case 10: cmd[1] = (char) address + 160;
				 strncpy(cmd+2, buff+160, 16);
				 storage_write_enable();
				 SPIO_send(buff + 160, 18);
				 while(storage_read_status(STATUS_WIP));
		case  9: cmd[1] = (char) address + 144;
				 strncpy(cmd+2, buff+144, 16);
				 storage_write_enable();
				 SPIO_send(buff + 144, 18);
				 while(storage_read_status(STATUS_WIP));
		case  8: cmd[1] = (char) address + 128;
				 strncpy(cmd+2, buff+128, 16);
				 storage_write_enable();
				 SPIO_send(buff + 128, 18);
				 while(storage_read_status(STATUS_WIP));
		case  7: cmd[1] = (char) address + 112;
				 strncpy(cmd+2, buff+112, 16);
				 storage_write_enable();
				 SPIO_send(buff + 112, 18);
				 while(storage_read_status(STATUS_WIP));
		case  6: cmd[1] = (char) address + 96;
				 strncpy(cmd+2, buff+96, 16);
				 storage_write_enable();
				 SPIO_send(buff + 96, 18);
				 while(storage_read_status(STATUS_WIP));
		case  5: cmd[1] = (char) address + 80;
				 strncpy(cmd+2, buff+80, 16);
				 storage_write_enable();
				 SPIO_send(buff + 80, 18);
				 while(storage_read_status(STATUS_WIP));
		case  4: cmd[1] = (char) address + 64;
				 strncpy(cmd+2, buff+64, 16);
				 storage_write_enable();
				 SPIO_send(buff + 64, 18);
				 while(storage_read_status(STATUS_WIP));
		case  3: cmd[1] = (char) address + 48;
				 strncpy(cmd+2, buff+48, 16);
				 storage_write_enable();
				 SPIO_send(buff + 48, 18);
				 while(storage_read_status(STATUS_WIP));
		case  2: cmd[1] = (char) address + 32;
				 strncpy(cmd+2, buff+32, 16);
				 storage_write_enable();
				 SPIO_send(buff + 32, 18);
				 while(storage_read_status(STATUS_WIP));
		case  1: cmd[1] = (char) address + 16;
				 strncpy(cmd+2, buff+16, 16);
				 storage_write_enable();
				 SPIO_send(buff + 16, 18);
				 while(storage_read_status(STATUS_WIP));
		case  0: cmd[1] = (char) address;
				 strncpy(cmd+2, buff, 16);
				 storage_write_enable();
				 SPIO_send(buff, 18);
				 while(storage_read_status(STATUS_WIP));
	}
//	free(cmd);

	return 0;
}

int storage_read_status(char mask){
	char cmd[2];
	
	cmd[0] = CMD_RDSR;
	SPIO_send(cmd, 2);
	SPIO_recv(cmd);
	
	return (int) cmd[0] & mask;
}

int storage_write_enable(void){
	char cmd = CMD_WREN;
	SPIO_send(cmd, 1);
	
	// TODO: ensure write enable latch is clear
	return 0;
}
