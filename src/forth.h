#ifndef FORTH_H

#include <def.h>
#include <drivers/devices/serial.h>

#define STACK_SIZE 256

void push(int64_t val);
int64_t pop(void);
void forth(char *);

#endif
