# Reads the current GDTR register into memory.
# sgdt stores the base address and limit of the GDT that the CPU is
# actually using at runtime. By comparing this value against the
# descriptor passed to lgdt, the kernel can verify that its own GDT
# was successfully loaded and is active, rather than a leftover GDT
# from the bootloader or previous execution stage.
.section .text
.global gdt_read_cpu
.extern GDT_DESC_CPU

gdt_read_cpu:
    sgdt GDT_DESC_CPU
    ret
