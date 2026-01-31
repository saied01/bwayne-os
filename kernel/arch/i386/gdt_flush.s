; loading own gdt into memory
; [bits 32]

section .text
global gdt_flush
gdt_flush:
  mov eax, [esp + 4]
  lgdt [eax]

  mov ax, 0x10 ; data selector
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov ss, ax
  mov gs, ax

  ; jump to flush at 0x08
  jmp 0x08:.flush

.flush:
; return to gdt.c
  ret
