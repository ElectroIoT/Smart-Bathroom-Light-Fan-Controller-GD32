/**
 * controller.h - hardware-independent two-timer control logic.
 *
 * Pure C, no hardware access, no globals: time and PIR level are passed in,
 * so it is fully unit-testable on a PC.
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
    /* configuration */
    uint32_t light_timeout_ms;
    uint32_t fan_timeout_ms;
    uint32_t warmup_ms;
    uint32_t debounce_ms;
    uint32_t stuck_high_ms;

    /* outputs (logical: true = ON) */
    bool light_on;
    bool fan_on;

    /* state */
    uint32_t start_ms;        /* tick at init, for warm-up           */
    uint32_t last_motion_ms;  /* tick of most recent motion          */
    uint32_t high_since_ms;   /* tick when debounced PIR went active */
    uint32_t cand_since_ms;   /* tick when the raw level last changed*/
    bool     pir_stable;      /* debounced PIR state (true = motion) */
    bool     pir_cand;        /* raw level being debounced           */
    bool     fault_stuck;     /* PIR stuck active - ignored          */
} ctrl_t;

/** Initialise with both outputs OFF. Timeouts of 0 are clamped to 1 ms. */
void ctrl_init(ctrl_t *c, uint32_t now_ms,
               uint32_t light_timeout_ms, uint32_t fan_timeout_ms,
               uint32_t warmup_ms, uint32_t debounce_ms,
               uint32_t stuck_high_ms);

/**
 * Call every main-loop pass. now_ms is a free-running 32-bit millisecond
 * counter (wrap-around safe). pir_active is the logical PIR state
 * (true = motion), i.e. polarity already applied.
 */
void ctrl_update(ctrl_t *c, uint32_t now_ms, bool pir_active);

#endif /* CONTROLLER_H */
