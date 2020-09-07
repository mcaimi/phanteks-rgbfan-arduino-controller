//generic utility functions
#include <Arduino.h>
#include "globals.h"

// time tracking since board boot in ms
static unsigned long timeSinceBoot = 0;

// return 0 only if time since boot is multiple of delay_msec variable
int delayLoop(uint16_t delay_msec) {
  timeSinceBoot = millis();
  return ((timeSinceBoot % delay_msec) > 0);
}
