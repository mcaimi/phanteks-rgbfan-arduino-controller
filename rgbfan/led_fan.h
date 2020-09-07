#include <Adafruit_NeoPixel.h>

// led strip controller variable
Adafruit_NeoPixel led_fan;
// effect mode
// 0 - leds off
// 1 - random cycle between default colors
// 2 - set static color (saved in eeprom, controlled by external tool)
uint8_t selected_mode;

// current color in random mode
uint8_t current_color;
// value read from serial
const uint8_t msg_len = 4;
static byte rgb[msg_len + 1];
