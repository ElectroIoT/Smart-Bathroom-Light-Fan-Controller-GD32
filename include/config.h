/**
 * config.h - single place for every tunable / hardware-specific setting.
 *
 * Every macro is wrapped in #ifndef so tests (or the build command) can
 * override it with -DNAME=value without editing this file.
 *
 * STATUS: pin choices and polarities below are PROPOSALS. They have NOT been
 * checked against your exact board or relay module. See docs/hardware_design.md.
 */
#ifndef CONFIG_H
#define CONFIG_H

/* ------------------------------------------------------------------ */
/* Timing                                                              */
/* ------------------------------------------------------------------ */
#ifndef LIGHT_TIMEOUT_MS
#define LIGHT_TIMEOUT_MS   420000UL   /* 7 min after the most recent motion  */
#endif
#ifndef FAN_TIMEOUT_MS
#define FAN_TIMEOUT_MS     600000UL   /* 10 min after the most recent motion */
#endif

/* PIR modules usually output HIGH / unstable for 30-60 s after power-up.
 * Motion is ignored (relays stay OFF) for this long after boot. */
#ifndef PIR_WARMUP_MS
#define PIR_WARMUP_MS      60000UL
#endif

/* PIR level must be stable this long before it counts as an edge. */
#ifndef PIR_DEBOUNCE_MS
#define PIR_DEBOUNCE_MS    50UL
#endif

/* If the PIR stays HIGH longer than this, treat it as stuck/faulty: stop
 * holding the relays on, let the normal timeouts run, and ignore the PIR
 * until it goes LOW again. */
#ifndef PIR_STUCK_HIGH_MS
#define PIR_STUCK_HIGH_MS  1800000UL  /* 30 min */
#endif

/* ------------------------------------------------------------------ */
/* Polarity                                                            */
/* ------------------------------------------------------------------ */
/* 1 = PIR output is HIGH when motion is detected, 0 = LOW when detected. */
#ifndef PIR_ACTIVE_HIGH
#define PIR_ACTIVE_HIGH    1
#endif

/* 1 = relay module energises when its input is driven LOW (most common
 * opto-isolated blue modules), 0 = energises when input is HIGH.
 * UNVERIFIED for your module - bench-test before connecting any load. */
#ifndef RELAY_ACTIVE_LOW
#define RELAY_ACTIVE_LOW   1
#endif

/* ------------------------------------------------------------------ */
/* GPIO assignment (port: 0=A, 1=B, 2=C ; pin: 0..15)                  */
/* ------------------------------------------------------------------ */
#ifndef PIR_PORT
#define PIR_PORT           0
#define PIR_PIN            0          /* PA0 */
#endif
#ifndef LIGHT_PORT
#define LIGHT_PORT         0
#define LIGHT_PIN          1          /* PA1 */
#endif
#ifndef FAN_PORT
#define FAN_PORT           0
#define FAN_PIN            2          /* PA2 */
#endif

/* PIR input bias: 0 = floating, 1 = pull-down, 2 = pull-up.
 * Pull-down is the safe default for an active-HIGH sensor so a
 * disconnected wire reads "no motion". */
#ifndef PIR_PULL
#define PIR_PULL           1
#endif

/* ------------------------------------------------------------------ */
/* Clock / debug                                                       */
/* ------------------------------------------------------------------ */
/* Firmware does NOT configure the PLL: it runs from the reset-default
 * internal oscillator. Change this value only if you change the clock. */
#ifndef SYSCLK_HZ
#define SYSCLK_HZ          8000000UL
#endif

/* Debug text on USART1 TX (PA9), 9600 8N1. 0 disables it entirely. */
#ifndef ENABLE_DEBUG_UART
#define ENABLE_DEBUG_UART  1
#endif
#ifndef DEBUG_BAUD
#define DEBUG_BAUD         9600UL
#endif

/* Heartbeat LED: fast blink during PIR warm-up, short blip every 2 s after,
 * so you can see the firmware is alive. PC13 is only a GUESS (common on
 * "minimum system" boards, usually active-LOW) - set to your board's LED pin.
 * If the green LED on your module is a fixed power LED it cannot blink. */
#ifndef ENABLE_HEARTBEAT_LED
#define ENABLE_HEARTBEAT_LED 1
#endif
#ifndef LED_PORT
#define LED_PORT           2
#define LED_PIN            13         /* PC13 */
#endif
#ifndef LED_ACTIVE_LOW
#define LED_ACTIVE_LOW     1
#endif

/* Manual override button / extra status LEDs: NOT implemented in this release. */

#endif /* CONFIG_H */
