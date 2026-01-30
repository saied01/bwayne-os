.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
The multiboot standard does not define the value of the stack pointer register
(esp) and it is up to the kernel to provide a stack. This allocates room for a
small stack by creating a symbol at the bottom of it, then allocating 16384
bytes for it, and finally creating a symbol at the top.
*/
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:

  mov $stack_top, %esp
  
  /*
  Up to this point we have GDT loaded, and Paging should be enabled.
  We can enter the high-level kernel and start the operating system
  */
  call kernel_main

  /*
  If the system has nothing more to do, put the computer
  into an infinite loop. To do that:
  1- Disable interrupts with cli (clear interrupt)
  2- Wait for the next interrupt to arrive with hlt (halt instruction).
    And since they are disables this locks up the computer.
  3- Jump to the hlt instruction if it ever wakes up.
  */
  cli
1:
  hlt
  jmp 1b


/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start
