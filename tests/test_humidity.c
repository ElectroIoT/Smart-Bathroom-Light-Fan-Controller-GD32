/*
 * Humidity option tests (built with -DHUMIDITY_SENSOR=11 and =22).
 * Default thresholds: humid at 70 %, normal at 62 %, max hold 60 min.
 */
#include <stdio.h>
#include "app.h"
#include "config.h"
#include "fake_hal.h"
#include "humidity.h"

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
#define MIN(n) ((uint32_t)(n) * 60000UL)

static void t_decode(void)
{
    printf("DHT frame decode and checksum\n");
    uint8_t rh = 0;
    uint8_t d11[5] = { 65, 0, 25, 0, 90 };
    uint8_t d22[5] = { 0x02, 0x8A, 0x01, 0x0B, 0 };      /* 65.0 % */
    d22[4] = (uint8_t)(d22[0] + d22[1] + d22[2] + d22[3]);
    CHECK(dht_decode(d11, 11, &rh) && rh == 65);
    CHECK(dht_decode(d22, 22, &rh) && rh == 65);
    d11[4] ^= 1;
    CHECK(!dht_decode(d11, 11, &rh));                    /* bad checksum */
}

static void t_hysteresis(void)
{
    printf("humidity hysteresis and sensor failure\n");
    humidity_t h;
    humidity_init(&h);
    humidity_update(&h, true, 50, 70, 62, 3);  CHECK(!h.humid);
    humidity_update(&h, true, 75, 70, 62, 3);  CHECK(h.humid);
    humidity_update(&h, true, 66, 70, 62, 3);  CHECK(h.humid);    /* between: keep */
    humidity_update(&h, true, 60, 70, 62, 3);  CHECK(!h.humid);
    humidity_update(&h, true, 66, 70, 62, 3);  CHECK(!h.humid);   /* between: keep */
    humidity_update(&h, true, 80, 70, 62, 3);  CHECK(h.humid);
    humidity_update(&h, false, 0, 70, 62, 3);
    humidity_update(&h, false, 0, 70, 62, 3);
    CHECK(h.humid && h.valid);                                    /* 2 fails: not yet */
    humidity_update(&h, false, 0, 70, 62, 3);
    CHECK(!h.humid && !h.valid);                                  /* 3rd: invalid */
    humidity_update(&h, true, 50, 70, 62, 3);
    CHECK(h.valid);                                               /* recovers */
}

static void t_polling(void)
{
    printf("sensor polled every HUMIDITY_POLL_MS\n");
    setup();
    humidity_reads = 0;
    run(10 * HUMIDITY_POLL_MS + 10);
    CHECK(humidity_reads >= 9 && humidity_reads <= 11);
}

static void t_humid_starts_fan(void)
{
    printf("high humidity starts the fan by itself, normal humidity stops it\n");
    setup();
    fake_rh_pct = 50;
    run(HUMIDITY_POLL_MS * 2);
    CHECK(!fan_is_on() && !light_is_on());
    fake_rh_pct = 85;
    run(HUMIDITY_POLL_MS + 100);
    CHECK(fan_is_on() && !light_is_on());                /* fan only, no light */
    run(MIN(30));
    CHECK(fan_is_on());                                  /* held while humid */
    fake_rh_pct = 66;                                    /* between thresholds */
    run(HUMIDITY_POLL_MS + 100);
    CHECK(fan_is_on());
    fake_rh_pct = 55;
    run(HUMIDITY_POLL_MS + 100);
    CHECK(!fan_is_on());                                 /* humidity-only run ends */
}

static void t_humid_extends_fan_after_motion(void)
{
    printf("humidity extends the fan run after a visit\n");
    setup();
    pulse(500);
    run(FAN_START_DELAY_MS + 2000);
    CHECK(fan_is_on());
    fake_rh_pct = 90;                                    /* shower steam */
    run(HUMIDITY_POLL_MS + 100);
    run(FAN_TIMEOUT_MS + MIN(5));                        /* past the normal timeout */
    CHECK(fan_is_on());
    fake_rh_pct = 50;
    run(HUMIDITY_POLL_MS + 100);
    CHECK(!fan_is_on());                                 /* then normal timer applies */
}

static void t_humid_disabled_fan(void)
{
    printf("fan disabled by the button: humidity cannot start it\n");
    setup();
    fake_btn[BTN_FAN] = true;  run(BUTTON_DEBOUNCE_MS + 5);
    fake_btn[BTN_FAN] = false; run(BUTTON_DEBOUNCE_MS + BUTTON_CHORD_WINDOW_MS + 15);
    fake_rh_pct = 95;
    run(HUMIDITY_POLL_MS * 3);
    CHECK(!fan_is_on());
}

static void t_max_run(void)
{
    printf("humidity holds the fan at most HUMIDITY_MAX_RUN_MS\n");
    setup();
    fake_rh_pct = 90;
    run(HUMIDITY_POLL_MS + 100);
    CHECK(fan_is_on());
    run(HUMIDITY_MAX_RUN_MS - MIN(5));
    CHECK(fan_is_on());
    run(MIN(10));
    CHECK(!fan_is_on());                                 /* stuck-high sensor limited */
    fake_rh_pct = 50;
    run(HUMIDITY_POLL_MS + 100);
    fake_rh_pct = 90;                                    /* new humid period */
    run(HUMIDITY_POLL_MS + 100);
    CHECK(fan_is_on());
}

static void t_sensor_failure(void)
{
    printf("sensor failure: fan not held, fault LED steady\n");
    setup();
    fake_rh_pct = 90;
    run(HUMIDITY_POLL_MS + 100);
    CHECK(fan_is_on());
    fake_sensor_ok = false;
    run(HUMIDITY_POLL_MS * (HUMIDITY_FAIL_LIMIT + 1));
    CHECK(!fan_is_on());                                 /* broken sensor releases the fan */
    CHECK(ind_state[IND_FAULT]);                         /* steady fault LED */
    fake_sensor_ok = true;
    fake_rh_pct = 50;
    run(HUMIDITY_POLL_MS + 100);
    CHECK(!ind_state[IND_FAULT]);                        /* cleared on recovery */
}

int main(void)
{
    t_decode();
    t_hysteresis();
    t_polling();
    t_humid_starts_fan();
    t_humid_extends_fan_after_motion();
    t_humid_disabled_fan();
    t_max_run();
    t_sensor_failure();
    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
