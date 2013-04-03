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
	int start_page, end_page, start_len, end_len, send;
	
	start_page = end_page = start_len = end_len = send = 0;

	if(length <= 0 || length > 255){
		return -1;
	} else if(length + address > 256){
		return -1;
	}
	
	if(length % 16 != 0 || address % 16 != 0){
		return -1;
	}
	
	start_page = address/16;
	end_page = (length+address-1)/16;
	start_len = (start_page+1)*16-address;
	end_len = 16-start_len;

//	cmd = (char*) malloc(18*sizeof(char));
//	strncpy(cmd+2, buff, length);

	cmd[0] = (char) CMD_WRITE;
	switch((length-1) / 16){
		case 15: cmd[1] = (char) (start_page+15)*16;
				 send = (end_page == 15 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+224, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case 14: cmd[1] = (char) (start_page+14)*16;
				 send = (end_page == 14 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+208, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case 13: cmd[1] = (char) (start_page+13)*16;
				 send = (end_page == 13 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+192, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case 12: cmd[1] = (char) (start_page+12)*16;
				 send = (end_page == 12 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+176, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case 11: cmd[1] = (char) (start_page+11)*16;
				 send = (end_page == 11 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+160, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case 10: cmd[1] = (char) (start_page+10)*16;
				 send = (end_page == 10 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+144, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  9: cmd[1] = (char) (start_page+9)*16;
				 send = (end_page == 9 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+128, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  8: cmd[1] = (char) (start_page+8)*16;
				 send = (end_page == 8 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+112, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  7: cmd[1] = (char) (start_page+7)*16;
				 send = (end_page == 7 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+96, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  6: cmd[1] = (char) (start_page+6)*16;
				 send = (end_page == 6 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+80, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  5: cmd[1] = (char) (start_page+5)*16;
				 send = (end_page == 5 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+64, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  4: cmd[1] = (char) (start_page+4)*16;
				 send = (end_page == 4 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+48, 16);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  3: cmd[1] = (char) (start_page+3)*16;
				 send = (end_page == 3 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+32, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  2: cmd[1] = (char) (start_page+2)*16;
				 send = (end_page == 2 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len+16, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  1: cmd[1] = (char) (start_page+1)*16;
				 send = (end_page == 1 ? end_len : 16);
				 strncpy(cmd+2, buff+start_len, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
				 while(storage_read_status(STATUS_WIP));
		case  0: cmd[1] = (char) address;
				 send = (start_page == end_page ? length : start_len);
				 strncpy(cmd+2, buff, send);
				 storage_write_enable();
				 SPIO_send(cmd, send + 2);
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
