#include <drivers/devices/apic.h>
#include <drivers/devices/serial.h>
#include <drivers/features/feature.h>

void APICSetBase(uint64_t Apic)
{
        uint32_t eax = (uint32_t)(Apic & 0xffffff000) | IA32_APIC_BASE_MSR_ENABLE, edx = 0;
#ifdef __PHYSICAL_MEMORY_EXTENSION__
        edx = (Apic >> 32) & 0x0f;
#endif
        MSRSet(IA32_APIC_BASE_MSR, eax, edx);
}

uint64_t APICGetBase(void)
{
        uint32_t eax, edx;
        MSRGet(IA32_APIC_BASE_MSR, &eax, &edx);
#ifdef __PHYSICAL_MEMORY_EXTENSION__
        return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
#else
        return (eax & 0xfffff000);
#endif
}

uint32_t APICRead(uint32_t reg)
{
        volatile uint32_t *Apic = (volatile uint32_t *)APICGetBase();
        return Apic[reg / 4];
}

void APICWrite(uint32_t reg, uint32_t value)
{
        volatile uint32_t *Apic = (volatile uint32_t *)APICGetBase();
        Apic[reg / 4] = value;
}

void APICInit(void)
{
        // APICSetBase(0xFEE00000 | 0x800);
        SerialPrint("INFO: APIC Initialised\r\n");
}
