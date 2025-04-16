#ifndef MEM_H
#define MEM_H

#include "types.h"

// Borrowed from m1n1 (https://github.com/AsahiLinux/m1n1/blob/main/src/utils.h#L85)
static inline u32 read32(u64 addr)
{
    u32 data;
    asm volatile("ldr %w0, [%1]" : "=r"(data) : "r"(addr) : "memory");
    return data;
}

// Borrowed from m1n1 (https://github.com/AsahiLinux/m1n1/blob/main/src/utils.h#L92)
static inline void write32(u64 addr, u32 data)
{
    asm volatile("str %w0, [%1]" : : "r"(data), "r"(addr) : "memory");
}

#endif // MEM_H
