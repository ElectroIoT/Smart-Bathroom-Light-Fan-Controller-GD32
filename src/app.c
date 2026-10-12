#include "app.h"
#include "config.h"
#include "hal.h"

#if (LIGHT_TIMEOUT_MS == 0) || (FAN_TIMEOUT_MS == 0)
#error "Timeouts must be greater than zero"
#endif
#if (LIGHT_TIMEOUT_MS > 0x7FFFFFFFUL) || (FAN_TIMEOUT_MS > 0x7FFFFFFFUL)
#error "Timeouts must be below 2^31 ms for wrap-safe arithmetic"
#endif

/* Physical pin level that means "relay OFF" / "relay ON". */
#define RELAY_OFF_LEVEL  (RELAY_ACTIVE_LOW ? 1 : 0)
#define RELAY_ON_LEVEL   (RELAY_ACTIVE_LOW ? 0 : 1)

static ctrl_t g_ctrl;
static bool g_light_out, g_fan_out;
static uint32_t g_led_ms;
static bool g_led_on;

static void relay_apply(bool light_on, bool fan_on)
{
    if (light_on != g_light_out) {
        g_light_out = light_on;
        hal_light_write(light_on ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
        hal_debug(light_on ? "LIGHT ON\r\n" : "LIGHT OFF\r\n");
    }
    if (fan_on != g_fan_out) {
        g_fan_out = fan_on;
        hal_fan_write(fan_on ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
        hal_debug(fan_on ? "FAN ON\r\n" : "FAN OFF\r\n");
    }
}

/* Non-blocking heartbeat. Warm-up: 125 ms toggle. Normal: 100 ms blip / 2 s. */
static void heartbeat(uint32_t now)
{
#if ENABLE_HEARTBEAT_LED
    bool warm = (uint32_t)(now - g_ctrl.start_ms) < PIR_WARMUP_MS;
    uint32_t wait = warm ? 125UL : (g_led_on ? 100UL : 1900UL);
    if ((uint32_t)(now - g_led_ms) >= wait) {
        g_led_ms = now;
        g_led_on = !g_led_on;
        hal_led_write(g_led_on);
    }
#else
    (void)now;
#endif
}

void app_init(void)
{
    g_light_out = false;
    g_fan_out = false;
    hal_init(RELAY_OFF_LEVEL, RELAY_OFF_LEVEL);
    ctrl_init(&g_ctrl, hal_millis(), LIGHT_TIMEOUT_MS, FAN_TIMEOUT_MS,
              PIR_WARMUP_MS, PIR_DEBOUNCE_MS, PIR_STUCK_HIGH_MS);
    g_led_ms = hal_millis();
    g_led_on = false;
    hal_debug("BOOT: relays OFF, PIR warm-up\r\n");
}

void app_step(void)
{
    bool level = hal_pir_level();
    bool active = PIR_ACTIVE_HIGH ? level : !level;

    uint32_t now = hal_millis();

    ctrl_update(&g_ctrl, now, active);
    heartbeat(now);
    relay_apply(g_ctrl.light_on, g_ctrl.fan_on);
}

const ctrl_t *app_state(void) { return &g_ctrl; }
