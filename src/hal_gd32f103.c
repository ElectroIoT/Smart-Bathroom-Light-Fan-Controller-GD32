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

bool hal_load_fan_enabled(void)
{
    volatile const uint16_t *p = (volatile const uint16_t *)STORE_PAGE_ADDR;
    if (p[0] == STORE_MAGIC)
        return p[1] != 0U;
    return true;                  /* never saved: fan enabled */
}

void hal_save_fan_enabled(bool enabled)
{
    volatile uint16_t *p = (volatile uint16_t *)STORE_PAGE_ADDR;

    if (p[0] == STORE_MAGIC && (p[1] != 0U) == enabled)
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
        p[1] = enabled ? 1U : 0U;
        (void)fmc_wait();
    }
    FMC_CTL &= ~(FMC_CTL_PG | FMC_CTL_PER);
    FMC_CTL |= FMC_CTL_LK;
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
