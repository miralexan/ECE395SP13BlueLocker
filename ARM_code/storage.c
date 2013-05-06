/* storage.c
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#include "storage.h"

#if DEBUG
{
	extern device uart;
}
static func_t functions;

void storage_init(){
	functions.read = &read_storage;
	functions.write = &write_storage;
	functions.flush = &SPIO_flush;
	dadd("FLASH", &functions);
}

/* read_storage
 * Inputs:
 *   device_h - A pointer to a device structure
 *   buff - buffer to receive input
 *   length - number of bytes to read
 *   address - the address of the flash device to begin reading from
 * Outputs:
 *   Returns the number of bytes read
 * Side-effects: None
 * Notes:
 *   The flash interface for reading is [READ_CMD ADDRESS byte0...byteN]
 *   Because of the way SPI works, you must write as many bytes as you
 *   expect to read in order to keep the clock running. All bytes after
 *   ADDRESS are ignored by the flash chip. Similarly, the first two bytes
 *   returned are garbage.
 */
int read_storage(device* device_h, char* buff, const int length, const unsigned char address){
	char cmd[258];

	memset(cmd, 0, 258);

	cmd[0] = (char) CMD_READ;
	cmd[1] = (char) address;

	SPIO_send(device_h, cmd, length+2, 0);
	SPIO_recv(device_h, cmd,2, 0);

#if DEBUG 
{
	dwrite_string(&uart, "Now trying to receive...\r\n", 0);
}
#endif

	return SPIO_recv(device_h, buff, length, 0);
}

/* write_storage
 * Inputs:
 *   device_h - A pointer to a device structure
 *   buff - buffer of data to write to flash storage
 *   length - number of bytes to write
 *   address - the address of the flash device to begin writing to
 * Outputs:
 *   Returns the number of bytes written, or -1 on error
 * Side-effects:
 *   Stores data in the attached flash storage chip. Clobbers the SPI buffer.
 * Notes:
 *   The flash interface for writing is [WRITE_CMD ADDRESS DATA0...DATAn].
 *   All data returned by the SPI interface during a write is garbage.
 *   The flash storage chip is divided into 16 16-byte pages. Writes cannot
 *   cross a page boundary, and the chip becomes inaccessible (except for the
 *   status register) while it commits its internal receive buffer to non-volitile 
 *   storage. Therefore, when writing one must A) identify where page boundaries occur
 *   based on starting address and write length; and B) poll the device to determine
 *   when a write-cycle is finished before continuing.
 */
int write_storage(device* device_h, const char* buff, const int length, const unsigned char address){
	char cmd[18];
	int start_page,  // The page a write begins in
	    end_page,    // The page a write ends in
		start_len,   // The number of bytes to be written to the start_page
		end_len,     // The number of bytes to be written to the end_page
		send;        // number of bytes to be written on any given page
	
	start_page = end_page = start_len = end_len = send = 0;

#if DEBUG 
{	
	dwrite_string(&uart, "First character to write is \"", 0);
	dwrite(&uart, buff[0], 1, 0);
	dwrite_string(&uart, "\"", 0);
}
#endif

	if(length <= 0 || length > 256){
		return -1;
	} else if(length + address > 256){
		return -1;
	}
	
	start_page = address/16;
	end_page = (length+address-1)/16;
	start_len = (start_page+1)*16-address;
	end_len = (address + length) - (end_page * 16);

	// Unrolled write loop for speed.
	// Beginning and ending pages are special cases with regards
	// to the number of bytes written; all intervening pages will
	// have a full 16 bytes written.
	// Pages are written last to first.
	// Each stage calculates the address to begin writing to, enables writing,
	// sends the data, flushes the garbage-filled SPI buffer, and waits
	// for the write cycle to finish.
	cmd[0] = (char) CMD_WRITE;
	send = end_len;
	switch(end_page - start_page){
		case 15: cmd[1] = (char) (start_page+15)*16;
				 memcpy(cmd+2, buff+start_len+224, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case 14: cmd[1] = (char) (start_page+14)*16;
				 memcpy(cmd+2, buff+start_len+208, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case 13: cmd[1] = (char) (start_page+13)*16;
				 memcpy(cmd+2, buff+start_len+192, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case 12: cmd[1] = (char) (start_page+12)*16;
				 memcpy(cmd+2, buff+start_len+176, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case 11: cmd[1] = (char) (start_page+11)*16;
				 memcpy(cmd+2, buff+start_len+160, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case 10: cmd[1] = (char) (start_page+10)*16;
				 memcpy(cmd+2, buff+start_len+144, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  9: cmd[1] = (char) (start_page+9)*16;
				 memcpy(cmd+2, buff+start_len+128, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  8: cmd[1] = (char) (start_page+8)*16;
				 memcpy(cmd+2, buff+start_len+112, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  7: cmd[1] = (char) (start_page+7)*16;
				 memcpy(cmd+2, buff+start_len+96, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  6: cmd[1] = (char) (start_page+6)*16;
				 memcpy(cmd+2, buff+start_len+80, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  5: cmd[1] = (char) (start_page+5)*16;
				 memcpy(cmd+2, buff+start_len+64, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  4: cmd[1] = (char) (start_page+4)*16;
				 memcpy(cmd+2, buff+start_len+48, 16);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  3: cmd[1] = (char) (start_page+3)*16;
				 memcpy(cmd+2, buff+start_len+32, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  2: cmd[1] = (char) (start_page+2)*16;
				 memcpy(cmd+2, buff+start_len+16, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 send = 16;
				 while(storage_read_status(STATUS_WIP));
		case  1: cmd[1] = (char) (start_page+1)*16;
				 memcpy(cmd+2, buff+start_len, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 while(storage_read_status(STATUS_WIP));
		case  0: cmd[1] = (char) address;
				 send = (start_page == end_page ? length : start_len);
				 memcpy(cmd+2, buff, send);
				 storage_write_enable();
				 SPIO_send(device_h, cmd, send + 2, 0);
				 SPIO_flush();
				 while(storage_read_status(STATUS_WIP));
	}

	return 0;
}

/* storage_read_status
 * Inputs:
 *   mask - bitmask controlling what status bits are checked
 * Outputs:
 *   Returns 0 if selected bit(s) are zero, non-zero if one or more
 *   bits are set.
 * Side-effects: None
 */
int storage_read_status(char mask){
	char cmd[2];
	
	cmd[0] = CMD_RDSR;

	SPIO_send(NULL, cmd, 2, 0);
	SPIO_recv(NULL, cmd, 1, 0);
	SPIO_recv(NULL, cmd, 1, 0);

	return (int) cmd[0] & mask;
}

/* storage_write_enable
 * Inputs: None
 * Outputs:
 *   Returns 0 on success, -1 on failure
 * Side-effects: 
 *   Enables writing to flash chip
 */
int storage_write_enable(void){

	char cmd = CMD_WREN;

	SPIO_send(NULL, &cmd, 1, 0);
	SPIO_flush();
	
	// TODO: ensure write enable latch is set
	return 0;
}
