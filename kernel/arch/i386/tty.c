#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer = (uint16_t *)VGA_MEMORY;
/* ----------------- TERMINAL INITIALIZATION AND BASICS ---------------- */
void terminal_initialize(void)
{
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

  for (size_t y = 0; y < VGA_HEIGHT; y++)
  {
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color)
{
  terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(void)
{
  for (size_t y = 1; y < VGA_HEIGHT; y++)
  {
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
    }
  }

  for (size_t x = 0; x < VGA_WIDTH; x++)
    terminal_putentryat(' ', terminal_color, x, VGA_HEIGHT - 1);
}

void terminal_putchar(char c)
{
  if (c == '\n')
  {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
    {
      terminal_scroll();
      terminal_row = VGA_HEIGHT - 1;
    }
    return;
  }

  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

  if (++terminal_column == VGA_WIDTH)
  {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
    {
      terminal_scroll();
      terminal_row = VGA_HEIGHT - 1;
    }
  }
}

void terminal_write(const char *data, size_t size)
{
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data)
{
  terminal_write(data, strlen(data));
}

/* ---------------------------- TERMINAL MOVEMENTS --------------------- */

#define TOGGLE_SHIFT_IDX 1
#define MOVE_DOWN_IDX 2
#define MOVE_UP_IDX 3
#define MOVE_LEFT_IDX 4
#define MOVE_RIGHT_IDX 5
#define NEW_LINE_IDX 6

struct scancode_info
{
  // If value is different than '\0', main_value is printed (if shift not held)
  uint8_t main_value;

  // if main_value is different than '\0' then print shifted value.
  // else it has an index in the special_scancodes table that determines value of execution
  uint8_t special_value;
};

static const struct scancode_info scancode_defs[256] = {
    [0x01] = {'\0', '\0'}, // Esc

    [0x02] = {'1', '!'},
    [0x03] = {'2', '@'},
    [0x04] = {'3', '#'},
    [0x05] = {'4', '$'},
    [0x06] = {'5', '%'},
    [0x07] = {'6', '^'},
    [0x08] = {'7', '&'},
    [0x09] = {'8', '*'},
    [0x0a] = {'9', '('},
    [0x0b] = {'0', ')'},
    [0x0c] = {'-', '_'},
    [0x0d] = {'=', '+'},
    [0x0e] = {'\0', MOVE_LEFT_IDX}, // Backspace

    [0x0f] = {'\0', '\0'}, // Tab
    [0x10] = {'q', 'Q'},
    [0x11] = {'w', 'W'},
    [0x12] = {'e', 'E'},
    [0x13] = {'r', 'R'},
    [0x14] = {'t', 'T'},
    [0x15] = {'y', 'Y'},
    [0x16] = {'u', 'U'},
    [0x17] = {'i', 'I'},
    [0x18] = {'o', 'O'},
    [0x19] = {'p', 'P'},
    [0x1a] = {'[', '{'},
    [0x1b] = {']', '}'},

    [0x1c] = {'\0', NEW_LINE_IDX}, // Enter

    [0x1d] = {'\0', '\0'}, // LCtrl

    [0x1e] = {'a', 'A'},
    [0x1f] = {'s', 'S'},
    [0x20] = {'d', 'D'},
    [0x21] = {'f', 'F'},
    [0x22] = {'g', 'G'},
    [0x23] = {'h', 'H'},
    [0x24] = {'j', 'J'},
    [0x25] = {'k', 'K'},
    [0x26] = {'l', 'L'},
    [0x27] = {';', ':'},
    [0x28] = {'\'', '"'},

    [0x29] = {'`', '~'},

    [0x2a] = {'\0', TOGGLE_SHIFT_IDX},        // LShift
    [0x80 | 0x2a] = {'\0', TOGGLE_SHIFT_IDX}, // LShift (UP)

    [0x2b] = {'\\', '|'}, // on a 102-key keyboard

    [0x2c] = {'z', 'Z'},
    [0x2d] = {'x', 'X'},
    [0x2e] = {'c', 'C'},
    [0x2f] = {'v', 'V'},
    [0x30] = {'b', 'B'},
    [0x31] = {'n', 'N'},
    [0x32] = {'m', 'M'},
    [0x33] = {',', '<'},
    [0x34] = {'.', '>'},
    [0x35] = {'/', '?'},
    [0x36] = {'\0', '\0'}, // RShift

    [0x37] = {'\0', '\0'}, // Keypad-* or */PrtScn on a 83/84-key keyboard

    [0x38] = {'\0', '\0'}, // LAlt
    [0x39] = {' ', ' '},   // Space bar

    [0x3a] = {'\0', TOGGLE_SHIFT_IDX}, // CapsLock

    // IDEA: ¿Como usarian los Fx para poder cambiar de color?
    [0x3b] = {'\0', '\0'}, // F1
    [0x3c] = {'\0', '\0'}, // F2
    [0x3d] = {'\0', '\0'}, // F3
    [0x3e] = {'\0', '\0'}, // F4
    [0x3f] = {'\0', '\0'}, // F5
    [0x40] = {'\0', '\0'}, // F6
    [0x41] = {'\0', '\0'}, // F7
    [0x42] = {'\0', '\0'}, // F8
    [0x43] = {'\0', '\0'}, // F9
    [0x44] = {'\0', '\0'}, // F10

    [0x45] = {'\0', '\0'}, // NumLock

    [0x46] = {'\0', '\0'}, // ScrollLock

    [0x47] = {'\0', '\0'},        // Keypad-7/Home
    [0x48] = {'\0', MOVE_UP_IDX}, // Keypad-8/Up
    [0x49] = {'\0', '\0'},        // Keypad-9/PgUp

    [0x4a] = {'\0', '\0'}, // Keypad--

    [0x4b] = {'\0', MOVE_LEFT_IDX},  // Keypad-4/Left
    [0x4c] = {'\0', '\0'},           // Keypad-5
    [0x4d] = {'\0', MOVE_RIGHT_IDX}, // Keypad-6/Right
    [0x4e] = {'\0', '\0'},           // Keypad-+

    [0x4f] = {'\0', '\0'},          // Keypad-1/End
    [0x50] = {'\0', MOVE_DOWN_IDX}, // Keypad-2/Down
    [0x51] = {'\0', '\0'},          // Keypad-3/PgDn

    [0x52] = {'\0', '\0'}, // Keypad-0/Ins
    [0x53] = {'\0', '\0'}, // Keypad-./Del

    [0x54] = {'\0', '\0'}, // Alt-SysRq on a 84+ key keyboard

    [0x57] = {'\0', '\0'}, // F11 on a 101+ key keyboard
    [0x58] = {'\0', '\0'}  // F12 on a 101+ key keyboard
};

static uint8_t shift_active = 0;

static void fix_rowcol(void)
{
  if (terminal_column >= VGA_WIDTH)
  {
    terminal_column = 0;
    terminal_row++;
  }

  if ((int)terminal_column < 0)
  {
    terminal_column = VGA_WIDTH - 1;
    terminal_row--;
  }

  if (terminal_row >= VGA_HEIGHT)
  {
    terminal_scroll();
    terminal_row = VGA_HEIGHT - 1;
  }

  if ((int)terminal_row < 0)
  {
    terminal_row = 0;
  }
}

static void tty_move_down()
{
  terminal_row++;
  fix_rowcol();
}

static void tty_move_up()
{
  terminal_row--;
  fix_rowcol();
}

static void tty_move_left(void)
{
  if (terminal_column > 0)
  {
    terminal_column--;
  }
  else if (terminal_row > 0)
  {
    terminal_row--;
    terminal_column = VGA_WIDTH - 1;
  }
  terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
}

static void tty_move_right()
{
  terminal_column++;
  fix_rowcol();
}

static void tty_newline()
{
  terminal_column = 0;
  terminal_row++;
  fix_rowcol();
}

void update_shift(uint8_t scan_code)
{
  if (scan_code == 0x2A || scan_code == 0x36)
    shift_active = !shift_active;
}

void terminal_handle_control(uint8_t control)
{
  switch (control)
  {
  case MOVE_RIGHT_IDX:
    tty_move_right();
    break;
  case MOVE_LEFT_IDX:
    tty_move_left();
    break;
  case NEW_LINE_IDX:
    tty_newline();
    break;
  case MOVE_UP_IDX:
    tty_move_up();
    break;
  case MOVE_DOWN_IDX:
    tty_move_down();
    break;
  }
}

void terminal_handle_scancode(uint8_t scan_code, uint8_t press)
{
  const struct scancode_info *info = &scancode_defs[scan_code];

  if (info->main_value == '\0')
  {
    if (press)
      terminal_handle_control(info->special_value);
    return;
  }

  if (press)
  {
    terminal_putchar(shift_active ? info->special_value : info->main_value);
  }
}
