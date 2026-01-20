#include <drivers/devices/pit.h>
#include <drivers/devices/serial.h>

void PITInit(uint32_t targetFreq)
{
        uint32_t divisor = 1193180 / targetFreq;
        outb(PIT_COMMAND, 0x34);
        outb(PIT_CHANNEL0, divisor & 0xFF);
        outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
        SerialPrint("INFO: PIT Initialised\r\n");
}

void PITDelay(uint32_t ticks)
{
        uint32_t start = tick_counter;
        while ((tick_counter - start) < ticks)
        {
                asm("hlt");
        }
}
