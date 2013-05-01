#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "spio.h"
#include "device.h"
#include "string.h"

#ifdef __DEBUG_H__
	#include "debug.h"
	#include "uart.h"
#endif

typedef struct device_t device;

#define CMD_READ 0x03
#define CMD_WRITE 0x02
#define CMD_WRDI 0x04
#define CMD_WREN 0x06
#define CMD_RDSR 0x05
#define CMD_WRSR 0x01

#define STATUS_WIP 0x01
#define STATUS_WEL 0x02
#define STATUS_BP0 0x04
#define STATUS_BP1 0x08

void storage_init(void);
int read_storage(device*, char*, const int, const unsigned char);
int write_storage(device*, const char*, const int, const unsigned char);
int storage_write_enable(void);
int storage_read_status(char);


#endif
