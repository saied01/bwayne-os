#include "keyboard.h"
#include <io.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/tty.h>
#include <stdint.h>

#define TOGGLE_SHIFT_IDX 1
#define MOVE_DOWN_IDX 2
#define MOVE_UP_IDX 3
#define MOVE_LEFT_IDX 4
#define MOVE_RIGHT_IDX 5
#define NEW_LINE_IDX 6

struct InterruptRegisters;

void init_keyboard()
{
  irq_install_handler(1, keyboard_handler);
}

void keyboard_handler(struct InterruptRegisters *regs)
{
  (void)regs;
  uint8_t raw = inb(KEYB_DATA_PORT);
  uint8_t scan_code = raw & 0x7F;
  uint8_t press = !(raw & 0x80); // 1 if pressed, 0 if let go

  update_shift(scan_code);

  terminal_handle_scancode(scan_code, press);
}
