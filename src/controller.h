/**
 * controller.h - hardware-independent control logic.
 *
 * Pure C, no hardware access, no globals: time, PIR level and button events
 * are passed in, so it is fully unit-testable on a PC.
 *
 * Features: two independent timers (light / fan), PIR debounce + warm-up +
 * stuck-HIGH fault, warning blink before light-off, fan start delay,
 * manual light override, fan enable/disable.
 *
 * NOTE: a PIR cannot reliably detect a completely stationary person. This
 * logic only extends the timers when the PIR reports motion; it cannot
 * guarantee occupancy detection.
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t light_timeout_ms;
    uint32_t fan_timeout_ms;
    uint32_t warmup_ms;
    uint32_t debounce_ms;
    uint32_t stuck_high_ms;
    uint32_t warn_before_ms;      /* warning blink this long before light-off; 0 = off */
    uint32_t warn_blink_ms;       /* length of the light-off blink                    */
    uint32_t fan_start_delay_ms;  /* fan starts this long after light came ON; 0 = now */
    uint32_t override_ms;         /* manual light override duration                   */
    bool     fan_enabled;         /* restored from storage by the caller              */
    uint32_t humid_max_run_ms;    /* humidity may hold the fan at most this long; 0 = no limit */
    bool     humid_autostart;     /* humidity alone may start the fan                 */
} ctrl_cfg_t;

typedef struct {
    ctrl_cfg_t cfg;

    /* outputs actually driven (logical: true = relay ON) */
    bool light_out;
    bool fan_out;

    /* internal timer state */
    bool light_on;            /* light countdown running                  */
    bool fan_on;              /* fan countdown running                    */
    bool fan_armed;           /* motion seen, fan waiting for start delay */
    bool light_warn;          /* warning blink: light briefly OFF         */
    bool warned;              /* warning already issued this countdown    */
    bool override_active;     /* manual light override running            */
    bool fan_enabled;
    bool humid;               /* humidity input (set by the caller)        */
    bool humid_expired;       /* humidity held the fan for the max time    */
    bool fan_by_humid;        /* fan was started by humidity only          */
    uint32_t humid_since_ms;

    uint32_t start_ms;
    uint32_t last_motion_ms;
    uint32_t light_on_since_ms;
    uint32_t warn_start_ms;
    uint32_t override_start_ms;
    uint32_t high_since_ms;
    uint32_t cand_since_ms;
    bool     pir_stable;      /* debounced PIR state (true = motion) */
    bool     pir_cand;
    bool     fault_stuck;     /* PIR stuck active - ignored          */
} ctrl_t;

/** Initialise with both outputs OFF. Zero timeouts are clamped to 1 ms. */
void ctrl_init(ctrl_t *c, uint32_t now_ms, const ctrl_cfg_t *cfg);

/**
 * Call every main-loop pass. now_ms is a free-running 32-bit millisecond
 * counter (wrap-around safe). pir_active is the logical PIR state
 * (true = motion), i.e. polarity already applied.
 */
void ctrl_update(ctrl_t *c, uint32_t now_ms, bool pir_active);

/** Optional humidity input. true = air is humid; the fan is held on / started. */
void ctrl_set_humid(ctrl_t *c, uint32_t now_ms, bool humid);

/** Button: toggle the manual light override (ON for cfg.override_ms). */
void ctrl_light_override_toggle(ctrl_t *c, uint32_t now_ms);

/** Button: enable/disable the fan. Disabling turns the fan off at once. */
void ctrl_set_fan_enabled(ctrl_t *c, bool enabled);

#endif /* CONTROLLER_H */
