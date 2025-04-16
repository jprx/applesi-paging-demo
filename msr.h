#ifndef MSR_H
#define MSR_H

#define read_msr(r) \
({                                             \
    u64 v;                                     \
    asm volatile("mrs %0, " #r : "=r"(v));     \
    v;                                         \
})

#define write_msr(r, v) ({ asm volatile("msr " #r ", %0" :: "r"(v)) })

#endif // MSR_H
