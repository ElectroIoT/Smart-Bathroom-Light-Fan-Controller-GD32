/**
 * hal_gd32f103.c - bare-register HAL for GD32F103C8T6.
 *
 * UNVERIFIED AGAINST THE GD32F10x USER MANUAL: base addresses and register
 * offsets below follow the STM32F103-compatible layout that the GD32F10x
 * family is believed to share. Check each against the official manual
 * before trusting it (see docs/hardware_design.md, "To verify").
 * SysTick registers are ARM Cortex-M3 architectural (not GD-specific).
 */
#include "hal.h"
#include "config.h"

#define REG32(a) (*(volatile uint32_t *)(a))

/* --- clocks (RCU) --- */
#define RCU_BASE        0x40021000UL
#define RCU_APB2EN      REG32(RCU_BASE + 0x18UL)
#define APB2EN_GPIOA    (1UL << 2)
#define APB2EN_GPIOB    (1UL << 3)
#define APB2EN_GPIOC    (1UL << 4)
#define APB2EN_ADC1     (1UL << 9)
#define APB2EN_USART0   (1UL << 14)

/* --- GPIO --- */
#define GPIO_BASE(p)    (0x40010800UL + 0x400UL * (unsigned long)(p))
#define GPIO_CTL0(p)    REG32(GPIO_BASE(p) + 0x00UL)
#define GPIO_CTL1(p)    REG32(GPIO_BASE(p) + 0x04UL)
#define GPIO_ISTAT(p)   REG32(GPIO_BASE(p) + 0x08UL)
#define GPIO_BOP(p)     REG32(GPIO_BASE(p) + 0x10UL)   /* set: bits 0-15, reset: 16-31 */

#define MODE_IN_FLOAT   0x4UL
#define MODE_IN_PUPD    0x8UL
#define MODE_OUT_PP_2M  0x2UL
#define MODE_AF_PP_2M   0xAUL

/* --- USART0 (USART1 on STM32), TX on PA9 --- */
#define USART0_BASE     0x40013800UL
#define USART_STAT      REG32(USART0_BASE + 0x00UL)
#define USART_DATA      REG32(USART0_BASE + 0x04UL)
#define USART_BAUD      REG32(USART0_BASE + 0x08UL)
#define USART_CTL0      REG32(USART0_BASE + 0x0CUL)
#define USART_TBE       (1UL << 7)
#define USART_TEN       (1UL << 3)
#define USART_UEN       (1UL << 13)

/* --- SysTick (ARM architectural) --- */
#define SYST_CSR        REG32(0xE000E010UL)
#define SYST_RVR        REG32(0xE000E014UL)
#define SYST_CVR        REG32(0xE000E018UL)

static volatile uint32_t g_ms;

void SysTick_Handler(void) { g_ms++; }

static void gpio_mode(unsigned port, unsigned pin, uint32_t mode)
{
    unsigned sh = (pin & 7u) * 4u;
    if (pin < 8)
        GPIO_CTL0(port) = (GPIO_CTL0(port) & ~(0xFUL << sh)) | (mode << sh);
    else
        GPIO_CTL1(port) = (GPIO_CTL1(port) & ~(0xFUL << sh)) | (mode << sh);
}

static void gpio_write(unsigned port, unsigned pin, bool level)
{
    GPIO_BOP(port) = level ? (1UL << pin) : (1UL << (pin + 16u));
}

static void clock_enable_port(unsigned port)
{
    RCU_APB2EN |= (port == 0) ? APB2EN_GPIOA :
                  (port == 1) ? APB2EN_GPIOB : APB2EN_GPIOC;
}

/* Indicator pin table, order matches indicator_t. */
static const struct { uint8_t port, pin; bool active_low; } ind_tab[IND_COUNT] = {
    { LED_PORT,        LED_PIN,        LED_ACTIVE_LOW },
    { LED_PIR_PORT,    LED_PIR_PIN,    STATUS_LED_ACTIVE_LOW },
    { LED_LIGHT_PORT,  LED_LIGHT_PIN,  STATUS_LED_ACTIVE_LOW },
    { LED_FAN_PORT,    LED_FAN_PIN,    STATUS_LED_ACTIVE_LOW },
    { LED_FAULT_PORT,  LED_FAULT_PIN,  STATUS_LED_ACTIVE_LOW },
    { LED_FANEN_PORT,  LED_FANEN_PIN,  STATUS_LED_ACTIVE_LOW },
    { LED_SYSEXT_PORT, LED_SYSEXT_PIN, STATUS_LED_ACTIVE_LOW },
};

/* Button pin table, order matches button_id_t. */
static const struct { uint8_t port, pin; } btn_tab[BTN_COUNT] = {
    { BTN_LIGHT_PORT, BTN_LIGHT_PIN },
    { BTN_FAN_PORT,   BTN_FAN_PIN },
};

/* --- independent watchdog (IWDG / FWDGT), UNVERIFIED vs GD32 manual --- */
#define IWDG_BASE       0x40003000UL
#define IWDG_KR         REG32(IWDG_BASE + 0x00UL)
#define IWDG_PR         REG32(IWDG_BASE + 0x04UL)
#define IWDG_RLR        REG32(IWDG_BASE + 0x08UL)
#define IWDG_SR         REG32(IWDG_BASE + 0x0CUL)
#define DBG_CTL0        REG32(0xE0042004UL)      /* bit 8: freeze watchdog while halted */

static void watchdog_start(void)
{
#if ENABLE_WATCHDOG
    /* ~40 kHz low-speed oscillator / 64 = 625 Hz (nominal; +-tolerance). */
    uint32_t reload = (WATCHDOG_TIMEOUT_MS * 625UL) / 1000UL;
    volatile uint32_t guard = 100000UL;

    DBG_CTL0 |= (1UL << 8);       /* debug halt must not reset the chip */
    IWDG_KR = 0x5555UL;           /* unlock PR / RLR */
    IWDG_PR = 4UL;                /* divider 64 */
    IWDG_RLR = (reload > 0xFFFUL) ? 0xFFFUL : reload - 1UL;
    while (IWDG_SR != 0UL && guard--) { }
    IWDG_KR = 0xAAAAUL;           /* load counter */
    IWDG_KR = 0xCCCCUL;           /* start (cannot be stopped) */
#endif
}

void hal_watchdog_kick(void)
{
#if ENABLE_WATCHDOG
    IWDG_KR = 0xAAAAUL;
#endif
}

/* --- flash storage for the fan-enable flag (FMC, UNVERIFIED vs manual) --- */
#define FMC_KEY         REG32(0x40022004UL)
#define FMC_STAT        REG32(0x4002200CUL)
#define FMC_CTL         REG32(0x40022010UL)
#define FMC_ADDR        REG32(0x40022014UL)
#define FMC_BUSY        (1UL << 0)
#define FMC_CTL_PG      (1UL << 0)
#define FMC_CTL_PER     (1UL << 1)
#define FMC_CTL_START   (1UL << 6)
#define FMC_CTL_LK      (1UL << 7)
#define STORE_MAGIC     0x5AA5U

static bool fmc_wait(void)
{
    volatile uint32_t guard = 2000000UL;
    while ((FMC_STAT & FMC_BUSY) && guard) guard--;
    return guard != 0;
}

void hal_load_settings(bool *fan_enabled, bool *leds_silent)
{
    volatile const uint16_t *p = (volatile const uint16_t *)STORE_PAGE_ADDR;
    *fan_enabled = true;          /* never saved: fan enabled, LEDs normal */
    *leds_silent = false;
    if (p[0] == STORE_MAGIC) {
        *fan_enabled = p[1] != 0U;
        *leds_silent = (p[2] == 1U);   /* erased (0xFFFF) by an older save = normal */
    }
}

void hal_save_settings(bool fan_enabled, bool leds_silent)
{
    volatile uint16_t *p = (volatile uint16_t *)STORE_PAGE_ADDR;

    if (p[0] == STORE_MAGIC && (p[1] != 0U) == fan_enabled && (p[2] == 1U) == leds_silent)
        return;                   /* already stored: no flash wear */

    FMC_KEY = 0x45670123UL;
    FMC_KEY = 0xCDEF89ABUL;
    FMC_STAT = 0x34UL;            /* clear end/error flags */

    FMC_CTL |= FMC_CTL_PER;       /* erase the page */
    FMC_ADDR = STORE_PAGE_ADDR;
    FMC_CTL |= FMC_CTL_START;
    if (fmc_wait()) {
        FMC_CTL &= ~FMC_CTL_PER;
        FMC_CTL |= FMC_CTL_PG;    /* program two half-words */
        p[0] = STORE_MAGIC;
        (void)fmc_wait();
        p[1] = fan_enabled ? 1U : 0U;
        (void)fmc_wait();
        p[2] = leds_silent ? 1U : 0U;
        (void)fmc_wait();
    }
    FMC_CTL &= ~(FMC_CTL_PG | FMC_CTL_PER);
    FMC_CTL |= FMC_CTL_LK;
}

/* --- optional DHT11 / DHT22 single-wire read (UNTESTED on hardware) --- */
#if HUMIDITY_SENSOR
/* Microseconds elapsed since a SysTick counter snapshot (< 1 ms spans). */
static uint32_t ticks_since(uint32_t start)
{
    uint32_t now = SYST_CVR;                       /* counts down */
    return (start >= now) ? (start - now) : (start + SYST_RVR + 1UL - now);
}

/* Wait for the pin to reach `level`; returns microseconds waited, 0 = timeout. */
static uint32_t wait_level(bool level, uint32_t timeout_us)
{
    uint32_t t0 = SYST_CVR, per_us = SYSCLK_HZ / 1000000UL, limit = timeout_us * per_us;
    while (((GPIO_ISTAT(HUMIDITY_PORT) & (1UL << HUMIDITY_PIN)) != 0) != level) {
        if (ticks_since(t0) > limit) return 0;
    }
    uint32_t w = ticks_since(t0) / per_us;
    return w ? w : 1;
}
#endif

bool hal_humidity_read(uint8_t raw[5])
{
#if HUMIDITY_SENSOR
    uint32_t per_us = SYSCLK_HZ / 1000000UL;
    unsigned i;

    for (i = 0; i < 5; i++) raw[i] = 0;
    clock_enable_port(HUMIDITY_PORT);

    /* start signal: pull LOW 20 ms, then release (pull-up) */
    gpio_write(HUMIDITY_PORT, HUMIDITY_PIN, false);
    gpio_mode(HUMIDITY_PORT, HUMIDITY_PIN, MODE_OUT_PP_2M);
    { uint32_t t = g_ms; while ((uint32_t)(g_ms - t) < 21UL) { } }
    gpio_write(HUMIDITY_PORT, HUMIDITY_PIN, true);
    gpio_mode(HUMIDITY_PORT, HUMIDITY_PIN, MODE_IN_PUPD);   /* latch HIGH = pull-up */

    /* sensor answers: LOW ~80 us, HIGH ~80 us */
    if (!wait_level(false, 100)) return false;
    if (!wait_level(true, 120))  return false;
    if (!wait_level(false, 120)) return false;

    for (i = 0; i < 40; i++) {
        uint32_t t0, high;
        if (!wait_level(true, 100)) return false;           /* end of the 50 us low */
        t0 = SYST_CVR;
        if (!wait_level(false, 120)) return false;          /* HIGH width: 26 us=0, 70 us=1 */
        high = ticks_since(t0) / per_us;
        raw[i / 8] = (uint8_t)((raw[i / 8] << 1) | (high > 45UL ? 1u : 0u));
    }
    return true;
#else
    (void)raw;
    return false;
#endif
}

/* --- optional bare-LDR ADC (ADC1, software start), UNVERIFIED vs manual --- */
#define ADC_BASE        0x40012400UL
#define ADC_STAT        REG32(ADC_BASE + 0x00UL)   /* bit 1: end of conversion */
#define ADC_CTL1        REG32(ADC_BASE + 0x08UL)
#define ADC_SAMPT1      REG32(ADC_BASE + 0x10UL)   /* sample time, channels 0-9 */
#define ADC_RSQ0        REG32(ADC_BASE + 0x2CUL)   /* sequence length */
#define ADC_RSQ2        REG32(ADC_BASE + 0x34UL)   /* first conversion channel */
#define ADC_RDATA       REG32(ADC_BASE + 0x4CUL)
#define ADC_ADON        (1UL << 0)
#define ADC_CAL         (1UL << 2)
#define ADC_RSTCAL      (1UL << 3)
#define ADC_EXTSEL_SW   (7UL << 17)
#define ADC_EXTTRIG     (1UL << 20)
#define ADC_SWSTART     (1UL << 22)

#if DAYLIGHT_SENSOR == 2
static void adc_init(void)
{
    volatile uint32_t guard;
    RCU_APB2EN |= APB2EN_ADC1;
    clock_enable_port(LDR_PORT);
    gpio_mode(LDR_PORT, LDR_PIN, 0x0UL);              /* analog input */
    ADC_CTL1 = ADC_ADON;                              /* power up */
    for (guard = 2000UL; guard; guard--) { }          /* stabilisation */
    ADC_CTL1 |= ADC_RSTCAL;
    for (guard = 100000UL; (ADC_CTL1 & ADC_RSTCAL) && guard; guard--) { }
    ADC_CTL1 |= ADC_CAL;
    for (guard = 1000000UL; (ADC_CTL1 & ADC_CAL) && guard; guard--) { }
    ADC_SAMPT1 |= 7UL << (3UL * LDR_ADC_CHANNEL);     /* 239.5 cycles: high-impedance divider */
    ADC_RSQ0 = 0UL;                                   /* one conversion */
    ADC_RSQ2 = (uint32_t)LDR_ADC_CHANNEL;
    ADC_CTL1 |= ADC_EXTTRIG | ADC_EXTSEL_SW;          /* software trigger */
}
#endif

uint16_t hal_ldr_read(void)
{
#if DAYLIGHT_SENSOR == 2
    uint32_t sum = 0;
    for (unsigned n = 0; n < 4; n++) {      /* average 4 samples */
        volatile uint32_t guard = 100000UL;
        ADC_CTL1 |= ADC_SWSTART;
        while (!(ADC_STAT & (1UL << 1)) && guard) guard--;
        if (!guard) return 0;                      /* timeout = dark = light works */
        sum += ADC_RDATA & 0xFFFUL;
    }
    return (uint16_t)(sum / 4UL);
#else
    return 0;
#endif
}

void hal_debug_value(const char *label, uint32_t value)
{
#if ENABLE_DEBUG_UART
    char buf[11];
    int i = 10;
    buf[i] = 0;
    do { buf[--i] = (char)('0' + (value % 10UL)); value /= 10UL; } while (value);
    hal_debug(label);
    hal_debug(&buf[i]);
    hal_debug("\r\n");
#else
    (void)label; (void)value;
#endif
}

bool hal_daylight_bright(void)
{
#if DAYLIGHT_SENSOR == 1
    bool level = (GPIO_ISTAT(DAYLIGHT_PORT) & (1UL << DAYLIGHT_PIN)) != 0;
    return level == (DAYLIGHT_BRIGHT_LEVEL != 0);
#else
    return false;
#endif
}

bool hal_button_pressed(button_id_t id)
{
    bool level;
    if (id >= BTN_COUNT) return false;
    level = (GPIO_ISTAT(btn_tab[id].port) & (1UL << btn_tab[id].pin)) != 0;
    return BUTTON_ACTIVE_LOW ? !level : level;
}

static bool ind_enabled(unsigned id)
{
    return (id == IND_SYSTEM) ? (ENABLE_HEARTBEAT_LED != 0) : (ENABLE_STATUS_LEDS != 0);
}

void hal_init(bool light_off_level, bool fan_off_level)
{
    clock_enable_port(PIR_PORT);
    clock_enable_port(LIGHT_PORT);
    clock_enable_port(FAN_PORT);
    clock_enable_port(0);         /* PA9 debug TX */
    for (unsigned i = 0; i < IND_COUNT; i++) {
        if (!ind_enabled(i)) continue;
        clock_enable_port(ind_tab[i].port);
        gpio_write(ind_tab[i].port, ind_tab[i].pin, ind_tab[i].active_low);  /* off */
        gpio_mode(ind_tab[i].port, ind_tab[i].pin, MODE_OUT_PP_2M);
    }

    /* Buttons: input with pull (pull-up when wired to GND). */
    for (unsigned i = 0; i < BTN_COUNT; i++) {
        clock_enable_port(btn_tab[i].port);
        gpio_mode(btn_tab[i].port, btn_tab[i].pin, MODE_IN_PUPD);
        gpio_write(btn_tab[i].port, btn_tab[i].pin, BUTTON_ACTIVE_LOW);
    }

#if DAYLIGHT_SENSOR == 2
    adc_init();
#elif DAYLIGHT_SENSOR == 1
    /* Digital LDR module: pull toward DARK so a disconnected wire reads dark. */
    clock_enable_port(DAYLIGHT_PORT);
    gpio_mode(DAYLIGHT_PORT, DAYLIGHT_PIN, MODE_IN_PUPD);
    gpio_write(DAYLIGHT_PORT, DAYLIGHT_PIN, DAYLIGHT_BRIGHT_LEVEL == 0);
#endif

    /* Safe state first: latch the OFF level, THEN make the pins outputs. */
    gpio_write(LIGHT_PORT, LIGHT_PIN, light_off_level);
    gpio_write(FAN_PORT, FAN_PIN, fan_off_level);
    gpio_mode(LIGHT_PORT, LIGHT_PIN, MODE_OUT_PP_2M);
    gpio_mode(FAN_PORT, FAN_PIN, MODE_OUT_PP_2M);

    /* PIR input with configurable bias (PIR_PULL: 0 float, 1 down, 2 up). */
    if (PIR_PULL == 0) {
        gpio_mode(PIR_PORT, PIR_PIN, MODE_IN_FLOAT);
    } else {
        gpio_mode(PIR_PORT, PIR_PIN, MODE_IN_PUPD);
        gpio_write(PIR_PORT, PIR_PIN, PIR_PULL == 2);  /* output latch picks pull */
    }

#if ENABLE_DEBUG_UART
    RCU_APB2EN |= APB2EN_USART0;
    gpio_mode(0, 9, MODE_AF_PP_2M);
    USART_BAUD = (SYSCLK_HZ + DEBUG_BAUD / 2) / DEBUG_BAUD;
    USART_CTL0 = USART_UEN | USART_TEN;
#endif

    /* 1 ms tick from the processor clock. */
    SYST_RVR = (SYSCLK_HZ / 1000UL) - 1UL;
    SYST_CVR = 0;
    SYST_CSR = 0x7UL;             /* enable, tick interrupt, CPU clock */

    watchdog_start();             /* last: relays are already in their safe state */
}

uint32_t hal_millis(void) { return g_ms; }   /* aligned 32-bit read is atomic */

bool hal_pir_level(void)
{
    return (GPIO_ISTAT(PIR_PORT) & (1UL << PIR_PIN)) != 0;
}

void hal_light_write(bool level) { gpio_write(LIGHT_PORT, LIGHT_PIN, level); }
void hal_fan_write(bool level)   { gpio_write(FAN_PORT, FAN_PIN, level); }

void hal_indicator(indicator_t id, bool on)
{
    if (id >= IND_COUNT || !ind_enabled(id)) return;
    gpio_write(ind_tab[id].port, ind_tab[id].pin, ind_tab[id].active_low ? !on : on);
}

void hal_debug(const char *msg)
{
#if ENABLE_DEBUG_UART
    /* Short fixed messages; waits only for the UART transmit register. */
    while (*msg) {
        while (!(USART_STAT & USART_TBE)) { }
        USART_DATA = (uint8_t)*msg++;
    }
#else
    (void)msg;
#endif
}
