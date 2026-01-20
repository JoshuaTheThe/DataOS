#ifndef GDT_H
#define GDT_H

#include <def.h>

#define GDT_SIZE 3

typedef struct __attribute__((packed))
{
        uint16_t limit_low;  // bits 0-15 of segment limit
        uint16_t base_low;   // bits 0-15 of base
        uint8_t base_middle; // bits 16-23 of base
        uint8_t access;      // access flags
        uint8_t granularity; // granularity + bits 16-19 of limit
        uint8_t base_high;   // bits 24-31 of base
        uint32_t base_upper; // bits 32-63 of base
        uint32_t reserved;   // must be 0
} gdtEntry_t;

typedef struct __attribute__((packed))
{
        uint16_t limit;
        uint32_t base;
} gdtPtr_t;

typedef struct __attribute__((packed))
{
        uint32_t prev_tss;
        uint32_t esp0;
        uint32_t ss0;
        uint32_t esp1;
        uint32_t ss1;
        uint32_t esp2;
        uint32_t ss2;
        uint32_t cr3;
        uint32_t eip;
        uint32_t eflags;
        uint32_t eax;
        uint32_t ecx;
        uint32_t edx;
        uint32_t ebx;
        uint32_t esp;
        uint32_t ebp;
        uint32_t esi;
        uint32_t edi;
        uint32_t es;
        uint32_t cs;
        uint32_t ss;
        uint32_t ds;
        uint32_t fs;
        uint32_t gs;
        uint32_t ldt;
        uint16_t trap;
        uint16_t iomap_base;
} gdtTssEntry_t;

void gdtInit(void);
void gdtSetEntry(int num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);

#endif
