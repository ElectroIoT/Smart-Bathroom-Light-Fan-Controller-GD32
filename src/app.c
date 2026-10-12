#include "app.h"
#include "button.h"
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
static button_t g_btn_light, g_btn_fan;
static bool g_light_out, g_fan_out;
static uint32_t g_led_ms;
static bool g_led_on;
static uint32_t g_fault_ms;
static bool g_fault_on;
static bool g_ind_pir, g_ind_light, g_ind_fan, g_ind_fan_en;   /* LED cache */

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
        hal_indicator(IND_SYSTEM, g_led_on);
    }
#else
    (void)now;
#endif
}

/* Status LEDs outside the enclosure: mirror state, written only on change. */
static void status_leds(uint32_t now)
{
#if ENABLE_STATUS_LEDS
    if (g_ctrl.pir_stable != g_ind_pir)      { g_ind_pir = g_ctrl.pir_stable;      hal_indicator(IND_PIR, g_ind_pir); }
    if (g_ctrl.light_out != g_ind_light)     { g_ind_light = g_ctrl.light_out;     hal_indicator(IND_LIGHT, g_ind_light); }
    if (g_ctrl.fan_out != g_ind_fan)         { g_ind_fan = g_ctrl.fan_out;         hal_indicator(IND_FAN, g_ind_fan); }
    if (g_ctrl.fan_enabled != g_ind_fan_en)  { g_ind_fan_en = g_ctrl.fan_enabled;  hal_indicator(IND_FANEN, g_ind_fan_en); }

    if (g_ctrl.fault_stuck) {
        if ((uint32_t)(now - g_fault_ms) >= FAULT_BLINK_MS) {
            g_fault_ms = now;
            g_fault_on = !g_fault_on;
            hal_indicator(IND_FAULT, g_fault_on);
        }
    } else if (g_fault_on) {
        g_fault_on = false;
        hal_indicator(IND_FAULT, false);
    }
#else
    (void)now;
#endif
}

void app_init(void)
{
    ctrl_cfg_t cfg;
    uint32_t now;

    g_light_out = false;
    g_fan_out = false;
    hal_init(RELAY_OFF_LEVEL, RELAY_OFF_LEVEL);   /* relays OFF first; starts watchdog */
    now = hal_millis();

    cfg.light_timeout_ms = LIGHT_TIMEOUT_MS;
    cfg.fan_timeout_ms = FAN_TIMEOUT_MS;
    cfg.warmup_ms = PIR_WARMUP_MS;
    cfg.debounce_ms = PIR_DEBOUNCE_MS;
    cfg.stuck_high_ms = PIR_STUCK_HIGH_MS;
    cfg.warn_before_ms = LIGHT_WARN_BEFORE_MS;
    cfg.warn_blink_ms = LIGHT_WARN_BLINK_MS;
    cfg.fan_start_delay_ms = FAN_START_DELAY_MS;
    cfg.override_ms = LIGHT_OVERRIDE_MS;
    cfg.fan_enabled = hal_load_fan_enabled();
    ctrl_init(&g_ctrl, now, &cfg);

    button_init(&g_btn_light, now, hal_button_pressed(BTN_LIGHT));
    button_init(&g_btn_fan, now, hal_button_pressed(BTN_FAN));

    g_led_ms = now;
    g_led_on = false;
    g_fault_on = false;
    g_fault_ms = now;
    g_ind_pir = g_ind_light = g_ind_fan = false;
    g_ind_fan_en = false;       /* forces the first write of the fan-enabled LED */
    hal_debug("BOOT: relays OFF, PIR warm-up\r\n");
    hal_debug(cfg.fan_enabled ? "FAN: enabled\r\n" : "FAN: DISABLED (button)\r\n");
}

void app_step(void)
{
    uint32_t now;
    bool level, active;

    hal_watchdog_kick();
    now = hal_millis();

    level = hal_pir_level();
    active = PIR_ACTIVE_HIGH ? level : !level;

    if (button_update(&g_btn_light, now, hal_button_pressed(BTN_LIGHT), BUTTON_DEBOUNCE_MS)) {
        ctrl_light_override_toggle(&g_ctrl, now);
        hal_debug(g_ctrl.override_active ? "OVERRIDE ON\r\n" : "OVERRIDE OFF\r\n");
    }
    if (button_update(&g_btn_fan, now, hal_button_pressed(BTN_FAN), BUTTON_DEBOUNCE_MS)) {
        ctrl_set_fan_enabled(&g_ctrl, !g_ctrl.fan_enabled);
        hal_save_fan_enabled(g_ctrl.fan_enabled);
        hal_debug(g_ctrl.fan_enabled ? "FAN: enabled\r\n" : "FAN: DISABLED\r\n");
    }

    ctrl_update(&g_ctrl, now, active);
    heartbeat(now);
    status_leds(now);
    relay_apply(g_ctrl.light_out, g_ctrl.fan_out);
}

const ctrl_t *app_state(void) { return &g_ctrl; }
