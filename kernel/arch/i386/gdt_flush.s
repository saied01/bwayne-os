# loading own gdt into memory
    .section .text
    .code32
    .global gdt_flush
    .global tss_flush

gdt_flush:
    movl 4(%esp), %eax
    lgdt (%eax)

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

  # jump to flush at 0x08
    ljmp $0x08, $.flush

.flush:
# return to gdt.c
    ret

tss_flush:
  movw $0x2B, %ax 
  ltr %ax
  ret
