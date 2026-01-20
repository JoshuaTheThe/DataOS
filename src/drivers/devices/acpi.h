#ifndef ACPI_H
#define ACPI_H

#include <def.h>
#include <drivers/io.h>

typedef struct RSDP_t
{
        char Signature[8];
        uint8_t Checksum;
        char OEMID[6];
        uint8_t Revision;
        uint32_t RsdtAddress;
} RSDP_t;

typedef struct XSDP_t
{
        char Signature[8];
        uint8_t Checksum;
        char OEMID[6];
        uint8_t Revision;
        uint32_t RsdtAddress; // deprecated since version 2.0

        uint32_t Length;
        uint64_t XsdtAddress;
        uint8_t ExtendedChecksum;
        uint8_t reserved[3];
} XSDP_t;

typedef struct ACPI_SDTHeader
{
        char Signature[4];
        uint32_t Length;
        uint8_t Revision;
        uint8_t Checksum;
        char OEMID[6];
        char OEMTableID[8];
        uint32_t OEMRevision;
        uint32_t CreatorID;
        uint32_t CreatorRevision;
} ACPI_SDTHeader;

typedef struct XSDT_t
{
        ACPI_SDTHeader header;
        uint64_t entries[];
} XSDT_t;

XSDT_t *ACPIInit(void);

#endif
