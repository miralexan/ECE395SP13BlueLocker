
#include "hasher.h"

//#define PASS_SALT "This is a song that gets on everybody's nerves / on everybody's nerves / on everybody's nerves / \
//This is a song that gets on everybody's nerves / and this is how it goes..."

void hashpass(const char* pass_in, char* pass_out) {
	SHA1Context pass_hash;
    SHA1Reset(&pass_hash);
//	SHA1Input(&pass_hash, PASS_SALT, strlen(PASS_SALT));
    SHA1Input(&pass_hash, (const unsigned char *) pass_in, strlen(pass_in));
	SHA1Result(&pass_hash);
	memcpy((char*)pass_out, (char*)pass_hash.Message_Digest, HASH_LENGTH);
}

int checkhash(const char *string, char *pass_hash) {
	SHA1Context string_hash;

    SHA1Reset(&string_hash);
//	SHA1Input(&string_hash, PASS_SALT, strlen(PASS_SALT));
    SHA1Input(&string_hash, (const unsigned char *) string, strlen(string));

    if (!SHA1Result(&string_hash)) {
        UART_data_write_string("ERROR-- could not compute message digest\r\n");
		return 0; // false
    }

//	setpass(string, string_hash);

#if DEBUG
	{
		UART_data_write_nstring(pass_hash, HASH_LENGTH);
		UART_data_write_string("\r\n");
		UART_data_write_nstring((char*) string_hash.Message_Digest, HASH_LENGTH);
		UART_data_write_string("\r\n");
	}
#endif

	if (strncmp((char*)string_hash.Message_Digest, pass_hash, HASH_LENGTH) != 0) { 
		return 0; // false
	}

	return 1; // true

}

