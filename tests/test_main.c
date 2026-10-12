/*
 * Host-side unit tests. Build/run with: python tools/build.py test
 * Time is simulated one millisecond per step, so the real 7/10 minute
 * defaults are tested without waiting. Rebuilt with different -D options
 * to cover relay and PIR polarity (see tools/build.py).
 */
#include <stdio.h>
#include "app.h"
#include "config.h"
#include "fake_hal.h"

#define LIGHT_ON_LVL  (RELAY_ACTIVE_LOW ? 0 : 1)
#define LIGHT_OFF_LVL (RELAY_ACTIVE_LOW ? 1 : 0)

static int g_fail, g_checks;

#define CHECK(cond) do { g_checks++; if (!(cond)) { g_fail++; \
    printf("  FAIL line %d: %s\n", __LINE__, #cond); } } while (0)

static bool light_is_on(void) { return light_pin == LIGHT_ON_LVL; }
static bool fan_is_on(void)   { return fan_pin == LIGHT_ON_LVL; }

static void run(uint32_t ms) { while (ms--) { fake_now++; app_step(); } }

/* Fresh system, warm-up finished, no motion. */
static void setup_at(uint32_t start)
{
    fake_reset(start);
    app_init();
    run(PIR_WARMUP_MS + 1);
}
static void setup(void) { setup_at(0); }

/* One motion pulse: PIR active for pulse_ms then idle. */
static void pulse(uint32_t pulse_ms)
{
    fake_set_motion(true);  run(pulse_ms);
    fake_set_motion(false);
}

#define MIN(n) ((uint32_t)(n) * 60000UL)

static void t_startup_no_motion(void)
{
    printf("startup, no motion\n");
    fake_reset(0);
    app_init();
    CHECK(init_calls == 1);
    CHECK(init_light_off == LIGHT_OFF_LVL && init_fan_off == LIGHT_OFF_LVL);
    CHECK(light_pin_driven && fan_pin_driven);
    CHECK(!light_is_on() && !fan_is_on());
    run(PIR_WARMUP_MS + MIN(20));
    CHECK(!light_is_on() && !fan_is_on());
    CHECK(light_writes == 0 && fan_writes == 0);
}

static void t_warmup_ignores_pir(void)
{
    printf("PIR ignored during warm-up\n");
    fake_reset(0);
    app_init();
    fake_set_motion(true);
    run(PIR_WARMUP_MS > 10 ? PIR_WARMUP_MS - 10 : 0);
    CHECK(!light_is_on() && !fan_is_on());
}

static void t_motion_turns_on(void)
{
    printf("motion turns both on\n");
    setup();
    pulse(500);
    CHECK(light_is_on() && fan_is_on());
}

static void t_glitch_ignored(void)
{
    printf("glitch shorter than debounce ignored\n");
    setup();
    if (PIR_DEBOUNCE_MS > 1) {
        pulse(PIR_DEBOUNCE_MS - 1);
        run(10);
        CHECK(!light_is_on() && !fan_is_on());
    }
}

static void t_timeouts(void)
{
    printf("light 7 min, fan 10 min, independent\n");
    setup();
    pulse(500);                          /* falling edge ~ t0 */
    run(LIGHT_TIMEOUT_MS - 600);         /* just before light timeout */
    CHECK(light_is_on() && fan_is_on());
    run(700);                            /* now past it (+ debounce slack) */
    CHECK(!light_is_on());               /* light OFF ...                */
    CHECK(fan_is_on());                  /* ... fan NOT switched off     */
    run(FAN_TIMEOUT_MS - LIGHT_TIMEOUT_MS - 1000);
    CHECK(fan_is_on());
    run(1500);
    CHECK(!fan_is_on());
    CHECK(!light_is_on());
}

static void t_exact_boundary(void)
{
    printf("exact timeout boundary\n");
    setup();
    pulse(PIR_DEBOUNCE_MS + 5);
    /* PIR fell at the call that crossed debounce; measure from relay state. */
    uint32_t guard = 0;
    while (light_is_on() && guard < LIGHT_TIMEOUT_MS + 5000) { run(1); guard++; }
    /* Elapsed since the falling edge must equal LIGHT_TIMEOUT_MS +/- debounce */
    CHECK(guard >= LIGHT_TIMEOUT_MS - 5);
    CHECK(guard <= LIGHT_TIMEOUT_MS + PIR_DEBOUNCE_MS + 5);
}

static void t_repeated_motion_resets(void)
{
    printf("repeated motion restarts both timers (spec example)\n");
    setup();
    pulse(500);
    run(MIN(4));
    pulse(500);                          /* motion at ~4:00 */
    run(MIN(6) + 30000);                 /* ~10:30 total: 6:30 after 2nd */
    CHECK(light_is_on());                /* would be off at 7:00 if not reset */
    run(MIN(1));                         /* ~11:30 => light off (7 min after 4:00) */
    CHECK(!light_is_on());
    CHECK(fan_is_on());                  /* fan until ~14:00 */
    run(MIN(2));                         /* ~13:30 */
    CHECK(fan_is_on());
    run(MIN(1));                         /* ~14:30 */
    CHECK(!fan_is_on());
}

static void t_motion_during_light_countdown(void)
{
    printf("motion during light countdown restarts light\n");
    setup();
    pulse(500);
    run(MIN(6));
    pulse(500);
    run(MIN(6));                         /* 12 min since first; 6 since 2nd */
    CHECK(light_is_on());
    run(MIN(1) + 2000);
    CHECK(!light_is_on());
}

static void t_motion_during_fan_countdown(void)
{
    printf("motion during fan countdown restarts fan (light re-arms too)\n");
    setup();
    pulse(500);
    run(MIN(8));                         /* light already off, fan still on */
    CHECK(!light_is_on() && fan_is_on());
    pulse(500);
    CHECK(fan_is_on());
    CHECK(light_is_on());                /* any motion turns both on */
    run(MIN(9));
    CHECK(fan_is_on());                  /* 9 min < 10 min */
    run(MIN(1) + 2000);
    CHECK(!fan_is_on());
}

static void t_held_high(void)
{
    printf("PIR held HIGH: one event, no repeated processing\n");
    setup();
    fake_set_motion(true);
    run(MIN(15));                        /* well beyond both timeouts */
    CHECK(light_writes == 1 && fan_writes == 1);   /* switched on once only */
    CHECK(light_is_on() && fan_is_on());           /* held while active     */
    fake_set_motion(false);
    run(LIGHT_TIMEOUT_MS - 1000);        /* timers run from the falling edge */
    CHECK(light_is_on());
    run(PIR_DEBOUNCE_MS + 2000);
    CHECK(!light_is_on());
}

static void t_stuck_high_fault(void)
{
    printf("PIR stuck HIGH: released after limit, ignored until LOW\n");
    setup();
    fake_set_motion(true);
    run(PIR_STUCK_HIGH_MS + 1000);
    CHECK(app_state()->fault_stuck);
    run(FAN_TIMEOUT_MS + 2000);          /* normal timeouts now run */
    CHECK(!light_is_on() && !fan_is_on());
    CHECK(light_writes == 2 && fan_writes == 2);   /* on once, off once */
    fake_set_motion(false);
    run(PIR_DEBOUNCE_MS + 5);
    CHECK(!app_state()->fault_stuck);
    pulse(500);
    CHECK(light_is_on() && fan_is_on());           /* works again */
}

static void t_reset_while_energized(void)
{
    printf("MCU reset while relays energized\n");
    setup();
    pulse(500);
    CHECK(light_is_on() && fan_is_on());
    app_init();                          /* models a reset: init runs again */
    CHECK(init_calls == 2);
    CHECK(!light_is_on() && !fan_is_on());         /* safe OFF */
    fake_set_motion(true);
    run(PIR_WARMUP_MS / 2);
    CHECK(!light_is_on() && !fan_is_on());         /* warm-up applies again */
}

static void t_tick_rollover(void)
{
    printf("millisecond tick rollover\n");
    /* start so the 32-bit counter wraps shortly after the motion event */
    setup_at(0u - (PIR_WARMUP_MS + 120000UL));
    pulse(500);
    CHECK(light_is_on() && fan_is_on());
    CHECK(fake_now > 0u - 1000000UL || fake_now < 1000000UL);
    run(LIGHT_TIMEOUT_MS - 600);
    CHECK(light_is_on());
    run(1500);
    CHECK(!light_is_on() && fan_is_on());
    run(FAN_TIMEOUT_MS - LIGHT_TIMEOUT_MS);
    CHECK(!fan_is_on());
    CHECK(fake_now < 0x80000000UL);      /* confirm the counter really wrapped */
}

static void t_polarity(void)
{
    printf("relay polarity (RELAY_ACTIVE_LOW=%d, PIR_ACTIVE_HIGH=%d)\n",
           RELAY_ACTIVE_LOW, PIR_ACTIVE_HIGH);
    setup();
    CHECK(light_pin == RELAY_ACTIVE_LOW && fan_pin == RELAY_ACTIVE_LOW);   /* OFF level */
    pulse(500);
    CHECK(light_pin == !RELAY_ACTIVE_LOW && fan_pin == !RELAY_ACTIVE_LOW); /* ON level  */
    run(FAN_TIMEOUT_MS + 2000);
    CHECK(light_pin == RELAY_ACTIVE_LOW && fan_pin == RELAY_ACTIVE_LOW);
}

int main(void)
{
    t_startup_no_motion();
    t_warmup_ignores_pir();
    t_motion_turns_on();
    t_glitch_ignored();
    t_timeouts();
    t_exact_boundary();
    t_repeated_motion_resets();
    t_motion_during_light_countdown();
    t_motion_during_fan_countdown();
    t_held_high();
    t_stuck_high_fault();
    t_reset_while_energized();
    t_tick_rollover();
    t_polarity();
    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
