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
/** Indicator LEDs (logical on/off; polarity handled inside the HAL). */
typedef enum { IND_SYSTEM, IND_PIR, IND_LIGHT, IND_FAN, IND_FAULT, IND_FANEN, IND_SYSEXT, IND_COUNT } indicator_t;
void hal_indicator(indicator_t id, bool on);
/** Push buttons: true while physically pressed (polarity handled inside). */
typedef enum { BTN_LIGHT, BTN_FAN, BTN_COUNT } button_id_t;
bool hal_button_pressed(button_id_t id);

/** Persistent settings (flash): fan enabled (default true) and LED silent mode (default false). */
void hal_load_settings(bool *fan_enabled, bool *leds_silent);
void hal_save_settings(bool fan_enabled, bool leds_silent);

/** Daylight sensor: true = bright (polarity handled inside). false if disabled. */
bool hal_daylight_bright(void);

/** Bare LDR: ADC reading 0..4095 (0 if disabled or the conversion timed out = dark). */
uint16_t hal_ldr_read(void);

/** Debug text plus a decimal number and CRLF (no-op when debug is off). */
void hal_debug_value(const char *label, uint32_t value);

/** Read one 5-byte DHT11/DHT22 frame. false = no sensor / timeout / disabled. */
bool hal_humidity_read(uint8_t raw[5]);

/** Feed the watchdog. Call every main-loop pass. */
void hal_watchdog_kick(void);

void hal_debug(const char *msg);    /* no-op when debug is disabled */

#endif
