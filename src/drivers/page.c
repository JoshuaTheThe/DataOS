#include <drivers/page.h>
#include <drivers/devices/serial.h>
#include <drivers/features/feature.h>

static ptEntry_t pml4[512] __attribute__((aligned(0x1000)));
static ptEntry_t pdpt[512][512] __attribute__((aligned(0x1000)));
static ptEntry_t pd[512][512] __attribute__((aligned(0x1000)));
static ptEntry_t pt[512][512] __attribute__((aligned(0x1000)));

void MapPage(void *virt, void *phys, uint64_t flags)
{
        uint64_t v = (uint64_t)virt;

        int pml4_idx = (v >> 39) & 0x1FF;
        int pdpt_idx = (v >> 30) & 0x1FF;
        int pd_idx = (v >> 21) & 0x1FF;
        int pt_idx = (v >> 12) & 0x1FF;
        if (!(pml4[pml4_idx] & PAGE_PRESENT))
                pml4[pml4_idx] = (uint64_t)&pdpt[pml4_idx] | PAGE_PRESENT | PAGE_RW;
        if (!(pdpt[pml4_idx][pdpt_idx] & PAGE_PRESENT))
                pdpt[pml4_idx][pdpt_idx] = (uint64_t)&pd[pml4_idx] | PAGE_PRESENT | PAGE_RW;
        if (!(pd[pml4_idx][pd_idx] & PAGE_PRESENT))
                pd[pml4_idx][pd_idx] = (uint64_t)&pt[pml4_idx] | PAGE_PRESENT | PAGE_RW;
        pt[pml4_idx][pt_idx] = ((uint64_t)phys & 0xFFFFFFFFF000) | (flags & 0xFFF);
}

void PagingInit(void)
{
        extern uint64_t _kernel_start_phys;
        extern uint64_t _kernel_end_phys;

        uint64_t cr4;
        asm volatile("mov %%cr4, %0" : "=r"(cr4));
        cr4 |= (1 << 5);
        asm volatile("mov %0, %%cr4" ::"r"(cr4));

        FeatureCR0Enable(31);
        for (uint64_t a = (uint64_t)&_kernel_start_phys; a < (uint64_t)&_kernel_end_phys; a += PAGE_SIZE)
        {
                MapPage((void *)a, (void *)a, PAGE_PRESENT | PAGE_RW);
        }

        MapPage((void *)0xFEE00000, (void *)0xFEE00000, PAGE_PRESENT | PAGE_RW | PAGE_NO_CACHE);

        SetCR3((uint64_t)pml4);

        SerialPrint("INFO: Paging Initialised\r\n");
}
