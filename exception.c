// jprx
#include "types.h"
#include "printf.h"
#include "msr.h"

void __attribute__((noreturn)) handle_exception_el1() {
    printf("ELR_EL1: 0x%X\n", read_msr(ELR_EL1));
    printf("ESR_EL1: 0x%X\n", read_msr(ESR_EL1));
    printf("FAR_EL1: 0x%X\n", read_msr(FAR_EL1));
    printf("TCR_EL1: 0x%X\n", read_msr(TCR_EL1));
    printf("SPSR_EL1: 0x%X\n", read_msr(SPSR_EL1));
    printf("SCTLR_EL1: 0x%X\n", read_msr(SCTLR_EL1));
    while(1);
}

void __attribute__((noreturn)) handle_exception_el2() {
    printf("ELR_EL2: 0x%X\n", read_msr(ELR_EL2));
    printf("ESR_EL2: 0x%X\n", read_msr(ESR_EL2));
    printf("FAR_EL2: 0x%X\n", read_msr(FAR_EL2));
    printf("TCR_EL1: 0x%X\n", read_msr(TCR_EL1));
    printf("SPSR_EL1: 0x%X\n", read_msr(SPSR_EL1));
    printf("SPSR_EL2: 0x%X\n", read_msr(SPSR_EL2));
    printf("HCR_EL2: 0x%X\n", read_msr(HCR_EL2));
    printf("SCTLR_EL1: 0x%X\n", read_msr(SCTLR_EL1));
    printf("SCTLR_EL2: 0x%X\n", read_msr(SCTLR_EL2));
    printf("HPFAR_EL2: 0x%X\n", read_msr(HPFAR_EL2));
    while(1);
}
