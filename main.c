#include "types.h"
#include "xnu.h"
#include "msr.h"
#include "printf.h"

void demo_main(struct boot_args *args) {
    printf("CurrentEL: %d\n", read_msr(CurrentEL));
    printf("physBase: 0x%X\n", args->physBase);
    printf("devtree: 0x%X (%d bytes)\n", args->deviceTreeP, args->deviceTreeLength);
    printf("boot-args: %s\n", args->CommandLine);
    while(1);
}