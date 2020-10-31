// LED Effect Modes
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "globals.h"

// persistent settings
#include <EEPROM.h>

extern Adafruit_NeoPixel led_fan;

// pack 32-bit color
uint32_t pack_rgb(uint8_t r, uint8_t g, uint8_t b) {
  return (uint32_t)((((uint32_t)r & 0xFF) << 16) | (((uint32_t)g & 0xFF) << 8) | ((uint32_t)b & 0xFF));
}

// helper effects functions
// set all leds to the same color
void solidColor(uint8_t r, uint8_t g, uint8_t b, Adafruit_NeoPixel *controller) {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    controller->setPixelColor(i, pack_rgb(r, g, b));
  }
  controller->show();
}

// utilities
void setupMode(uint8_t * mode_setting, byte * initial_color_array) {
   // read last saved running mode
  *mode_setting = EEPROM.read(MODE_ADDR);

  // if mode is 'static color', read color channel values from eeprom
  if (*mode_setting == MODE_STATICCOLOR) {
    *(initial_color_array) = MODE_STATICCOLOR;
    *(initial_color_array + 1) = EEPROM.read(R_ADDR);
    *(initial_color_array + 2) = EEPROM.read(G_ADDR);
    *(initial_color_array + 3) = EEPROM.read(B_ADDR);
  }
}

void saveMode(uint8_t mode_setting, byte *settings) {
  switch ((uint8_t)settings[0]) {
    case MODE_LIGHTSOFF:
    case MODE_RANDOMCOLOR:
      EEPROM.update(MODE_ADDR, mode_setting);
      break;
    case MODE_STATICCOLOR:
    case MODE_BREATHE:
      EEPROM.update(MODE_ADDR, mode_setting);
      EEPROM.update(R_ADDR, settings[1]);
      EEPROM.update(G_ADDR, settings[2]);
      EEPROM.update(B_ADDR, settings[3]);
      break;
  }
}

// main functions table
void brightWhite() {
  solidColor(255, 255, 255, &led_fan);
}
void brightRed() {
  solidColor(255, 0, 0, &led_fan);
}
void brightGreen() {
  solidColor(0, 255, 0, &led_fan);
}
void brightBlue() {
  solidColor(0, 0, 255, &led_fan);
}

// static color
void staticColor(uint8_t r, uint8_t g, uint8_t b) {
  solidColor(r, g, b, &led_fan);
  if (led_fan.getBrightness() < 0xFF) led_fan.setBrightness(0xFF);
}

// shut lights off
void lightsOff() {
  solidColor(0, 0, 0, &led_fan);
}

// breathe effect
void breathe(uint8_t r, uint8_t g, uint8_t b, uint8_t intensity) {
  staticColor(r, g, b);
  led_fan.setBrightness(intensity);
}
