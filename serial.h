// jprx

#ifndef SERIAL_H
#define SERIAL_H

// Setup your serial port here
// You can find the address of uart0 in your platform's device tree
// (Look in /S/V/Preboot, or an IPSW file)

// #define APPLESI_MODE_VMAPPLE
#define APPLESI_MODE_M4

// Real M4 devices:
#ifdef APPLESI_MODE_M4
#define SERIAL_BASEADDR 0x3AD200000
#define SERIAL_WRITE_PORT_OFFSET 0x20
#endif // APPLESI_MODE_M4

// VMAPPLE devices:
#ifdef APPLESI_MODE_VMAPPLE
#define SERIAL_BASEADDR 0x20010000
#define SERIAL_WRITE_PORT_OFFSET 0x00
#endif // APPLESI_MODE_VMAPPLE

#endif // SERIAL_H
