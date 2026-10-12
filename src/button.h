/** button.h - debounced push-button with one event per press. Pure logic. */
#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool stable;        /* debounced state (true = pressed) */
    bool cand;
    uint32_t since_ms;
} button_t;

/** Start from the current reading, so a button held at boot gives no event. */
void button_init(button_t *b, uint32_t now_ms, bool pressed_now);

/** Returns true exactly once per press (on the debounced press edge). */
bool button_update(button_t *b, uint32_t now_ms, bool pressed_now, uint32_t debounce_ms);

#endif
