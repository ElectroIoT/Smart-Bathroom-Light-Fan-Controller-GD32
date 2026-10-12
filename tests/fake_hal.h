#ifndef FAKE_HAL_H
#define FAKE_HAL_H

#include <stdbool.h>
#include <stdint.h>
#include "hal.h"

extern uint32_t fake_now;          /* simulated millisecond counter  */
extern bool fake_pir_level;        /* physical PIR pin level         */
extern bool light_pin, fan_pin;    /* physical relay pin levels      */
extern bool light_pin_driven, fan_pin_driven;
extern int  light_writes, fan_writes;
extern int  init_calls;
extern bool ind_state[IND_COUNT];
extern int  ind_toggles[IND_COUNT];
extern bool init_light_off, init_fan_off;

extern bool fake_btn[BTN_COUNT];    /* true = physically pressed        */
extern bool stored_fan_enabled;      /* simulated flash flag (survives reset) */
extern int  store_writes;
extern int  wdt_kicks;

void fake_reset(uint32_t start_time);
/** Simulated power cycle: clears everything except the stored flag. */
/** Set PIR from a logical motion state, honouring PIR_ACTIVE_HIGH. */
void fake_set_motion(bool motion);

#endif
