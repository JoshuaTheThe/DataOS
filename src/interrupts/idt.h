#ifndef IDT_H
#define IDT_H

#include <def.h>
#include <drivers/io.h>

#define IDT_ENTRIES 256

typedef struct __attribute__((packed))
{
        uint16_t base_low;
        uint16_t selector;
        uint8_t ist;
        uint8_t type_attributes;
        uint16_t base_mid;
        uint32_t base_high;
        uint32_t zero;
} idtEntry_t;

typedef struct __attribute__((__packed__))
{
        uint16_t limit;
        uint64_t base;
} idtPtr_t;

void idtInit(void);
void idtDefault(int code, int eip, int cs);
void idtSetEntry(uint8_t n, void *handler, idtEntry_t *idt);

void idtTimerPre(void);

typedef struct InterruptFrame
{
        uint64_t rip;
        uint64_t cs;
        uint64_t flags;
        uint64_t rsp;
        uint64_t ss;
} InterruptFrame;

#endif
