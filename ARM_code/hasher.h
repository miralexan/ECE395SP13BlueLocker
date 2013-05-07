/* hasher.h
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#ifndef HASHER_H
#define HASHER_H

#include "sha1.h"
#include "string.h"
#include "uart.h"
#include "debug.h"

#define HASH_LENGTH 20

void hashpass(const char*, char*);
int checkhash(const char*, char*);

#endif // HASHER_H\

