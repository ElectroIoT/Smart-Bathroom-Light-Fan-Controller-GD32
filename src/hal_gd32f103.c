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

void hal_init(bool light_off_level, bool fan_off_level)
{
    clock_enable_port(PIR_PORT);
    clock_enable_port(LIGHT_PORT);
    clock_enable_port(FAN_PORT);
    clock_enable_port(0);         /* PA9 debug TX */

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
}

uint32_t hal_millis(void) { return g_ms; }   /* aligned 32-bit read is atomic */

bool hal_pir_level(void)
{
    return (GPIO_ISTAT(PIR_PORT) & (1UL << PIR_PIN)) != 0;
}

void hal_light_write(bool level) { gpio_write(LIGHT_PORT, LIGHT_PIN, level); }
void hal_fan_write(bool level)   { gpio_write(FAN_PORT, FAN_PIN, level); }

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
