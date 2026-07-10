ARCH := i386
KERNEL_ARCH := kernel/arch
DRIVERS := kernel/drivers

AS := i686-elf-as
CC := i686-elf-gcc

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wa,--32 \
          -Iinclude \
          -Ikernel/include \
					-Ilib/include \
					-Ikernel/drivers \
					-Ikernel/arch/i386 \
					-D__is_libk  

LDFLAGS := -T $(KERNEL_ARCH)/$(ARCH)/linker.ld \
           -nostdlib -ffreestanding \
           -B/usr/bin -Wl,-m,elf_i386

TARGET := bwayneos

KERNEL_OBJS := \
	$(KERNEL_ARCH)/$(ARCH)/boot.o \
	$(KERNEL_ARCH)/$(ARCH)/tty.o \
	$(KERNEL_ARCH)/$(ARCH)/gdt.o \
	$(KERNEL_ARCH)/$(ARCH)/gdt_flush.o \
	$(KERNEL_ARCH)/$(ARCH)/idt.o \
	$(KERNEL_ARCH)/$(ARCH)/idt_asm.o \
	$(KERNEL_ARCH)/$(ARCH)/gdt_read_cpu.o \
	$(KERNEL_ARCH)/$(ARCH)/io.o \
	$(KERNEL_ARCH)/$(ARCH)/irq.o \
	kernel/kernel/kernel.o

LIB_OBJS := \
	lib/string/strlen.o \
	lib/string/memset.o \
	lib/stdio/printf.o \
	lib/stdio/putchar.o \
	lib/stdio/itoa.o \

DRIVER_OBJS := \
	$(DRIVERS)/timer/pit.o \
	$(DRIVERS)/pic/pic.o \
	$(DRIVERS)/keyboard/keyboard.o \

OBJS := $(KERNEL_OBJS) $(LIB_OBJS) $(DRIVER_OBJS)

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
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
