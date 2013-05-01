#ifndef HASHER_H
#define HASHER_H

#include "sha1.h"
#include "string.h"
#include "uart.h"

#ifdef __DEBUG_H__
	#include "debug.h"
#endif

#define HASH_LENGTH 20

void hashpass(const char*, char*);
int checkhash(const char*, char*);

#endif // HASHER_H\

