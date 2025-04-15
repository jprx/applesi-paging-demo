# jprx
TOOL_PREFIX = aarch64-elf-
AS = $(TOOL_PREFIX)gcc
CC = $(TOOL_PREFIX)gcc
LD = $(TOOL_PREFIX)gcc
OBJCOPY = $(TOOL_PREFIX)objcopy

FLAGS = -nostdlib -ffreestanding -fPIE -c -O0 -mgeneral-regs-only -mstrict-align
ASFLAGS = $(FLAGS) -x assembler-with-cpp
CFLAGS  = $(FLAGS)
LDFLAGS1 = -nostdlib -ffreestanding -fPIE
LDFLAGS2 = -Wl,--no-warn-rwx-segments -Wl,--no-relax -Wl,-pie -Wl,-no-dynamic-linker

OBJS := start.o paging.o

demo.bin: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS1) -T linker.ld $(LDFLAGS2) -o demo.obj
	$(OBJCOPY) -O binary demo.obj demo.bin

.PHONY: clean
clean:
	rm -f start.o paging.o demo.obj demo.bin
