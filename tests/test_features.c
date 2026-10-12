/*
 * Feature tests with the DEFAULT configuration: warning blink, fan start
 * delay, manual light override, fan enable button (persistent), watchdog.
 * Time is simulated one millisecond per step.
 */
#include <stdio.h>
#include "app.h"
#include "config.h"
#include "fake_hal.h"

#define ON_LVL   (RELAY_ACTIVE_LOW ? 0 : 1)

static int g_fail, g_checks;
#define CHECK(cond) do { g_checks++; if (!(cond)) { g_fail++; \
    printf("  FAIL line %d: %s\n", __LINE__, #cond); } } while (0)

static bool light_is_on(void) { return light_pin == ON_LVL; }
static bool fan_is_on(void)   { return fan_pin == ON_LVL; }

static void run(uint32_t ms) { while (ms--) { fake_now++; app_step(); } }

static void setup(void)
{
    fake_reset(0);
    app_init();
    run(PIR_WARMUP_MS + 1);
}

static void pulse(uint32_t ms)
{
    fake_set_motion(true);  run(ms);
    fake_set_motion(false); run(PIR_DEBOUNCE_MS + 2);
}

static void press(button_id_t b)
{
    fake_btn[b] = true;  run(BUTTON_DEBOUNCE_MS + 5);
    fake_btn[b] = false; run(BUTTON_DEBOUNCE_MS + 5);
}

#define MIN(n) ((uint32_t)(n) * 60000UL)

static void t_warning_blink(void)
{
    printf("warning blink before light-off\n");
    setup();
    pulse(500);
    CHECK(light_is_on());
    uint32_t t_on = fake_now, t_blink = 0, t_back = 0, t_off = 0;
    bool prev = true;
    int transitions = 0;
    for (uint32_t i = 0; i < LIGHT_TIMEOUT_MS + 5000; i++) {
        run(1);
        bool cur = light_is_on();
        if (cur != prev) {
            transitions++;
            if (transitions == 1) t_blink = fake_now;       /* ON -> OFF (blink) */
            else if (transitions == 2) t_back = fake_now;   /* OFF -> ON         */
            else if (transitions == 3) { t_off = fake_now; break; }
            prev = cur;
        }
    }
    CHECK(transitions == 3);
    CHECK(t_back - t_blink >= LIGHT_WARN_BLINK_MS - 5 && t_back - t_blink <= LIGHT_WARN_BLINK_MS + 5);
    CHECK(t_off - t_blink >= LIGHT_WARN_BEFORE_MS - 5 && t_off - t_blink <= LIGHT_WARN_BEFORE_MS + 5);
    CHECK(t_blink - t_on > LIGHT_TIMEOUT_MS - LIGHT_WARN_BEFORE_MS - 200);
    CHECK(fan_is_on());                   /* fan unaffected by the light blink / light-off */
}

static void t_warning_wave_restarts(void)
{
    printf("motion after the warning restarts the timer\n");
    setup();
    pulse(500);
    run(LIGHT_TIMEOUT_MS - LIGHT_WARN_BEFORE_MS + LIGHT_WARN_BLINK_MS + 2000);  /* blink done */
    CHECK(light_is_on());
    pulse(500);                           /* user waves */
    run(LIGHT_TIMEOUT_MS - 2000);         /* well past the old expiry */
    CHECK(light_is_on());
    run(4000);
    CHECK(!light_is_on());                /* ends after full timeout from the wave */
}

static void t_fan_start_delay(void)
{
    printf("fan starts after the light has been on FAN_START_DELAY_MS\n");
    setup();
    fake_set_motion(true);
    run(PIR_DEBOUNCE_MS + 5);
    fake_set_motion(false);
    CHECK(light_is_on() && !fan_is_on());
    run(FAN_START_DELAY_MS - 2000);
    CHECK(light_is_on() && !fan_is_on());     /* still waiting */
    run(4000);
    CHECK(light_is_on() && fan_is_on());      /* started */
    run(FAN_TIMEOUT_MS + 1000);
    CHECK(!fan_is_on() && !light_is_on());
}

static void t_fan_keeps_running_on_new_motion(void)
{
    printf("fan already running: new motion does not restart the delay\n");
    setup();
    pulse(500);
    run(MIN(8));                              /* light off (7 min), fan on */
    CHECK(!light_is_on() && fan_is_on());
    pulse(500);
    CHECK(light_is_on() && fan_is_on());      /* fan never dropped */
}

static void t_override(void)
{
    printf("manual light override\n");
    setup();
    press(BTN_LIGHT);
    CHECK(light_is_on() && !fan_is_on());     /* light only, fan independent */
    run(MIN(20));
    CHECK(light_is_on());                     /* no PIR needed */
    run(LIGHT_OVERRIDE_MS - MIN(20) + 2000);
    CHECK(!light_is_on());                    /* ends by itself */

    press(BTN_LIGHT);
    CHECK(light_is_on());
    press(BTN_LIGHT);                         /* press again cancels */
    CHECK(!light_is_on());

    pulse(500);                               /* fan keeps its own logic */
    press(BTN_LIGHT);
    run(FAN_START_DELAY_MS + 2000);
    CHECK(light_is_on() && fan_is_on());
}

static void t_button_debounce(void)
{
    printf("button debounce and no repeat while held\n");
    setup();
    fake_btn[BTN_LIGHT] = true;  run(BUTTON_DEBOUNCE_MS - 10);
    fake_btn[BTN_LIGHT] = false; run(100);    /* bounce shorter than debounce */
    CHECK(!light_is_on());
    fake_btn[BTN_LIGHT] = true;  run(MIN(1));  /* held for a minute: one event */
    CHECK(light_is_on());
    fake_btn[BTN_LIGHT] = false; run(100);
    CHECK(light_is_on());
}

static void t_button_held_at_boot(void)
{
    printf("button held at boot gives no event\n");
    fake_reset(0);
    fake_btn[BTN_LIGHT] = true;
    app_init();
    run(PIR_WARMUP_MS + 1000);
    CHECK(!light_is_on());
}

static void t_fan_disable_button(void)
{
    printf("fan enable/disable button, LED, persistence\n");
    setup();
    CHECK(ind_state[IND_FANEN]);              /* enabled by default: LED on */
    press(BTN_FAN);
    CHECK(!ind_state[IND_FANEN]);             /* disabled: LED off */
    CHECK(!stored_fan_enabled && store_writes == 1);
    pulse(500);
    run(FAN_START_DELAY_MS + 2000);
    CHECK(light_is_on() && !fan_is_on());     /* PIR still works for the light */
    run(MIN(12));
    CHECK(!fan_is_on() && !ind_state[IND_FAN]);

    /* power failure / restart: still disabled, LED off */
    app_init();
    run(PIR_WARMUP_MS + 1);
    CHECK(!ind_state[IND_FANEN]);
    pulse(500);
    run(FAN_START_DELAY_MS + 2000);
    CHECK(light_is_on() && !fan_is_on());

    /* enable again */
    press(BTN_FAN);
    CHECK(ind_state[IND_FANEN] && stored_fan_enabled);
    pulse(500);
    run(FAN_START_DELAY_MS + 2000);
    CHECK(fan_is_on());

    /* disabling while the fan runs turns it off at once */
    press(BTN_FAN);
    CHECK(!fan_is_on() && !ind_state[IND_FANEN]);
}

static void t_watchdog_kick(void)
{
    printf("watchdog kicked every pass (also during warm-up)\n");
    fake_reset(0);
    app_init();
    run(1000);
    CHECK(wdt_kicks == 1000);
    run(PIR_WARMUP_MS);
    CHECK((uint32_t)wdt_kicks == 1000 + PIR_WARMUP_MS);
}

int main(void)
{
    t_warning_blink();
    t_warning_wave_restarts();
    t_fan_start_delay();
    t_fan_keeps_running_on_new_motion();
    t_override();
    t_button_debounce();
    t_button_held_at_boot();
    t_fan_disable_button();
    t_watchdog_kick();
    printf("\n%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
