/*
   Command line interface for the Phanteks RGB fan Controller
   Firmware code runs on an Arduino UNO microcontroller
   v0.1
*/

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>

// serial commands
#define LIGHTS_OFF 0x00
#define RANDOM_COLOR 0x01
#define STATIC_COLOR 0x02

// macros
#define COMPONENT_OK(k) ((k >= 0) && (k <=255))
#define ISNULL(k) (k == NULL)

// data types
typedef union {
  struct {
    unsigned char command;
    unsigned char r;
    unsigned char g;
    unsigned char b;
  };
  unsigned char _bytes[4];
} payload;


