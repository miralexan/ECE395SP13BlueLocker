#ifndef HASHER_H
#define HASHER_H

#include "sha1.h"
#include "string.h"
#include "debug.h"

#if DEBUG
	#include "device.h"
#endif

#define HASH_LENGTH 20

int hashpass(const char*, char*);
int checkhash(const char*, char*);

#endif // HASHER_H\

