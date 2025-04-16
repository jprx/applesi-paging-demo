// jprx
#include "types.h"
#include "msr.h"
#include "config.h"
#include "printf.h"

#ifdef DEMO_USE_16K

void dump_pagetables() {
    u64 *pt = (u64*)read_msr(TTBR0_EL1);
    for (usize i = 0; i < 4; i++) {
        printf("TTBR0_EL1[%d]: 0x%X\n", i, pt[i]);
    }
    u64 *l1_pt = (u64*)(pt[0] & ~(0x0FFF));
    for (usize i = 0; i < 16; i++) {
        printf("0x%X: 0x%X\n", &l1_pt[i], l1_pt[i]);
    }
    u64 *l2_pt = (u64*)(l1_pt[0] & ~(0x0FFF));
    for (usize i = 0; i < 16; i++) {
        printf("0x%X: 0x%X\n", &l2_pt[i], l2_pt[i]);
    }
}

#endif // DEMO_USE_16K

#ifdef DEMO_USE_4K

void dump_pagetables() {
    u64 *pt = (u64*)read_msr(TTBR0_EL1);
    for (usize i = 0; i < 4; i++) {
        printf("TTBR0_EL1[%d]: 0x%X\n", i, pt[i]);
    }
    u64 *l1_pt = (u64*)(pt[0] & ~(0x0FFF));
    for (usize i = 0; i < 16; i++) {
        printf("0x%X: 0x%X\n", &l1_pt[i], l1_pt[i]);
    }
    u64 *l2_pt = (u64*)(l1_pt[0] & ~(0x0FFF));
    for (usize i = 0; i < 16; i++) {
        printf("0x%X: 0x%X\n", &l2_pt[i], l2_pt[i]);
    }
}

#endif // DEMO_USE_4K

void dump_regs() {
    printf("SPSR_EL1: 0x%X\n", read_msr(SPSR_EL1));
    printf("TCR_EL1: 0x%X\n", read_msr(TCR_EL1));
    printf("MAIR_EL1: 0x%X\n", read_msr(MAIR_EL1));
    printf("TTBR0_EL1: 0x%X\n", read_msr(TTBR0_EL1));
    printf("SCTLR_EL1: 0x%X\n", read_msr(SCTLR_EL1));
    printf("SPSR_EL2: 0x%X\n", read_msr(SPSR_EL2));
    printf("TCR_EL2: 0x%X\n", read_msr(TCR_EL2));
    printf("MAIR_EL2: 0x%X\n", read_msr(MAIR_EL2));
    printf("TTBR0_EL2: 0x%X\n", read_msr(TTBR0_EL2));
    printf("SCTLR_EL2: 0x%X\n", read_msr(SCTLR_EL2));
    printf("HCR_EL2: 0x%X\n", read_msr(HCR_EL2));
    printf("VTCR_EL2: 0x%X\n", read_msr(VTCR_EL2));
    printf("VTTBR_EL2: 0x%X\n", read_msr(VTTBR_EL2));
    dump_pagetables();
}
