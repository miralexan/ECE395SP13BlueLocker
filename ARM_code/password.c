/* password.c
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#include "password.h"

char password[512];
char password_set;
extern device flash;

/* readpass
 * Inputs: none
 * Outputs: none
 * Side-Effects:
 *   Loads the current password state and hash from storage.
 */
void readpass() {
	dread(&flash, &password_set, 1, 0);
	dread(&flash, password, HASH_LENGTH, 1);
}

/* setpass
 * Inputs:
 *   pass_in - A C-string containing the user password
 * Outputs: none
 * Side-Effects:
 *   Generates a hash from the password and writes it to storage.
 */
void setpass(const char* pass_in) {
	hashpass(pass_in, password);
	password_set = 1;
	dwrite(&flash, &password_set, 1, 0);
	dwrite(&flash, password, HASH_LENGTH, 1);
}

/* checkpass
 * Inputs: 
 *   string - A C-string containing the user password
 * Outputs:
 *   Returns 1 if the supplied password generates a match to the
 *	 stored hash, 0 if not.
 * Side-Effects: None
 */
int checkpass(const char *string) {
	if (password_set == 0) { return 0; }
	return checkhash(string, password);
}

/* passisset
 * Inputs: none
 * Outputs:
 *   Returns 1 if a password has been set, 0 if not
 * Side-Effects: none
 */
int passisset() {
	return password_set;
}

/* unsetpass
 * Inputs: none
 * Outputs: none
 * Side-Effects:
 *   Unsets the current password.
 */
void unsetpass() {
	password_set = 0;
	dwrite(&flash, &password_set, 1, 0);
}
