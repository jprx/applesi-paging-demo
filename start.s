// jprx
.global demo_start

#include "serial.h"
#include "config.h"

#if !defined(DEMO_USE_4K) && !defined(DEMO_USE_16K)
#error "You need to pick either 4K or 16K pages (see config.h)"
#endif

#if defined(DEMO_USE_4K) && defined(DEMO_USE_16K)
#error "You need to pick either 4K or 16K pages (see config.h)"
#endif

#ifdef DEMO_USE_4K
#define TCR_EL1_TG0        ((0b00 << 14ULL))
#define TCR_EL1_TG1        ((0b10 << 30ULL))
#define TCR_EL2_TG0        ((0b00 << 14ULL))
#endif // DEMO_USE_4K

#ifdef DEMO_USE_16K
#define TCR_EL1_TG0        ((0b10 << 14ULL))
#define TCR_EL1_TG1        ((0b01 << 30ULL))
#define TCR_EL2_TG0        ((0b10 << 14ULL))
#endif // DEMO_USE_16K

#define TCR_EL1_T0SZ       ((16 << 0ULL))
#define TCR_EL1_T1SZ       ((16 << 16ULL))
#define TCR_EL2_T0SZ       ((16 << 0ULL))

/* 52 bits (4PB) physical addresses */
// #define TCR_EL1_IPS        ((0b110 << 32ULL))

/* 42 bits (4TB) physical addresses */
#define TCR_EL1_IPS        ((0b011 << 32ULL))

/* 48 bits */
#define TCR_EL2_PS ((0b101 << 16ULL))

/* Access Flag in hardware */
/* Prevents hitting exceptions every memory access */
#define TCR_EL1_HA         ((1 << 39ULL))
#define TCR_EL2_HA         ((1 << 21ULL))

// 0b01: Write-back
#define TCR_IRGN ((0b01))
#define TCR_ORGN ((0b01))
#define TCR_ORGN0 ((TCR_ORGN << 10ULL))
#define TCR_ORGN1 ((TCR_ORGN << 26ULL))
#define TCR_IRGN0 ((TCR_IRGN << 8ULL))
#define TCR_IRGN1 ((TCR_IRGN << 24ULL))

// SH0/ SH1 inner shared
#define TCR_SH0 ((0b11 << 12ULL))
#define TCR_SH1 ((0b11 << 28ULL))

#define TCR_DEFAULT_EL1 ((TCR_EL1_TG0 | TCR_EL1_TG1 | TCR_EL1_T0SZ | TCR_EL1_T1SZ | TCR_EL1_IPS | TCR_EL1_HA | TCR_IRGN0 | TCR_ORGN0 | TCR_IRGN1 | TCR_ORGN1 | TCR_SH0 | TCR_SH1))
#define TCR_DEFAULT_EL2 ((TCR_EL2_TG0 | TCR_EL2_T0SZ | TCR_EL2_PS | TCR_EL2_HA))

// Keep it simple- all memory is device memory nGnRnE
#define MAIR_BRINGUP 0x0000000000000000ULL

// In reality, this is 0x480, but we grab a few extra bytes to be safe
#define BOOT_ARGS_SIZE ((0x500))

// MOV64(r, imm)- mov 64-bit immediate imm into register r
.macro MOV64 r, imm
    movz \r, #(((\imm) >> 48) & 0x0FFFF), lsl #48
    movk \r, #(((\imm) >> 32) & 0x0FFFF), lsl #32
    movk \r, #(((\imm) >> 16) & 0x0FFFF), lsl #16
    movk \r, #(((\imm) >> 00) & 0x0FFFF), lsl #00
.endm

// LEA64(r, symbol)- load pc-relative address of symbol into register r
.macro LEA64 r, symbol
    adrp \r, \symbol
    add \r, \r, :lo12:\symbol
.endm

// SERIAL_PUTC(c)- write char 'c' to UART (destroys x21 and x22)
.macro SERIAL_PUTC c
    MOV64 x21, SERIAL_BASEADDR
    mov x22, \c
    str w22, [x21, SERIAL_WRITE_PORT_OFFSET]
.endm

.section .text.start, "ax"

demo_start:
    /* Disable interrupts */
    msr daifset, 0xF

    /* iBoot info */
    mov x27, x0

    SERIAL_PUTC 'H'
    SERIAL_PUTC 'i'
    SERIAL_PUTC '\n'

    /* Switch to EL1, if we aren't already there */
    mrs x0, CurrentEL
    cmp x0, 4
    beq demo_start_el1

switch_to_el1:
    /* Setup an exception table in EL2 before we leave,
     * so we can catch any exceptions that make their way up here */
    LEA64 x0, exception_el2
    msr vbar_el2, x0

    msr sctlr_el1, xzr

    /* HCR.RW = 1 (enable AARCH64 at EL1) */
    /* Seems like we can't turn off E2H nor RW */
    msr hcr_el2, xzr
    mrs x0, hcr_el2
    orr x0, x0, #(1 << 31)
    msr hcr_el2, x0

    MOV64 x0, TCR_DEFAULT_EL2
    msr tcr_el2, x0

    MOV64 x0, MAIR_BRINGUP
    msr mair_el2, x0

    LEA64 x0, demo_start_el1
    msr elr_el2, x0

    // I and F set, prev mode is EL1h
    MOV64 x0, 0x65
    msr spsr_el2, x0
    eret

demo_start_el1:
    LEA64 x0, exception_el1
    msr vbar_el1, x0

    /* C code needs a stack */
    mov x0, 1
    msr SPSel, x0
    adrp fp, _stack_bottom
    and fp, fp, #0xFFFFFFFFFFFF0000
    mov sp, fp

    /* Set aside iBoot info for later */
    /* We're going to overwrite it in RAM with page tables so copy it somewhere safe */
    LEA64 x0, boot_args_copy
    mov x1, x27
    MOV64 x2, BOOT_ARGS_SIZE
    bl _memcpy_fast

    /* Construct page tables at runtime, 
     * as we don't know where we'll be loaded */
#ifdef DEMO_USE_4K
    LEA64 x0, _page_tables
    bl make_4k_pagetable
#endif // DEMO_USE_4K

#ifdef DEMO_USE_16K
    LEA64 x0, _page_tables
    bl make_16k_pagetable
#endif // DEMO_USE_16K

    LEA64 x0, _page_tables
    msr ttbr0_el1, x0
    msr ttbr1_el1, x0
    MOV64 x0, MAIR_BRINGUP
    msr mair_el1, x0
    MOV64 x0, TCR_DEFAULT_EL1
    msr tcr_el1, x0
    isb

    /* Just enable MMU (no caches or anything) */
    mrs x0, sctlr_el1
    orr x0, x0, #1
    msr sctlr_el1, x0

    /* Flush TLB */
    dsb ishst
    tlbi vmalle1
    dsb ish
    isb

    /* Paging is now enabled */
    LEA64 x0, boot_args_copy
    bl demo_main
    b .

.align 14
exception_el2:
    .rept 8192
    nop
    .endr
    SERIAL_PUTC 'E'
    SERIAL_PUTC 'L'
    SERIAL_PUTC '2'
    SERIAL_PUTC '\n'
    mov x0, 1
    msr SPSel, x0
    LEA64 fp, _stack_bottom
    mov sp, fp
    b handle_exception_el2

.align 14
exception_el1:
    .rept 8192
    nop
    .endr
    SERIAL_PUTC 'E'
    SERIAL_PUTC 'L'
    SERIAL_PUTC '1'
    SERIAL_PUTC '\n'
    mov x0, 1
    msr SPSel, x0
    LEA64 fp, _stack_bottom
    mov sp, fp
    b handle_exception_el1


boot_args_copy:
    .fill 0x500, 1, 0
