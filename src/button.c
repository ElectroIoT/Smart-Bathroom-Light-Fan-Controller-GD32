#include "button.h"

void button_init(button_t *b, uint32_t now_ms, bool pressed_now)
{
    b->stable = b->cand = pressed_now;
    b->since_ms = now_ms;
}

bool button_update(button_t *b, uint32_t now_ms, bool pressed_now, uint32_t debounce_ms)
{
    if (pressed_now == b->stable) {
        b->cand = pressed_now;
        return false;
    }
    if (pressed_now != b->cand) {
        b->cand = pressed_now;
        b->since_ms = now_ms;
    }
    if ((uint32_t)(now_ms - b->since_ms) >= debounce_ms) {
        b->stable = pressed_now;
        return pressed_now;           /* event only on press, not release */
    }
    return false;
}
