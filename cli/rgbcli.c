/*
   Command line interface for the Phanteks RGB fan Controller
   Firmware code runs on an Arduino UNO microcontroller
   v0.1
*/

#include "rgbcli.h"

// static payloads
static const payload lightsoff_payload = { .command = LIGHTS_OFF, .r = 0x00, .g = 0x00, .b = 0x00 };
static const payload randomcolor_payload = { .command = RANDOM_COLOR, .r = 0x00, .g = 0x00, .b = 0x00 };
// variable payload, rgb values can be set via CLI switches
static payload staticcolor_payload = { .command = STATIC_COLOR, .r = 0x00, .g = 0x00, .b = 0x00 };

// operation outcome
int outcome = 0;

// serial device file descriptor
int serial_descriptor;
// getopt related
int opt, mode_flag = 0;
unsigned int r, g, b;
char * device_name = NULL;

// setup serial device
void setup_serial_device(int serial_descriptor) {
  // serial settings
  struct termios serial_settings;
  // descriptor settings
  struct stat descriptor_stats;

  // try to get exclusive access
  if (flock(serial_descriptor, LOCK_EX|LOCK_NB) != 0) {
    close(serial_descriptor);
    printf("RGBCLI: Cannot acquire exclusive lock on device.\n");
    exit(EXIT_FAILURE);
  }

  // read settings
  if (tcgetattr(serial_descriptor, &serial_settings) != 0) {
    close (serial_descriptor);
    printf("RGBCLI: Cannot read flags from serial port descriptor.\n");
    exit(EXIT_FAILURE);
  }

  // get file descriptor settings
  if (fstat(serial_descriptor, &descriptor_stats) != 0) {
    close(serial_descriptor);
    printf("RGBCLI: fstat() failed on serial_descriptor.\n");
    exit(EXIT_FAILURE);
  }

  // set serial flags
  // 9600 baud, 8 data bits, 1 stop bit, no parity
  serial_settings.c_cflag &= ~(PARENB | CSTOPB);
  serial_settings.c_cflag |= (CS8 | CREAD | CLOCAL);
  serial_settings.c_oflag &= ~(OPOST | ONLCR);

  // set baud rate
  cfsetospeed(&serial_settings, B9600);

  // setup line settings
  if (tcsetattr(serial_descriptor, TCSANOW, &serial_settings) != 0) {
    close(serial_descriptor);
    printf("RGBCLI: Cannot set flags on the serial port descriptor.\n");
    exit(EXIT_FAILURE);
  }
}

// main function
int main(int argc, char **argv) {
  // parse commandline
  while ((opt = getopt(argc, argv, "m:d:r:g:b:")) != -1) {
    switch (opt) {
      case 'm':
        mode_flag = atoi(optarg);
        break;
      case 'd':
        device_name = optarg;
        break;
      case 'r':
        r = atoi(optarg);
        break;
      case 'g':
        g = atoi(optarg);
        break;
      case 'b':
        b = atoi(optarg);
        break;
      default:
        printf("RGBCLI: Syntax Error.");
        exit(EXIT_FAILURE);
    }
  }

  // CLI sanity check
  if ISNULL(device_name) {
    printf("RGBCLI: Device Name must be specified.\n");
    exit(EXIT_FAILURE);
  }

  // component sanity check
  if (mode_flag == STATIC_COLOR) {
      if (!COMPONENT_OK(r) || !COMPONENT_OK(g) || !COMPONENT_OK(b)) {
        printf("RGBCLI: RGB Components must be bounded between 0 and 255\n");
        exit(EXIT_FAILURE);
      }
  }

  // open serial device
  if ((serial_descriptor = open(device_name, O_WRONLY|O_NOCTTY)) < 0) {
      printf("RGBCLI: Cannot Open %s in RW mode.\n", device_name);
      exit(EXIT_FAILURE);
  } else {
    setup_serial_device(serial_descriptor);
  }

  // send payload to arduino controller
  switch (mode_flag) {
    case LIGHTS_OFF:
      printf("RGBCLI: Using Device [%s], operating mode: LIGHTS_OFF...\n", device_name);
      write(serial_descriptor, lightsoff_payload._bytes, sizeof(lightsoff_payload._bytes));
      outcome = 1;
      break;
    case RANDOM_COLOR:
      printf("RGBCLI: Using Device [%s], operating mode: RANDOM_COLOR...\n", device_name);
      write(serial_descriptor, randomcolor_payload._bytes, sizeof(randomcolor_payload._bytes));
      outcome = 1;
      break;
    case STATIC_COLOR:
      printf("RGBCLI: Using Device [%s], operating mode: STATIC_COLOR (r:%d, g:%d, b:%d)...\n", device_name, r, g, b);
      staticcolor_payload.command = STATIC_COLOR;
      staticcolor_payload.r = (unsigned char)r;
      staticcolor_payload.g = (unsigned char)g;
      staticcolor_payload.b = (unsigned char)b;
      write(serial_descriptor, staticcolor_payload._bytes, sizeof(staticcolor_payload._bytes));
      outcome = 1;
      break;
    default:
      printf("RGBCLI: Unknown mode\n");
  }

  // unlock descriptor
  flock(serial_descriptor, LOCK_UN);
  // close serial device
  close(serial_descriptor);
  if (outcome == 0) {
    exit(EXIT_FAILURE);
  } else exit(EXIT_SUCCESS);
}

