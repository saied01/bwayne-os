ARCH := i386
KERNEL_ARCH := kernel/arch

AS := i686-elf-as
CC := i686-elf-gcc

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
          -Iinclude \
          -Ikernel/include

LDFLAGS := -T $(KERNEL_ARCH)/$(ARCH)/linker.ld \
           -nostdlib -ffreestanding

TARGET := bwayneos

KERNEL_OBJS := \
	$(KERNEL_ARCH)/$(ARCH)/boot.o \
	$(KERNEL_ARCH)/$(ARCH)/tty.o \
	$(KERNEL_ARCH)/$(ARCH)/gdt.o \
	kernel/kernel/kernel.o

LIB_OBJS := \
	lib/string/strlen.o

OBJS := $(KERNEL_OBJS) $(LIB_OBJS)

all: $(TARGET)

qemu: $(TARGET)
	qemu-system-i386 -kernel $(TARGET) -display gtk

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# C → o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ASM → o
%.o: %.s
	$(AS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
