#ifndef PASSWORD_H
#define PASSWORD_H

#include "hasher.h"
#include "uart.h"
#include "string.h"
#include "storage.h"

#define DEBUG 1

void readpass(void);
void setpass(const char*);
int checkpass(const char*);
int passisset(void);
void unsetpass(void);

#endif // PASSWORD_H\

