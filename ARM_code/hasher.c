#include "hasher.h"

#if DEBUG
	extern device uart;
#endif

int hashpass(const char* pass_in, char* pass_out) {
	SHA1Context pass_hash;
    SHA1Reset(&pass_hash);
    SHA1Input(&pass_hash, (const unsigned char *) pass_in, strlen(pass_in));
	
	if (!SHA1Result(&string_hash)) {
#if DEBUG
        dwrite_string(&uart, "ERROR-- could not compute message digest\r\n", 0);
#endif
		return -1;
    }
	
	memcpy((char*)pass_out, (char*)pass_hash.Message_Digest, HASH_LENGTH);
	
	return 0;
}

int checkhash(const char *string, char *pass_hash) {
	char string_hash[HASH_LENGTH];
	
	if(hashpass(string, string_hash) == -1){
		return 0;
	}

#if DEBUG
	{
		dwrite(&uart, pass_hash, HASH_LENGTH, 0);
		dwrite_string(&uart, "\r\n", 0);
		dwrite(&uart, string_hash, HASH_LENGTH, 0);
		dwrite_string(&uart, "\r\n", 0);
	}
#endif

	if (strncmp(string_hash, pass_hash, HASH_LENGTH) != 0) { 
		return 0; // false
	}

	return 1; // true

}

