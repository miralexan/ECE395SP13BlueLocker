/* hasher.c
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#include "hasher.h"

extern device uart;

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
        dwrite_string(&uart, "ERROR-- could not compute message digest\r\n", 0);
		return 0; // false
    }

//	setpass(string, string_hash);

#if DEBUG
	{
		dwrite(&uart, pass_hash, HASH_LENGTH, 0);
		dwrite_string(&uart, "\r\n", 0);
		dwrite(&uart, (char*) string_hash.Message_Digest, HASH_LENGTH, 0);
		dwrite_string(&uart, "\r\n", 0);
	}
#endif

	if (strncmp((char*)string_hash.Message_Digest, pass_hash, HASH_LENGTH) != 0) { 
		return 0; // false
	}

	return 1; // true

}

