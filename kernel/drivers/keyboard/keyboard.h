#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#define ACK 0xFA
#define RESEND 0xFE
#define KEYB_STATUS_PORT 0x64
#define KEYB_DATA_PORT 0x60

#include <kernel/idt.h>

void init_keyboard();
void keyboard_handler(struct InterruptRegisters *);
void handle_scancode(uint8_t, uint8_t);

#endif // !KEYBOARD_H
