/*
 * Bare-LDR (ADC) option tests, built with -DDAYLIGHT_SENSOR=2.
 * fake_ldr_adc simulates the ADC reading 0..4095 (higher = brighter).
 * Defaults: bright at 2000 or more, dark at 1200 or less, filter 10 s.
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

static void t_threshold(void)
{
    printf("ADC threshold with hysteresis\n");
    CHECK(!daylight_threshold(false, 1500, 2000, 1200));   /* in the gap: keep dark   */
    CHECK(daylight_threshold(true, 1500, 2000, 1200));     /* in the gap: keep bright */
    CHECK(daylight_threshold(false, 2000, 2000, 1200));    /* at the bright limit     */
    CHECK(!daylight_threshold(true, 1200, 2000, 1200));    /* at the dark limit       */
    CHECK(!daylight_threshold(true, 0, 2000, 1200));       /* 0 (unplugged) = dark    */
}

static void t_dark_adc_light_works(void)
{
    printf("dark ADC: light works\n");
    setup();
    fake_ldr_adc = 300;
    run(DAYLIGHT_FILTER_MS + 2000);
    pulse(500);
    CHECK(light_is_on());
}

static void t_bright_adc_blocks_light_not_fan(void)
{
    printf("bright ADC: light stays off, fan runs\n");
    setup();
    fake_ldr_adc = 3500;
    run(DAYLIGHT_FILTER_MS + 2000);
    pulse(500);
    CHECK(!light_is_on());
    run(FAN_START_DELAY_MS + 2000);
    CHECK(fan_is_on() && !light_is_on());
}

static void t_gap_keeps_state(void)
{
    printf("reading in the hysteresis gap keeps the last decision\n");
    setup();
    fake_ldr_adc = 3500;
    run(DAYLIGHT_FILTER_MS + 2000);
    fake_ldr_adc = 1600;                                   /* between 1200 and 2000 */
    run(DAYLIGHT_FILTER_MS + 2000);
    pulse(500);
    CHECK(!light_is_on());                                 /* still bright */
}

static void t_unplugged_reads_dark(void)
{
    printf("sensor unplugged (ADC 0): light still works\n");
    setup();
    fake_ldr_adc = 3500;
    run(DAYLIGHT_FILTER_MS + 2000);
    fake_ldr_adc = 0;                                      /* wire falls off */
    run(DAYLIGHT_FILTER_MS + 2000);
    pulse(500);
    CHECK(light_is_on());
}

int main(void)
{
    t_threshold();
    t_dark_adc_light_works();
    t_bright_adc_blocks_light_not_fan();
    t_gap_keeps_state();
    t_unplugged_reads_dark();
    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
