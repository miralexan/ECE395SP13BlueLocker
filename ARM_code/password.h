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

