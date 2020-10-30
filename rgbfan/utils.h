//generic utility functions
#define BETWEEN(X) ((X>0) && (X<BREATHE_STEPS))

// return 1 (true) only if time since boot is multiple of delay_msec
int delayLoop(uint16_t delay_msec);
