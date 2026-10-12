#ifndef FAKE_HAL_H
#define FAKE_HAL_H

#include <stdbool.h>
#include <stdint.h>

extern uint32_t fake_now;          /* simulated millisecond counter  */
extern bool fake_pir_level;        /* physical PIR pin level         */
extern bool light_pin, fan_pin;    /* physical relay pin levels      */
extern bool light_pin_driven, fan_pin_driven;
extern int  light_writes, fan_writes;
extern int  init_calls;
extern int  led_toggles;
extern bool init_light_off, init_fan_off;

void fake_reset(uint32_t start_time);
/** Set PIR from a logical motion state, honouring PIR_ACTIVE_HIGH. */
void fake_set_motion(bool motion);

#endif
