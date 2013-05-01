
#include "password.h"

char password[512];
char password_set;
extern device flash;

void readpass() {
	dread(&flash, &password_set, 1, 0);
	dread(&flash, password, HASH_LENGTH, 1);
}

void setpass(const char* pass_in) {
	hashpass(pass_in, password);
	password_set = 1;
	dwrite(&flash, &password_set, 1, 0);
	dwrite(&flash, password, HASH_LENGTH, 1);
}

int checkpass(const char *string) {
	if (password_set == 0) { return 0; }
	return checkhash(string, password);
}

int passisset() {
	return password_set;
}

void unsetpass() {
	password_set = 0;
	dwrite(&flash, &password_set, 1, 0);
}
