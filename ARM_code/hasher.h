#ifndef HASHER_H
#define HASHER_H

#include "sha1.h"
#include "uart.h"
#include "string.h"

#define DEBUG 1

#define HASH_LENGTH 20

void hashpass(char*, char*);
int checkhash(char*, char*);

#endif // HASHER_H\

