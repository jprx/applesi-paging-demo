#ifndef CONFIG_H
#define CONFIG_H

// Do we use 4k or 16k paging?
#define DEMO_USE_4K
//#define DEMO_USE_16K

// If 4k, do we use 1GB giant pages or 2MB huge pages?
#ifdef DEMO_USE_4K
// #define USE_1GB_PAGES
#define USE_2MB_PAGES
#endif // DEMO_USE_4K

#endif // CONFIG_H
