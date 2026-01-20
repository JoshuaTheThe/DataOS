#include <gdt.h>
#include <string.h>

static gdtEntry_t gdt[GDT_SIZE];
static gdtPtr_t gdtp;

void gdtSetEntry(int num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran)
{
        gdt[num].limit_low = limit & 0xFFFF;
        gdt[num].base_low = base & 0xFFFF;
        gdt[num].base_middle = (base >> 16) & 0xFF;
        gdt[num].base_high = (base >> 24) & 0xFF;
        gdt[num].access = access;
        gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
        gdt[num].base_upper = (base >> 32) & 0xFFFFFFFF;
        gdt[num].reserved = 0;
}

void gdtInit(void)
{
        /* not required */
}
