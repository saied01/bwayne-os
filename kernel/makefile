ARCH := i386

AS := i686-elf-as
CC := i686-elf-gcc

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
          -Iinclude

LDFLAGS := -T arch/$(ARCH)/linker.ld \
           -nostdlib -ffreestanding

TARGET := bwayneos 

OBJS := \
	arch/$(ARCH)/boot.o \
	arch/$(ARCH)/tty.o \
	kernel/kernel.o

all: $(TARGET)

qemu: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

arch/$(ARCH)/boot.o: arch/$(ARCH)/boot.s
	$(AS) $< -o $@

arch/$(ARCH)/tty.o: arch/$(ARCH)/tty.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
