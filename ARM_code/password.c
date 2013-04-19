
#include "password.h"

char password[512];
//int password_length;
char password_set;

void readpass() {
	read_storage(&password_set, 1, 0);
	if (password_set == 1) {
		password_set = 1;
	} else {
		password_set = 0;
	}
	read_storage(password, HASH_LENGTH, 1);
}

void setpass(const char* pass_in) {
	hashpass(pass_in, password);
	password_set = 1;
	write_storage(&password_set, 1, 0);
	write_storage(password, HASH_LENGTH, 1);
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
	write_storage(&password_set, 1, 0);
}
