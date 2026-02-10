#ifndef PIC_H
#define PIC_H

#include <stdint.h>
#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)
#define PIC_EOI 0x20

void pic_init(void);
void pic_remap(uint8_t, uint8_t);
void pic_send_eoi(uint8_t);
void PIC_disable(void);
void irq_set_mask(uint8_t);
void irq_clear_mask(uint8_t);

#endif // !PIC_H
