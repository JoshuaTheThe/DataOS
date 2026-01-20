#include <drivers/devices/acpi.h>
#include <drivers/devices/serial.h>
#include <stdint.h>
#include <string.h>

#define RSDP_SIGNATURE "RSD PTR "

RSDP_t *RSDPFind(void)
{
        uint8_t *addr;
        for (addr = (uint8_t *)0xE0000; addr < (uint8_t *)0xFFFFF; addr += 16)
        {
                if (memcmp(addr, RSDP_SIGNATURE, 8) == 0)
                {
                        return (RSDP_t *)addr;
                }
        }
        return 0;
}

XSDT_t *XSDTGet(XSDP_t *xsdp)
{
        return (XSDT_t *)(uint64_t)xsdp->XsdtAddress;
}

int ACPIValidate(uint8_t *table, size_t length)
{
        uint8_t sum = 0;
        for (size_t i = 0; i < length; i++)
                sum += table[i];
        return sum == 0;
}

void XSDTParse(XSDT_t *xsdt)
{
        int entries = (xsdt->header.Length - sizeof(ACPI_SDTHeader)) / sizeof(uint64_t);

        for (int i = 0; i < entries; i++)
        {
                ACPI_SDTHeader *table = (ACPI_SDTHeader *)(uint64_t)xsdt->entries[i];
                if (!ACPIValidate((uint8_t *)table, table->Length))
                {
                        SerialPrint("ERROR: Table is invalid\r\n");
                        continue;
                }
        }
}

XSDT_t *ACPIInit(void)
{
        RSDP_t *rsdp = RSDPFind();
        if (!rsdp)
        {
                SerialPrint("ERROR: RSDP not found!\r\n");
                return NULL;
        }

        if (!ACPIValidate((uint8_t *)rsdp, sizeof(RSDP_t)))
        {
                SerialPrint("ERROR: RSDP checksum invalid!\r\n");
                return NULL;
        }

        SerialPrint("INFO: RSDP found\r\n");

        XSDT_t *xsdt = XSDTGet((XSDP_t *)rsdp);
        if (!xsdt)
        {
                SerialPrint("ERROR: XSDT not found!\r\n");
                return NULL;
        }

        if (!ACPIValidate((uint8_t *)xsdt, xsdt->header.Length))
        {
                SerialPrint("ERROR: XSDT checksum invalid!\r\n");
                return NULL;
        }

        XSDTParse(xsdt);
        SerialPrint("INFO: ACPI Initialised\r\n");
        return xsdt;
}
