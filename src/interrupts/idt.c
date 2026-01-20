#include <interrupts/idt.h>
#include <drivers/devices/serial.h>
#include <drivers/devices/apic.h>
#include <drivers/features/feature.h>

static idtEntry_t idt[256];
static idtPtr_t idtp;
int tick_counter = 0;

void idtTimer(InterruptFrame *frame)
{
        tick_counter++;
        outb(0x20, 0x20);
        SerialPrint("INFO: PIT Fired\r\n");
}

void idtSetEntry(uint8_t n, void *handler, idtEntry_t *idtEntries)
{
        idtEntries[n].base_low = (uint64_t)handler & 0xFFFF;
        idtEntries[n].base_mid = ((uint64_t)handler >> 16) & 0xFFFF;
        idtEntries[n].base_high = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
        idtEntries[n].selector = 8;
        idtEntries[n].zero = 0;
        idtEntries[n].type_attributes = 0x8E;
        idtEntries[n].ist = 0;
}

void idtInit(void)
{
        for (size_t i = 0; i < IDT_ENTRIES; ++i)
        {
                idtSetEntry((uint8_t)i, (void *)hcf, idt);
        }

        idtSetEntry(0x00, (void *)idtTimerPre, idt);
        idtp.limit = (sizeof(idtEntry_t) * IDT_ENTRIES) - 1;
        idtp.base = (uint64_t)idt;
        __asm("lidt (%0)" : : "r"(&idtp));
        SerialPrint("INFO: IDT Loaded\r\n");

        if (IsFeaturePresent(5))
        {
                //APICInit();
        }
}
