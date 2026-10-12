#include "controller.h"

/* Elapsed time that stays correct across a 32-bit counter wrap. */
static uint32_t elapsed(uint32_t now, uint32_t then)
{
    return (uint32_t)(now - then);
}

void ctrl_init(ctrl_t *c, uint32_t now_ms,
               uint32_t light_timeout_ms, uint32_t fan_timeout_ms,
               uint32_t warmup_ms, uint32_t debounce_ms,
               uint32_t stuck_high_ms)
{
    c->light_timeout_ms = light_timeout_ms ? light_timeout_ms : 1u;
    c->fan_timeout_ms   = fan_timeout_ms   ? fan_timeout_ms   : 1u;
    c->warmup_ms        = warmup_ms;
    c->debounce_ms      = debounce_ms;
    c->stuck_high_ms    = stuck_high_ms;

    c->light_on = false;
    c->fan_on   = false;

    c->start_ms       = now_ms;
    c->last_motion_ms = now_ms;
    c->high_since_ms  = now_ms;
    c->cand_since_ms  = now_ms;
    c->pir_stable     = false;
    c->pir_cand       = false;
    c->fault_stuck    = false;
}

/* Debounce the raw level. Returns true when the stable state just changed. */
static bool debounce(ctrl_t *c, uint32_t now, bool raw)
{
    if (raw == c->pir_stable) {
        c->pir_cand = raw;            /* back to stable level: drop candidate */
        return false;
    }
    if (raw != c->pir_cand) {         /* new candidate level: start timing */
        c->pir_cand = raw;
        c->cand_since_ms = now;
    }
    if (elapsed(now, c->cand_since_ms) >= c->debounce_ms) {
        c->pir_stable = raw;
        return true;
    }
    return false;
}

void ctrl_update(ctrl_t *c, uint32_t now, bool pir_active)
{
    bool changed;

    /* Warm-up: PIR output is meaningless right after power-up. */
    if (elapsed(now, c->start_ms) < c->warmup_ms) {
        c->light_on = false;
        c->fan_on   = false;
        return;
    }

    changed = debounce(c, now, pir_active);

    if (changed) {
        if (c->pir_stable) {
            /* Rising edge = one motion event. */
            c->high_since_ms = now;
            if (!c->fault_stuck) {
                c->last_motion_ms = now;
                c->light_on = true;
                c->fan_on   = true;
            }
        } else {
            /* Falling edge: motion was present until now; clear any fault. */
            c->fault_stuck = false;
            c->last_motion_ms = now;
        }
    }

    /* PIR continuously active = motion still ongoing: hold timers, no
     * per-pass restart. A stuck-high sensor is released after a limit. */
    bool hold = c->pir_stable && !c->fault_stuck;
    if (hold && elapsed(now, c->high_since_ms) >= c->stuck_high_ms) {
        c->fault_stuck = true;
        c->last_motion_ms = now;      /* normal timeouts start counting here */
        hold = false;
    }

    if (!hold) {
        if (c->light_on && elapsed(now, c->last_motion_ms) >= c->light_timeout_ms)
            c->light_on = false;
        if (c->fan_on && elapsed(now, c->last_motion_ms) >= c->fan_timeout_ms)
            c->fan_on = false;
    }
}
