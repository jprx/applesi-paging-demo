// jprx
#include <stdint.h>
#include <stddef.h>
#include "printf.h"

typedef uint64_t u64;
typedef size_t usize;

#define NUM_ENTRIES_4K     512
#define NUM_ENTRIES_16K    2048

#define BLOCK_ENTRY        0x401
#define TABLE_DESCRIPTOR   0x03

void make_4k_pagetable(u64 *pg_tables) {
    u64 *l0_pt = &pg_tables[0 * NUM_ENTRIES_4K];
    u64 *l1_tables = &pg_tables[1 * NUM_ENTRIES_4K];
    u64 *l2_tables = &pg_tables[(NUM_ENTRIES_4K + 1) * NUM_ENTRIES_4K];

    for (usize i = 0; i < NUM_ENTRIES_4K; i++) {
        for (usize j = 0; j < NUM_ENTRIES_4K; j++) {
            for (usize k = 0; k < NUM_ENTRIES_4K; k++) {
                l2_tables[(NUM_ENTRIES_4K*NUM_ENTRIES_4K*i)+(NUM_ENTRIES_4K*j)+k] = (i << 39ull) | (j << 30ull) | (k << 21ull) | BLOCK_ENTRY;
            }
        }
    }

    for (usize i = 0; i < NUM_ENTRIES_4K; i++) {
        for (usize j = 0; j < NUM_ENTRIES_4K; j++) {
            l1_tables[(NUM_ENTRIES_4K*i)+j] = (u64)(&l2_tables[(NUM_ENTRIES_4K*NUM_ENTRIES_4K*i)+(NUM_ENTRIES_4K*j)]) | TABLE_DESCRIPTOR;
        }
    }

    for (usize i = 0; i < NUM_ENTRIES_4K; i++) {
        l0_pt[i] = (u64)(&l1_tables[NUM_ENTRIES_4K*i]) | TABLE_DESCRIPTOR;
    }
}

void make_16k_pagetable(u64 *pg_tables) {
    u64 *l0_pt = &pg_tables[0];
    u64 *l1_pt = &pg_tables[NUM_ENTRIES_16K];
    u64 *l2_tables = &pg_tables[2 * NUM_ENTRIES_16K];
    for (usize i = 0; i < NUM_ENTRIES_16K; i++) {
        for (usize j = 0; j < NUM_ENTRIES_16K; j++) {
            l2_tables[(NUM_ENTRIES_16K*i)+j] = (i << 36ull) | (j << 25ull) | BLOCK_ENTRY;
        }
    }

    for (usize i = 0; i < NUM_ENTRIES_16K; i++) {
        l1_pt[i] = (u64)(&l2_tables[NUM_ENTRIES_16K*i]) | TABLE_DESCRIPTOR;
    }

    l0_pt[0] = (u64)&l1_pt[0] | TABLE_DESCRIPTOR;
    l0_pt[1] = (u64)&l1_pt[0] | TABLE_DESCRIPTOR;
}
