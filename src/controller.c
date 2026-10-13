#include "controller.h"

/* Elapsed time that stays correct across a 32-bit counter wrap. */
static uint32_t elapsed(uint32_t now, uint32_t then)
{
    return (uint32_t)(now - then);
}

void ctrl_init(ctrl_t *c, uint32_t now_ms, const ctrl_cfg_t *cfg)
{
    c->cfg = *cfg;
    if (!c->cfg.light_timeout_ms) c->cfg.light_timeout_ms = 1u;
    if (!c->cfg.fan_timeout_ms)   c->cfg.fan_timeout_ms   = 1u;

    c->light_out = c->fan_out = false;
    c->light_on = c->fan_on = false;
    c->fan_armed = c->light_warn = c->warned = c->override_active = false;
    c->fan_enabled = cfg->fan_enabled;
    c->humid = c->humid_expired = c->fan_by_humid = false;
    c->daylight_bright = c->light_suppressed = false;
    c->humid_since_ms = now_ms;

    c->start_ms = c->last_motion_ms = c->light_on_since_ms = now_ms;
    c->warn_start_ms = c->override_start_ms = now_ms;
    c->high_since_ms = c->cand_since_ms = now_ms;
    c->pir_stable = c->pir_cand = false;
    c->fault_stuck = false;
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
    if (elapsed(now, c->cand_since_ms) >= c->cfg.debounce_ms) {
        c->pir_stable = raw;
        return true;
    }
    return false;
}

static void motion_event(ctrl_t *c, uint32_t now)
{
    c->last_motion_ms = now;
    c->fan_by_humid = false;          /* a person is here: normal fan timer */
    c->warned = false;                /* new countdown, new warning */
    c->light_warn = false;
    if (!c->light_on) {
        c->light_on = true;
        c->light_on_since_ms = now;
        /* daylight decides once per visit; the fan timer still runs */
        c->light_suppressed = c->cfg.daylight_enabled && c->daylight_bright;
    }
    if (c->fan_enabled && !c->fan_on)
        c->fan_armed = true;          /* fan starts after the start delay */
}

static void pir_logic(ctrl_t *c, uint32_t now, bool pir_active)
{
    bool changed = debounce(c, now, pir_active);

    if (changed) {
        if (c->pir_stable) {
            c->high_since_ms = now;   /* rising edge = one motion event */
            if (!c->fault_stuck)
                motion_event(c, now);
        } else {
            c->fault_stuck = false;   /* falling edge: clear any fault   */
            c->last_motion_ms = now;  /* motion was present until now    */
            c->warned = false;
        }
    }

    /* PIR continuously active = motion ongoing: hold timers (no per-pass
     * restart). A stuck-HIGH sensor is released after a limit. */
    bool hold = c->pir_stable && !c->fault_stuck;
    if (hold && elapsed(now, c->high_since_ms) >= c->cfg.stuck_high_ms) {
        c->fault_stuck = true;
        c->last_motion_ms = now;      /* normal timeouts start counting here */
        c->warned = false;
        hold = false;
    }

    if (!hold) {
        uint32_t since = elapsed(now, c->last_motion_ms);

        if (c->light_on) {
            uint32_t lead = c->cfg.warn_before_ms;
            /* warning blink: only if the timeout is long enough to hold it */
            if (lead && !c->warned &&
                c->cfg.light_timeout_ms > lead + c->cfg.warn_blink_ms &&
                since >= c->cfg.light_timeout_ms - lead) {
                c->warned = true;
                c->warn_start_ms = now;
            }
            if (since >= c->cfg.light_timeout_ms) {
                c->light_on = false;
                c->light_suppressed = false;
                c->warned = false;
            }
        }
        bool humid_hold = c->humid && !c->humid_expired && c->fan_enabled;
        if (c->fan_on && !humid_hold &&
            (since >= c->cfg.fan_timeout_ms || c->fan_by_humid)) {
            c->fan_on = false;    /* timer done, or humidity-only run ended */
            c->fan_by_humid = false;
        }
    } else {
        c->warned = false;            /* PIR active: no warning pending */
    }

    c->light_warn = c->light_on && c->warned &&
                    elapsed(now, c->warn_start_ms) < c->cfg.warn_blink_ms;

    /* fan start delay: counted from the moment the light came ON */
    if (!c->light_on) {
        c->fan_armed = false;
    } else if (c->fan_armed && !c->fan_on && c->fan_enabled &&
               elapsed(now, c->light_on_since_ms) >= c->cfg.fan_start_delay_ms) {
        c->fan_on = true;
        c->fan_armed = false;
    }
}

void ctrl_update(ctrl_t *c, uint32_t now, bool pir_active)
{
    /* Warm-up: PIR output is meaningless right after power-up. */
    if (elapsed(now, c->start_ms) < c->cfg.warmup_ms) {
        c->light_on = c->fan_on = c->fan_armed = false;
        c->light_warn = false;
        c->light_suppressed = false;
    } else {
        pir_logic(c, now, pir_active);
    }

    /* humidity may hold / start the fan, but only for a limited time */
    if (c->humid && !c->humid_expired && c->cfg.humid_max_run_ms &&
        elapsed(now, c->humid_since_ms) >= c->cfg.humid_max_run_ms)
        c->humid_expired = true;
    if (c->cfg.humid_autostart && c->humid && !c->humid_expired &&
        c->fan_enabled && !c->fan_on &&
        elapsed(now, c->start_ms) >= c->cfg.warmup_ms) {
        c->fan_on = true;
        c->fan_by_humid = true;
    }

    if (c->override_active &&
        elapsed(now, c->override_start_ms) >= c->cfg.override_ms)
        c->override_active = false;

    c->light_out = (c->light_on && !c->light_warn && !c->light_suppressed) ||
                   c->override_active;
    c->fan_out   = c->fan_on && c->fan_enabled;
}

void ctrl_set_daylight(ctrl_t *c, bool bright)
{
    c->daylight_bright = bright;
}

void ctrl_set_humid(ctrl_t *c, uint32_t now, bool humid)
{
    if (humid && !c->humid) {
        c->humid_since_ms = now;
        c->humid_expired = false;
    }
    if (!humid)
        c->humid_expired = false;
    c->humid = humid;
}

void ctrl_light_override_toggle(ctrl_t *c, uint32_t now)
{
    c->override_active = !c->override_active;
    c->override_start_ms = now;
    c->light_out = (c->light_on && !c->light_warn && !c->light_suppressed) ||
                   c->override_active;
}

void ctrl_set_fan_enabled(ctrl_t *c, bool enabled)
{
    c->fan_enabled = enabled;
    if (!enabled) {
        c->fan_on = false;
        c->fan_armed = false;
        c->fan_by_humid = false;
    }
    c->fan_out = c->fan_on && c->fan_enabled;
}
