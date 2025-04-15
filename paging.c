// jprx
#include <stdint.h>
#include <stddef.h>

typedef uint64_t u64;
typedef size_t usize;

#define NUM_ENTRIES_4K     512
#define NUM_ENTRIES_16K    2048

#define BLOCK_ENTRY        0x401
#define TABLE_DESCRIPTOR   0x03

void make_4k_pagetable(u64 *l0_pt, u64 *tables) {
    for (usize i = 0; i < NUM_ENTRIES_4K; i++) {
        for (usize j = 0; j < NUM_ENTRIES_4K; j++) {
            tables[(NUM_ENTRIES_4K*i)+j] = (i << 39ull) | (j << 30ull) | BLOCK_ENTRY;
        }
    }

    for (usize i = 0; i < NUM_ENTRIES_4K; i++) {
        l0_pt[i] = (u64)(&tables[NUM_ENTRIES_4K*i]) | TABLE_DESCRIPTOR;
    }
}

void make_16k_pagetable(u64 *l0_pt, u64 *l1_pt, u64 *tables) {
    for (usize i = 0; i < NUM_ENTRIES_16K; i++) {
        for (usize j = 0; j < NUM_ENTRIES_16K; j++) {
            tables[(NUM_ENTRIES_16K*i)+j] = (i << 36ull) | (j << 25ull) | BLOCK_ENTRY;
        }
    }

    for (usize i = 0; i < NUM_ENTRIES_16K; i++) {
        l1_pt[i] = (u64)(&tables[NUM_ENTRIES_16K*i]) | TABLE_DESCRIPTOR;
    }

    l0_pt[0] = (u64)&l1_pt[0] | TABLE_DESCRIPTOR;
    l0_pt[1] = (u64)&l1_pt[0] | TABLE_DESCRIPTOR;
}
