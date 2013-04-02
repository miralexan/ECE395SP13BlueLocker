#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "spio.h"

#define CMD_READ 0x03
#define CMD_WRITE 0x02
#define CMD_WRDI 0x04
#define CMD_WREN 0x06
#define CMD_RDSR 0x05
#define CMD_WRSR 0x01

#define BASE_ADDR 0x00

int read_storage(char*, char);
int write_storage(char*, int, char);

#endif
