/* device.c
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#include "device.h"

char designations[5][6] = {{"\0\0\0\0\0\0"},
						  {"\0\0\0\0\0\0"},
						  {"\0\0\0\0\0\0"},
						  {"\0\0\0\0\0\0"},
						  {"\0\0\0\0\0\0"}};
func_t* ops[5];

/* dadd
 * Inputs:
 *   name - The name of the registering device
 *   operations - a pointer to the device's func_t
 *   structure.
 * Outputs:
 *   0 on success, -1 on failure
 * Side-Effects:
 *   Registers device name to be an option when initializing
 *   device structures
 * Note:
 *   This registration process was created to side-step a circular include
 *   error. There's probably a better way to handle this.
 */
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

/* dinit
 * Inputs:
 *   device_h - a pointer to the device to be initialized
 *   type - the name of the peripheral to initialize this device to.
 * Outputs:
 *   0 on success, -1 on failure
 * Side-Effects:
 *   Clears device_h's flags member and sets the internal func_t pointer
 *   to the appropriate peripheral's.
 */
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

/* dread
 * Inputs:
 *   device_h - A pointer to a device structure
 *   buff - buffer to receive input
 *   length - number of bytes to read
 *   address - the address to begin reading from
 * Outputs:
 *   Returns the number of bytes read, or -1 on failure
 * Side-effects: 
 *   None, should set flags in device_h to reflect status
 */
int dread(device* device_h, char* buff, const int length, const unsigned char address){
	if(device_h->function->read == 0){
		return -1;
	}

	return device_h->function->read(device_h, buff, length, address);
}

/* dwrite
 * Inputs:
 *   device_h - A pointer to a device structure
 *   buff - buffer of data to write/send
 *   length - number of bytes to write
 *   address - the address to begin writing to
 * Outputs:
 *   Returns the number of bytes written, or -1 on failure
 * Side-effects: 
 *   None, should set flags in device_h to reflect status
 */
int dwrite(device* device_h, const char* buff, const int length, const unsigned char address){
	if(device_h->function->write == 0){
		return -1;
	}

 	return device_h->function->write(device_h, buff, length, address);
}

/* dwrite_string
 * Inputs:
 *   device_h - A pointer to a device structure
 *   string - A C_string (null-terminated character array)
 *   address - the address to begin writing to
 * Outputs:
 *   Returns the number of bytes written, or -1 on failure
 * Side-effects: 
 *   None, should set flags in device_h to reflect status
 */
int dwrite_string(device* device_h, const char* string, const unsigned char address){
	if(device_h->function->write == 0){
		return -1;
	}

 	return device_h->function->write(device_h, string, strlen(string), address);
}

/* dflush
 * Inputs:
 *   device_h - A pointer to a device structure
 * Outputs:
 *   Returns 0 on success, or -1 on failure
 * Side-effects: 
 *   None, should set flags in device_h to reflect status
 */
int dflush(device* device_h){
	if(device_h->function->flush == 0){
		return -1;
	}
	return device_h->function->flush();
}

/* dflush
 * Inputs:
 *   device_h - A pointer to a device structure
 * Outputs: None
 * Side-effects: 
 *   Clears flags member of device_h
 */
void dclear(device* device_h){
	device_h->flags = 0;
}
