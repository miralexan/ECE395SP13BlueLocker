#ifndef PASSWORD_H
#define PASSWORD_H

#include "hasher.h"
#include "uart.h"
#include "string.h"
#include "storage.h"

#define DEBUG 1

void readpass(void);
void setpass(char*);
int checkpass(char*);
int passisset(void);
void unsetpass(void);

#endif // PASSWORD_H\

