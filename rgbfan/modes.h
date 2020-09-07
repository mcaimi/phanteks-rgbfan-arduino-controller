// LED Effect Modes
// RGB helpers
// Unpack 32-bit color
#define R(color) ((color >> 16) & 0xFF)
#define G(color) ((color >> 8) & 0xFF)
#define B(color) color & 0xFF

// pack 32-bit color
uint32_t pack_rgb(uint8_t, uint8_t, uint8_t);

// set all leds to the same color
void solidColor(uint8_t, uint8_t, uint8_t, Adafruit_NeoPixel*);

// utilities
void setupMode(uint8_t * mode_setting, byte * initial_color_array);
void saveMode(uint8_t mode_setting, byte * settings);

// solid color modes
void brightWhite();
void brightRed();
void brightGreen();
void brightBlue();

// static color
void staticColor(uint8_t r, uint8_t g, uint8_t b);

// shuts off leds
void lightsOff();
