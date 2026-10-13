/*
 * Daylight option tests (built with -DDAYLIGHT_SENSOR=1, default filter 10 s).
 * fake_daylight: true = bright.
 */
#include <stdio.h>
#include "app.h"
#include "config.h"
#include "daylight.h"
#include "fake_hal.h"

#define ON_LVL   (RELAY_ACTIVE_LOW ? 0 : 1)

static int g_fail, g_checks;
#define CHECK(cond) do { g_checks++; if (!(cond)) { g_fail++; \
    printf("  FAIL line %d: %s\n", __LINE__, #cond); } } while (0)

static bool light_is_on(void) { return light_pin == ON_LVL; }
static bool fan_is_on(void)   { return fan_pin == ON_LVL; }
static void run(uint32_t ms) { while (ms--) { fake_now++; app_step(); } }
static void setup(void) { fake_reset(0); app_init(); run(PIR_WARMUP_MS + 1); }
static void pulse(uint32_t ms)
{
    fake_set_motion(true);  run(ms);
    fake_set_motion(false); run(PIR_DEBOUNCE_MS + 2);
}
static void press(button_id_t b)
{
    fake_btn[b] = true;  run(BUTTON_DEBOUNCE_MS + 5);
    fake_btn[b] = false; run(BUTTON_DEBOUNCE_MS + 5);
    run(BUTTON_CHORD_WINDOW_MS + 10);        /* a single press is accepted after the chord window */
}
#define MIN(n) ((uint32_t)(n) * 60000UL)

static void t_filter(void)
{
    printf("daylight filter: dark by default, glitches ignored, lamp-on frozen\n");
    daylight_t d;
    daylight_init(&d, 0);
    CHECK(!d.bright);                                      /* unknown = dark */
    daylight_update(&d, 1000, true, true, 10000);  CHECK(!d.bright);
    daylight_update(&d, 5000, true, true, 10000);  CHECK(!d.bright);
    daylight_update(&d, 11000, true, true, 10000); CHECK(d.bright);   /* stable 10 s */
    daylight_update(&d, 12000, false, true, 10000);
    daylight_update(&d, 13000, true, true, 10000);                    /* glitch */
    daylight_update(&d, 30000, true, true, 10000); CHECK(d.bright);
    /* while sampling is off the state is kept and the timer restarts */
    daylight_update(&d, 40000, false, false, 10000);
    daylight_update(&d, 45000, false, true, 10000); CHECK(d.bright);
    daylight_update(&d, 55500, false, true, 10000); CHECK(!d.bright);
}

static void t_dark_light_works(void)
{
    printf("dark: visit switches the light on\n");
    setup();
    fake_daylight = false;
    run(30000);
    pulse(500);
    CHECK(light_is_on());
}

static void t_bright_blocks_light_not_fan(void)
{
    printf("bright: light stays off, fan still starts\n");
    setup();
    fake_daylight = true;
    run(DAYLIGHT_FILTER_MS + 2000);                        /* sensor settled: bright */
    pulse(500);
    CHECK(!light_is_on());
    CHECK(!ind_state[IND_LIGHT]);
    run(FAN_START_DELAY_MS + 2000);
    CHECK(fan_is_on() && !light_is_on());                  /* fan unaffected */
    run(FAN_TIMEOUT_MS + 2000);
    CHECK(!fan_is_on());
}

static void t_short_glitch_ignored(void)
{
    printf("short bright glitch does not block the light\n");
    setup();
    fake_daylight = true;
    run(DAYLIGHT_FILTER_MS - 3000);
    fake_daylight = false;
    run(1000);
    pulse(500);
    CHECK(light_is_on());
}

static void t_override_always_works(void)
{
    printf("override button turns the light on even in daylight\n");
    setup();
    fake_daylight = true;
    run(DAYLIGHT_FILTER_MS + 2000);
    press(BTN_LIGHT);
    CHECK(light_is_on());
    press(BTN_LIGHT);
    CHECK(!light_is_on());
}

static void t_visit_consistent(void)
{
    printf("one decision per visit, next visit re-evaluates\n");
    setup();
    fake_daylight = true;
    run(DAYLIGHT_FILTER_MS + 2000);
    pulse(500);
    CHECK(!light_is_on());
    fake_daylight = false;                                 /* sun goes behind a cloud */
    run(DAYLIGHT_FILTER_MS + 2000);
    pulse(500);                                            /* same visit */
    CHECK(!light_is_on());
    run(LIGHT_TIMEOUT_MS + 2000);                          /* visit over */
    run(DAYLIGHT_FILTER_MS + 2000);
    pulse(500);                                            /* next visit, now dark */
    CHECK(light_is_on());
}

static void t_lamp_cannot_fool_sensor(void)
{
    printf("lamp light is ignored while the lamp is on\n");
    setup();
    fake_daylight = false;
    run(DAYLIGHT_FILTER_MS + 2000);
    pulse(500);
    CHECK(light_is_on());
    fake_daylight = true;                                  /* lamp shines on the sensor */
    run(LIGHT_TIMEOUT_MS - 5000);
    CHECK(light_is_on());                                  /* nothing changes */
    run(10000);                                            /* lamp off now */
    CHECK(!light_is_on());
    fake_daylight = false;                                 /* lamp light gone */
    run(2000);
    fake_daylight = true;                                  /* afterglow glitch */
    pulse(500);
    CHECK(light_is_on());                                  /* not yet accepted as daylight */
}

int main(void)
{
    t_filter();
    t_dark_light_works();
    t_bright_blocks_light_not_fan();
    t_short_glitch_ignored();
    t_override_always_works();
    t_visit_consistent();
    t_lamp_cannot_fool_sensor();
    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
