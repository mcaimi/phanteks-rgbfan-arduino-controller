// global definitions
// number of leds in the strip
#define NUM_LEDS 12
// number of random color modes supported
#define NUM_MODES 4
// controller pin on the arduino board
#define DATA_PIN 6
// program delay value, before changing effect function (5secs)
#define DELAY 5000
// arduino max random value for random() function
#define MAX_RAND 65535
// eeprom addresses for settings
#define MODE_ADDR 0x00
#define R_ADDR 0x01
#define G_ADDR 0x02
#define B_ADDR 0x03
// define serial timeout (ms)
#define SERIAL_TIMEOUT  1000

// running modes
#define MODE_LIGHTSOFF 0x00
#define MODE_RANDOMCOLOR 0x01
#define MODE_STATICCOLOR 0x02
