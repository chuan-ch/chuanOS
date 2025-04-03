CC = g++
ASM = nasm
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-x86_64

CFLAGS = -ffreestanding -nostdlib -O0 -Wall -Wextra
ASMFLAGS = -f bin
LDFLAGS = -nostdlib -T kernel/linker.ld

SRCDIR = .
BUILDDIR = build

KERNEL_SRCS = $(wildcard kernel/*.cpp) $(wildcard hal/*.cpp)
KERNEL_OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(KERNEL_SRCS))

all: os-image.bin

$(BUILDDIR)/boot.bin: boot/boot.S
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/kernel.bin: $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@ $@

os-image.bin: $(BUILDDIR)/boot.bin $(BUILDDIR)/kernel.bin
	cat $^ > $@
	truncate -s 1440k $@

run: os-image.bin
	$(QEMU) -drive format=raw,file=$<

debug: os-image.bin
	$(QEMU) -s -S -drive format=raw,file=$< &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(BUILDDIR)/kernel.bin"

clean:
	rm -rf $(BUILDDIR) os-image.bin

.PHONY: all run debug clean