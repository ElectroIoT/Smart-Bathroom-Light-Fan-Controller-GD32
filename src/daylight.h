/**
 * daylight.h - optional daylight sensor filter (pure, no hardware).
 *
 * The sensor is only sampled while the lamp is OFF, so the lamp cannot fool
 * it. After the lamp turns off, the old state is kept for filter_ms before
 * a new reading is accepted. Starts as "dark" (light allowed) so a missing
 * or broken sensor can never block the light.
 */
#ifndef DAYLIGHT_H
#define DAYLIGHT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool bright;        /* filtered state: true = daylight, light suppressed */
    bool cand;
    uint32_t since_ms;
} daylight_t;

void daylight_init(daylight_t *d, uint32_t now_ms);

/** raw_bright: sensor reading. sampling: false while the lamp is ON. */
void daylight_update(daylight_t *d, uint32_t now_ms, bool raw_bright,
                     bool sampling, uint32_t filter_ms);

#endif
