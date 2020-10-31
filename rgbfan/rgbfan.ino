// Phanteks RGB Fan Controller
// This only works for now for my Phanteks AuraSync Fan
// And only the one with a 3-pin connector (+5V power supply)
#include "globals.h"
#include "led_fan.h"
#include "modes.h"
#include "utils.h"

// effects table, for now only solid colors...
void (*effectTable[NUM_MODES])() = {
  brightWhite,
  brightRed,
  brightBlue,
  brightGreen
};

// setup and run Arduino functions
void setup() {
  // initialize serial line
  Serial.begin(SERIAL_SPEED, SERIAL_MODE);
  Serial.setTimeout(SERIAL_TIMEOUT);

  // clean rgb array
  memset(rgb, 0x00, msg_len + 1);

  // zero out current_color
  current_color = 0;

  // breathe level starts at 0
  breathe_intensity = 0;
  breathe_direction = 0;

  // setup run mode
  setupMode(&selected_mode, rgb);

  // add random enthropy
  randomSeed(analogRead(0));

  // create a FAN objetct (12 leds, pin 6)
  led_fan = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
  led_fan.begin();
}

// serial callback
void serialEvent() {
  if (Serial.available()) {
    if (Serial.readBytes(rgb, msg_len) == msg_len) {
      // parse protocol messages
      switch ((uint8_t)rgb[0]) {
        case MODE_LIGHTSOFF:
          // set selected mode to lights off
          selected_mode = 0;
          saveMode(selected_mode, rgb);
          Serial.write(OP_OK);
          break;
        case MODE_RANDOMCOLOR:
          // set selected mode to random colors
          selected_mode = 1;
          saveMode(selected_mode, rgb);
          Serial.write(OP_OK);
          break;
        case MODE_STATICCOLOR: 
          // selects static color from input values
          selected_mode = 2;
          saveMode(selected_mode, rgb);
          Serial.write(OP_OK);
          break;
        case MODE_BREATHE: 
          // selects static color from input values
          selected_mode = 3;
          saveMode(selected_mode, rgb);
          Serial.write(OP_OK);
          break;
        default:
          // no mode selected, or malformed message received
          Serial.write(OP_KO);
          break;
      }
    }
  }
}

void loop() {
  // act accordingly to selected lighting mode
  switch (selected_mode) {
    case MODE_LIGHTSOFF:
      lightsOff();
      break;
     case MODE_RANDOMCOLOR:
      // set led color according to current mode
      effectTable[current_color]();
      // update current mode
      if (!delayLoop(DELAY)) {
        current_color = random(MAX_RAND) % NUM_MODES;
      }
      break;
     case MODE_STATICCOLOR:
      // set specific color
      staticColor((uint8_t)rgb[1], (uint8_t)rgb[2], (uint8_t)rgb[3]);
      break;
     case MODE_BREATHE:
      // set specific color and update intensity
      if (!delayLoop(BREATHE_DELAY)) {
        if (!BETWEEN(breathe_intensity)) breathe_direction ^= 1;
        if (breathe_direction)
          breathe_intensity++;
        else
          breathe_intensity--;
  
        // update mode
        breathe((uint8_t)rgb[1], (uint8_t)rgb[2], (uint8_t)rgb[3], (uint8_t)breathe_intensity);
      }
      break;
  }
}
