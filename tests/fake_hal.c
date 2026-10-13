#include "fake_hal.h"
#include "hal.h"
#include "config.h"

uint32_t fake_now;
bool fake_pir_level;
bool light_pin, fan_pin;
bool light_pin_driven, fan_pin_driven;
int light_writes, fan_writes;
int init_calls;
bool init_light_off, init_fan_off;

bool fake_btn[BTN_COUNT];
bool stored_fan_enabled = true;
int store_writes;
int wdt_kicks;

bool hal_button_pressed(button_id_t id) { return fake_btn[id]; }
bool hal_load_fan_enabled(void) { return stored_fan_enabled; }
void hal_save_fan_enabled(bool enabled) { stored_fan_enabled = enabled; store_writes++; }
void hal_watchdog_kick(void) { wdt_kicks++; }

bool fake_daylight;
int fake_ldr_adc;
bool hal_daylight_bright(void) { return fake_daylight; }
uint16_t hal_ldr_read(void) { return (uint16_t)fake_ldr_adc; }
void hal_debug_value(const char *label, uint32_t value) { (void)label; (void)value; }

bool fake_sensor_ok = true;
int fake_rh_pct = 40;
int humidity_reads;

bool hal_humidity_read(uint8_t raw[5])
{
    humidity_reads++;
    if (!fake_sensor_ok) return false;
    if (HUMIDITY_SENSOR == 22) {
        raw[0] = (uint8_t)((fake_rh_pct * 10) >> 8);
        raw[1] = (uint8_t)((fake_rh_pct * 10) & 0xFF);
    } else {
        raw[0] = (uint8_t)fake_rh_pct;
        raw[1] = 0;
    }
    raw[2] = 25; raw[3] = 0;
    raw[4] = (uint8_t)(raw[0] + raw[1] + raw[2] + raw[3]);
    return true;
}

void fake_set_motion(bool motion)
{
    fake_pir_level = PIR_ACTIVE_HIGH ? motion : !motion;
}

void fake_reset(uint32_t start_time)
{
    fake_now = start_time;
    fake_set_motion(false);
    light_pin = fan_pin = false;
    light_pin_driven = fan_pin_driven = false;
    light_writes = fan_writes = 0;
    init_calls = 0;
    for (int i = 0; i < BTN_COUNT; i++) fake_btn[i] = false;
    stored_fan_enabled = true;
    store_writes = 0;
    wdt_kicks = 0;
    fake_daylight = false;
    fake_ldr_adc = 0;
    fake_sensor_ok = true;
    fake_rh_pct = 40;
    humidity_reads = 0;
    for (int i = 0; i < IND_COUNT; i++) { ind_state[i] = false; ind_toggles[i] = 0; }
}

void hal_init(bool light_off_level, bool fan_off_level)
{
    init_calls++;
    init_light_off = light_off_level;
    init_fan_off = fan_off_level;
    light_pin = light_off_level;      /* latch safe level before driving */
    fan_pin = fan_off_level;
    light_pin_driven = fan_pin_driven = true;
}

uint32_t hal_millis(void) { return fake_now; }
bool hal_pir_level(void) { return fake_pir_level; }
void hal_light_write(bool level) { light_pin = level; light_writes++; }
void hal_fan_write(bool level) { fan_pin = level; fan_writes++; }
bool ind_state[IND_COUNT];
int ind_toggles[IND_COUNT];
void hal_indicator(indicator_t id, bool on)
{
    if (ind_state[id] != on) ind_toggles[id]++;
    ind_state[id] = on;
}
void hal_debug(const char *msg) { (void)msg; }
