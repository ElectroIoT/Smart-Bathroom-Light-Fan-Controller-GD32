/**
 * hal.h - the only interface between application logic and hardware.
 * Implemented by src/hal_gd32f103.c on target and tests/fake_hal.c on a PC.
 * All levels here are PHYSICAL pin levels (true = pin HIGH).
 */
#ifndef HAL_H
#define HAL_H

#include <stdbool.h>
#include <stdint.h>

/** Drive relay pins to their OFF level FIRST, then enable outputs. */
void hal_init(bool light_off_level, bool fan_off_level);
uint32_t hal_millis(void);          /* free-running, wraps at 2^32 */
bool hal_pir_level(void);           /* physical level of PIR pin   */
void hal_light_write(bool level);
void hal_fan_write(bool level);
void hal_debug(const char *msg);    /* no-op when debug is disabled */

#endif
