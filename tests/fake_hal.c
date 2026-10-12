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
int led_toggles;
static bool led_prev;
void hal_led_write(bool on) { if (on != led_prev) led_toggles++; led_prev = on; }
void hal_debug(const char *msg) { (void)msg; }
