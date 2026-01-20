#ifndef SERIAL_H
#define SERIAL_H

#include<def.h>
#include<drivers/io.h>

void SerialInit(void);
void SerialPut(char c);
void SerialPrint(const char *const str);
char SerialRead(void);
bool SerialCanRead(void);

#endif
