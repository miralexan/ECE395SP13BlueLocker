/* password.h
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#ifndef PASSWORD_H
#define PASSWORD_H

#include "hasher.h"
#include "string.h"
#include "device.h"

void readpass(void);
void setpass(const char*);
int checkpass(const char*);
int passisset(void);
void unsetpass(void);

#endif // PASSWORD_H\

