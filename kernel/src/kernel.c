#include <stdint.h>

#include "bootinfo.h"

void kernel_main(BootInfo *boot_info)
{
    (void)boot_info;

    for (;;) {
        __asm__ volatile (
            "hlt"
        );
    }
}