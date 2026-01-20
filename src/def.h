#pragma once
#ifndef DEF_H

#define DEF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define WIDTH 1024
#define HEIGHT 768

#define cli() __asm("cli")
#define sti() __asm("sti")

void hcf(void);

#pragma pack(1)

#endif