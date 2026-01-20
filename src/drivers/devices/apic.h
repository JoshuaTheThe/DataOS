#ifndef APIC_H
#define APIC_H

#include <def.h>
#include <drivers/io.h>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800

#define APIC_LVT_TIMER 0x320
#define APIC_LVT_LINT0 0x350

#define APIC_TIMER_ONE_SHOT 0x0
#define APIC_TIMER_PERIODIC 0x20000
#define APIC_TIMER_TSC_DEADLINE 0x40000

#define APIC_DELIVERY_FIXED 0x0

#define APIC_TIMER_DIVIDE_CONFIG 0x3E0
#define APIC_TIMER_INITIAL_COUNT 0x380

void APICSetBase(uint64_t Apic);
uint64_t APICGetBase(void);
uint32_t APICRead(uint32_t reg);
void APICWrite(uint32_t reg, uint32_t value);
void APICInit(void);

#endif
