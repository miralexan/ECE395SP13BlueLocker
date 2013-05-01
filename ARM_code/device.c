#include "device.h"

char designations[5][6] = {{"\0\0\0\0\0\0"},
						  {"\0\0\0\0\0\0"},
						  {"\0\0\0\0\0\0"},
						  {"\0\0\0\0\0\0"},
						  {"\0\0\0\0\0\0"}};
func_t* ops[5];

int dadd(const char* name, func_t* operations){
	int i = 0;
	while(1){
		if(i >= 5){
		 	return -1;
		} else if(designations[i][0] == '\0'){
			strcpy(designations[i], name);
			ops[i] = operations;
			break;
		} 

		i++;
	}

	return 0;
}

int dinit(device* device_h, const char* type){
	int i = 0;	

	device_h->flags = 0;

	for(i = 0; i < 5; i++){
		if(strcmp(type, designations[i]) == 0){
			device_h->function = ops[i];
			return 0;
		}
	}

	return -1;
}

int dread(device* device_h, char* buff, const int length, const unsigned char address){
	if(device_h->function->read == 0){
		return -1;
	}

	return device_h->function->read(device_h, buff, length, address);
}

int dwrite(device* device_h, const char* buff, const int length, const unsigned char address){
	if(device_h->function->write == 0){
		return -1;
	}

 	return device_h->function->write(device_h, buff, length, address);
}

int dwrite_string(device* device_h, const char* string, const unsigned char address){
	if(device_h->function->write == 0){
		return -1;
	}

 	return device_h->function->write(device_h, string, strlen(string), address);
}

int dflush(device* device_h){
	return device_h->function->flush();
}

void dclear(device* device_h){
	device_h->flags = 0;
}
