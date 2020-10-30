// global definitions
// number of leds in the strip
#define NUM_LEDS 12
// number of random color modes supported
#define NUM_MODES 4
// controller pin on the arduino board
#define DATA_PIN 6
// program delay value, before changing effect function (5secs)
#define DELAY 5000
// breathe effect timings
#define BREATHE_STEPS 255
#define BREATHE_DELAY DELAY/BREATHE_STEPS
// arduino max random value for random() function
#define MAX_RAND 65535
// eeprom addresses for settings
#define MODE_ADDR 0x00
#define R_ADDR 0x01
#define G_ADDR 0x02
#define B_ADDR 0x03
// define serial timeout (ms)
#define SERIAL_TIMEOUT  1000
// serial speed
#define SERIAL_SPEED  9600
#define SERIAL_MODE   SERIAL_8N1 // 8-bit, no parity one stop bit

// running modes
#define MODE_LIGHTSOFF 0x00
#define MODE_RANDOMCOLOR 0x01
#define MODE_STATICCOLOR 0x02
#define MODE_BREATHE 0x03

// op return
#define OP_OK 0x00
#define OP_KO 0x01
