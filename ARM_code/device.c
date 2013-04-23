#include "device.h"

int dinit(device* device_h, const int type){
	device_h->flags = 0;

	switch(type){
	 	case UART:  device_h->read = &UART_recv;
				    device_h->write = &UART_send;
				    device_h->flush = &UART_flush;
				    break;
		case SPIO:  device_h->read = &SPIO_recv;
				    device_h->write = &SPIO_send;
				    device_h->flush = &SPIO_flush;
				    break;
		case FLASH: device_h->read = &read_storage;
				    device_h->write = &write_storage;
				    device_h->flush = &SPIO_flush;
				    break;
		default:	return -1;
	}
	return 0;
}

int dread(device* device_h, char* buff, const int length, const unsigned char address){
	if(device_h->read == 0){
		return -1;
	}

	return device_h->read(device_h, buff, length, address);
}

int dwrite(device* device_h, const char* buff, const int length, const unsigned char address){
	if(device_h->write == 0){
		return -1;
	}

 	return device_h->write(device_h, buff, length, address);
}

int dwrite_string(device* device_h, const char* string, const unsigned char address){
	if(device_h->write == 0){
		return -1;
	}

 	return device_h->write(device_h, string, strlen(string), address);
}

int dflush(device* device_h){
	return device_h->flush();
}

void dclear(device* device_h){
	device_h->flags = 0;
}
