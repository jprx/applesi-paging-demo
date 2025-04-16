#ifndef XNU_H
#define XNU_H

#include <stdint.h>

// xnu-11215.81.4/pexpert/pexpert/arm64/boot.h

#define BOOT_LINE_LENGTH        1024

/*
 * Video information..
 */

struct Boot_Video {
	unsigned long   v_baseAddr;     /* Base address of video memory */
	unsigned long   v_display;      /* Display Code (if Applicable */
	unsigned long   v_rowBytes;     /* Number of bytes per pixel row */
	unsigned long   v_width;        /* Width */
	unsigned long   v_height;       /* Height */
	unsigned long   v_depth;        /* Pixel Depth and other parameters */
};

#define kBootVideoDepthMask             (0xFF)
#define kBootVideoDepthDepthShift       (0)
#define kBootVideoDepthRotateShift      (8)
#define kBootVideoDepthScaleShift       (16)
#define kBootVideoDepthBootRotateShift  (24)

#define kBootFlagsDarkBoot              (1ULL << 0)

typedef struct Boot_Video       Boot_Video;

/* Boot argument structure - passed into Mach kernel at boot time.
 */
#define kBootArgsRevision               1
#define kBootArgsRevision2              2       /* added boot_args.bootFlags */
#define kBootArgsVersion1               1
#define kBootArgsVersion2               2

typedef struct boot_args {
	uint16_t                Revision;                       /* Revision of boot_args structure */
	uint16_t                Version;                        /* Version of boot_args structure */
	uint64_t                virtBase;                       /* Virtual base of memory */
	uint64_t                physBase;                       /* Physical base of memory */
	uint64_t                memSize;                        /* Size of memory */
	uint64_t                topOfKernelData;        /* Highest physical address used in kernel data area */
	Boot_Video              Video;                          /* Video Information */
	uint32_t                machineType;            /* Machine Type */
	void                    *deviceTreeP;           /* Base of flattened device tree */
	uint32_t                deviceTreeLength;       /* Length of flattened tree */
	char                    CommandLine[BOOT_LINE_LENGTH];  /* Passed in command line */
	uint64_t                bootFlags;              /* Additional flags specified by the bootloader */
	uint64_t                memSizeActual;          /* Actual size of memory */
} boot_args;

#endif // XNU_H
