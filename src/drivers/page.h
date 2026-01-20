#ifndef PAGE_H
#define PAGE_H

#include <def.h>

#define PAGE_SIZE 0x1000
#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x4
#define PAGE_PWT 0x8
#define PAGE_NO_CACHE 0x10
#define PAGE_XD 0x3F
#define PAGE_PS 0x80

#define APIC_BASE 0xFEE00000

typedef uint64_t ptEntry_t;
typedef ptEntry_t pt_t[512];

void PagingIdentityMap(uint64_t phys_start, uint64_t phys_end);
void PagingInit(void);
void *Alloc(size_t Size);

#endif
