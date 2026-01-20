#include <limine.h>
#include "def.h"
#include <string.h>
#include <gdt.h>
#include <drivers/io.h>
#include <drivers/page.h>
#include <drivers/devices/serial.h>
#include <drivers/gdi/gdi.h>
#include <drivers/gdi/text.h>
#include <drivers/features/sse.h>
#include <drivers/features/feature.h>
#include <drivers/devices/apic.h>
#include <drivers/devices/pci.h>
#include <drivers/devices/ide.h>
#include <drivers/devices/rtc.h>
#include <drivers/devices/pit.h>
#include <drivers/devices/acpi.h>
#include <interrupts/idt.h>
#include <forth.h>

void hcf(void)
{
        cli();
        SerialPrint("PANIC: Unspecified Kernel Panic\r\n");
        for (;;)
        {
                asm("hlt");
        }
}

__attribute__((used, section(".requests"))) static volatile LIMINE_BASE_REVISION(2);

__attribute__((used, section(".requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

__attribute__((used, section(".requests_start_marker"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker"))) static volatile LIMINE_REQUESTS_END_MARKER;

void kmain(void)
{
        cli();
        SerialInit();
        if (LIMINE_BASE_REVISION_SUPPORTED == false)
        {
                SerialPrint("PANIC: Limine Unsupported version\r\n");
                hcf();
        }

        if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
        {
                SerialPrint("PANIC: framebuffer_request.response is NULL or the count is less than 1\r\n");
                hcf();
        }

        // if (framebuffer_request.response->framebuffer_count < 2)
        // {
        //         SerialPrint("PANIC: we require at least two frame buffers\r\n");
        //         char str[128];
        //         itos(framebuffer_request.response->framebuffer_count, str, 16);
        //         SerialPrint(str);
        //
        //         hcf();
        // }

        /* Driver initialisation */
        cli();
        FeaturesInit();
        PagingInit();
        idtInit();
        // PITInit(1000);
        // ACPIInit();

        // pciEnumerateDevices(pciRegister);
        // IDEFind();

        struct limine_framebuffer *front = framebuffer_request.response->framebuffers[0];
        // struct limine_framebuffer *back = framebuffer_request.response->framebuffers[1];

        // if (front->width != back->width || front->height != back->height || front->bpp != back->bpp)
        // {
        //         SerialPrint("PANIC: back and front dimensions differ\r\n");
        //         hcf();
        // }

        DISPLAY Display0;
        Display0.BPP = front->bpp;
        Display0.W = front->width;
        Display0.H = front->height;
        Display0.Framebuffer = front->address;
        Display0.Front = front->address;
        Display0.DepthBuffer = /* Gotta Allocate somehow, otherwise we ignore Z */ NULL;

        Display0.FOV = 1;

        int rw = 128;
        int rh = 128;
        RECT Rect;
        SURFACE Surface;

        Surface.W = rw;
        Surface.H = rh;
        Surface.BPP = Display0.BPP;
        Surface.FOV = Display0.FOV;

        uint32_t SurfaceBuff[rw * rh];
        memset(SurfaceBuff, 0, sizeof(SurfaceBuff));
        Surface.Buffer = SurfaceBuff;
        Surface.DepthBuffer = NULL; /* Ignored on null */
        Surface.X = 0;
        Surface.Y = 0;
        Surface.Z = 1.0f;

        Rect.Points[0] = (POINT){.X = 0,
                                 .Y = 0,
                                 .Z = 1,
                                 .Col = {
                                     .R = 255,
                                     .G = 0,
                                     .B = 0,
                                     .A = 255}};
        Rect.Points[1] = (POINT){.X = Surface.W,
                                 .Y = 0,
                                 .Z = 1,
                                 .Col = {
                                     .R = 0,
                                     .G = 255,
                                     .B = 0,
                                     .A = 255}};
        Rect.Points[2] = (POINT){.X = 0,
                                 .Y = Surface.H,
                                 .Z = 1,
                                 .Col = {
                                     .R = 0,
                                     .G = 0,
                                     .B = 255,
                                     .A = 255}};
        Rect.Points[3] = (POINT){.X = Surface.W,
                                 .Y = Surface.H,
                                 .Z = 1,
                                 .Col = {
                                     .R = 0,
                                     .G = 0,
                                     .B = 0,
                                     .A = 255}};
        for (size_t p = 0; p < 4; ++p)
        {
                Rect.Points[p].X -= Surface.W / 2;
                Rect.Points[p].Y -= Surface.H / 2;
        }
        GDIDrawRect(&Surface, &Rect); /* Precompute */

        char buff[128];
        int i = 0;
        GDIBlitSurface(&Display0, &Surface);
        while (true)
        {
                if (SerialCanRead())
                {
                        char chr = SerialRead();
                        if (i > 126 || chr == '\r')
                        {
                                buff[i] = 0;
                                i = 0;
                                SerialPrint("\r\n");
                                forth(buff);
                        }
                        else if (chr == '\b' && i > 0)
                        {
                                i -= 1;
                                SerialPut('\b');
                        }
                        else
                        {
                                buff[i] = chr;
                                SerialPut(buff[i]);
                                i++;
                        }
                }
        }
        hcf();
}
