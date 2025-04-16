# Apple Silicon 4K vs 16K Paging Demo

A simple bare metal test to run on Apple Silicon Macs that sets up identity paging in the lower half (TTBR0) using either 4K or 16K page sizes.

Requirements:
- `aarch64-elf-gcc` cross compiler (`brew install aarch64-elf-gcc`)
- `macvdmtool` (plus a USB SuperSpeed cable) to view serial output on real hardware

# Tested Environments

|                 |  4K  | 16K |
|-----------------|------|------|
|VMAPPLE on M1 Pro|  ✅  |  ✅  |
|  M4 Bare Metal  |  ✅  |  ✅  |

# Setup

First, pick your mode (4k or 16k page sizes) in `config.h`.
Then, make sure your serial port is correct for your platform in `serial.h`.

Finally, follow these steps to build and run:
1. `make`
2. Copy `demo.bin` somewhere you can access it in 1TR / your VM
3. Boot into 1TR and downgrade to permissive security
4. Install this with `kmutil configure-boot -v /Volumes/[YOUR VOLUME] -c demo.bin -r --entry-point 0 --lowest-virtual-address 0`
5. Use `macvdmtool` or watch your VM's serial port to see output
