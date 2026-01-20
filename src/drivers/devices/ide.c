#include <drivers/devices/ide.h>

static IDEDriver_t IDEState;

void IDEWrite(unsigned char channel, unsigned char reg, unsigned char data)
{
        if (reg > 0x07 && reg < 0x0C)
                IDEWrite(channel, ATA_REG_CONTROL, 0x80 | IDEState.Channels[channel].nIEN);
        if (reg < 0x08)
                outb(IDEState.Channels[channel].base + reg - 0x00, data);
        else if (reg < 0x0C)
                outb(IDEState.Channels[channel].base + reg - 0x06, data);
        else if (reg < 0x0E)
                outb(IDEState.Channels[channel].ctrl + reg - 0x0A, data);
        else if (reg < 0x16)
                outb(IDEState.Channels[channel].bmide + reg - 0x0E, data);
        if (reg > 0x07 && reg < 0x0C)
                IDEWrite(channel, ATA_REG_CONTROL, IDEState.Channels[channel].nIEN);
}

unsigned char IDERead(unsigned char channel, unsigned char reg)
{
        unsigned char result;
        if (reg > 0x07 && reg < 0x0C)
                IDEWrite(channel, ATA_REG_CONTROL, 0x80 | IDEState.Channels[channel].nIEN);
        if (reg < 0x08)
                result = inb(IDEState.Channels[channel].base + reg - 0x00);
        else if (reg < 0x0C)
                result = inb(IDEState.Channels[channel].base + reg - 0x06);
        else if (reg < 0x0E)
                result = inb(IDEState.Channels[channel].ctrl + reg - 0x0A);
        else if (reg < 0x16)
                result = inb(IDEState.Channels[channel].bmide + reg - 0x0E);
        if (reg > 0x07 && reg < 0x0C)
                IDEWrite(channel, ATA_REG_CONTROL, IDEState.Channels[channel].nIEN);
        return result;
}

void IDEReadBuffer(uint8_t channel, uint8_t reg, uint64_t buffer, uint64_t quads)
{
        if (reg > 0x07 && reg < 0x0C)
                IDEWrite(channel, ATA_REG_CONTROL, 0x80 | IDEState.Channels[channel].nIEN);
        if (reg < 0x08)
                insl(IDEState.Channels[channel].base + reg - 0x00, (void*)buffer, quads);
        else if (reg < 0x0C)
                insl(IDEState.Channels[channel].base + reg - 0x06, (void*)buffer, quads);
        else if (reg < 0x0E)
                insl(IDEState.Channels[channel].ctrl + reg - 0x0A, (void*)buffer, quads);
        else if (reg < 0x16)
                insl(IDEState.Channels[channel].bmide + reg - 0x0E, (void*)buffer, quads);
        if (reg > 0x07 && reg < 0x0C)
                IDEWrite(channel, ATA_REG_CONTROL, IDEState.Channels[channel].nIEN);
}

void IDEInitialise(void)
{
        if (!IDEState.Dev)
                return;
        int i, j, k, count = 0;
        IDEState.Channels[ATA_PRIMARY].base = (IDEState.Dev->bar[0] & 0xFFFFFFFC) + 0x1F0 * (!IDEState.Dev->bar[0]);
        IDEState.Channels[ATA_PRIMARY].ctrl = (IDEState.Dev->bar[1] & 0xFFFFFFFC) + 0x3F6 * (!IDEState.Dev->bar[1]);
        IDEState.Channels[ATA_SECONDARY].base = (IDEState.Dev->bar[2] & 0xFFFFFFFC) + 0x170 * (!IDEState.Dev->bar[2]);
        IDEState.Channels[ATA_SECONDARY].ctrl = (IDEState.Dev->bar[3] & 0xFFFFFFFC) + 0x376 * (!IDEState.Dev->bar[3]);
        IDEState.Channels[ATA_PRIMARY].bmide = (IDEState.Dev->bar[4] & 0xFFFFFFFC) + 0;   // Bus Master IDE
        IDEState.Channels[ATA_SECONDARY].bmide = (IDEState.Dev->bar[4] & 0xFFFFFFFC) + 8; // Bus Master IDE
        SerialPrint("INFO: IDE Channels Set\r\n");
        IDEWrite(ATA_PRIMARY, ATA_REG_CONTROL, 0x02);
        IDEWrite(ATA_SECONDARY, ATA_REG_CONTROL, 0x02);
        SerialPrint("INFO: IDE IRQs Disabled\r\n");
        for (i = 0; i < 2; i++)
                for (j = 0; j < 2; j++)
                {

                        unsigned char err = 0, type = IDE_ATA, status;
                        IDEState.IDEDev[count].Reserved = 0;

                        IDEWrite(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
                        PITDelay(1);
                        IDEWrite(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
                        PITDelay(1);
                        if (IDERead(i, ATA_REG_STATUS) == 0)
                                continue;

                        while (1)
                        {
                                status = IDERead(i, ATA_REG_STATUS);
                                if ((status & ATA_SR_ERR))
                                {
                                        err = 1;
                                        break;
                                }
                                if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ))
                                        break;
                        }

                        if (err != 0)
                        {
                                unsigned char cl = IDERead(i, ATA_REG_LBA1);
                                unsigned char ch = IDERead(i, ATA_REG_LBA2);

                                if (cl == 0x14 && ch == 0xEB)
                                        type = IDE_ATAPI;
                                else if (cl == 0x69 && ch == 0x96)
                                        type = IDE_ATAPI;
                                else
                                        continue;

                                IDEWrite(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
                                PITDelay(1);
                        }

                        IDEReadBuffer(i, ATA_REG_DATA, (uint64_t)IDEState.Buff, 128);

                        IDEState.IDEDev[count].Reserved = 1;
                        IDEState.IDEDev[count].Type = type;
                        IDEState.IDEDev[count].Channel = i;
                        IDEState.IDEDev[count].Drive = j;
                        IDEState.IDEDev[count].Signature = *((unsigned short *)(IDEState.Buff + ATA_IDENT_DEVICETYPE));
                        IDEState.IDEDev[count].Capabilities = *((unsigned short *)(IDEState.Buff + ATA_IDENT_CAPABILITIES));
                        IDEState.IDEDev[count].CommandSets = *((unsigned int *)(IDEState.Buff + ATA_IDENT_COMMANDSETS));

                        if (IDEState.IDEDev[count].CommandSets & (1 << 26))
                                IDEState.IDEDev[count].Size = *((unsigned int *)(IDEState.Buff + ATA_IDENT_MAX_LBA_EXT));
                        else
                                IDEState.IDEDev[count].Size = *((unsigned int *)(IDEState.Buff + ATA_IDENT_MAX_LBA));

                        for (k = 0; k < 40; k += 2)
                        {
                                IDEState.IDEDev[count].Model[k] = IDEState.Buff[ATA_IDENT_MODEL + k + 1];
                                IDEState.IDEDev[count].Model[k + 1] = IDEState.Buff[ATA_IDENT_MODEL + k];
                        }
                        IDEState.IDEDev[count].Model[40] = 0;

                        count++;
                }
}

void IDEFind(void)
{
        IDEState.Dev = pciFindOfType(0x01, 0x01);
        if (!IDEState.Dev)
        {
                SerialPrint("ERROR: Could not find IDE Device\r\n");
                return;
        }

        SerialPrint("INFO: IDE Device found\r\n");
        IDEInitialise();
}
