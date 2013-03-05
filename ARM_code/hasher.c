
#include "hasher.h"

#define PASS_SALT "This is a song that gets on everybody's nerves / on everybody's nerves / on everybody's nerves / \
This is a song that gets on everybody's nerves / and this is how it goes..."

void setpass(char* pass_in, char* pass_out) {
	SHA1Context pass_hash;
    SHA1Reset(&pass_hash);
	SHA1Input(&pass_hash, PASS_SALT, strlen(PASS_SALT));
    SHA1Input(&pass_hash, (const unsigned char *) pass_in, strlen(pass_in));
	SHA1Result(&pass_hash);
	memcpy((char*)pass_out, (char*)pass_hash.Message_Digest, HASH_LENGTH);
}

int checkpass(char *pass_hash, char *string, int length) {
	int i;
	char string_hash[HASH_LENGTH];
//	SHA1Context string_hash;
//
//    SHA1Reset(&string_hash);
//	SHA1Input(&string_hash, PASS_SALT, strlen(PASS_SALT));
//    SHA1Input(&string_hash, (const unsigned char *) string, length);
//
//    if (!SHA1Result(&string_hash)) {
//        UART_data_write_string("ERROR-- could not compute message digest\r\n");
//		return 0; // false
//    }

	setpass(string, string_hash);

#if DEBUG
	{
		UART_data_write_nstring(pass_hash, HASH_LENGTH);
		UART_data_write_string("\r\n");
		UART_data_write_nstring(/*(char*) */string_hash/*.Message_Digest*/, HASH_LENGTH);
		UART_data_write_string("\r\n");
	}
#endif

	for(i = 0; i < HASH_LENGTH; i++){ 
		if (string_hash/*.Message_Digest*/[i] != pass_hash[i]) { 
			return 0; // false
		}
	}

	return 1; // true

}

