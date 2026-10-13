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

/* External status LEDs (e.g. mounted on the outside of the enclosure).
 * Wire each as:  GPIO -> 330 ohm..1 kOhm resistor -> LED -> GND  (active-HIGH).
 *   PIR LED   : lit while the PIR reports motion (solid if held HIGH)
 *   LIGHT LED : lit while the light relay is commanded ON
 *   FAN LED   : lit while the fan relay is commanded ON
 *   FAULT LED : fast blink = PIR stuck HIGH
 * Put the resistor at the LED end of a long wire. 0 disables all four. */
#ifndef ENABLE_STATUS_LEDS
#define ENABLE_STATUS_LEDS 1
#endif
#ifndef LED_PIR_PORT
#define LED_PIR_PORT       0
#define LED_PIR_PIN        3          /* PA3 */
#endif
#ifndef LED_LIGHT_PORT
#define LED_LIGHT_PORT     0
#define LED_LIGHT_PIN      4          /* PA4 */
#endif
#ifndef LED_FAN_PORT
#define LED_FAN_PORT       0
#define LED_FAN_PIN        5          /* PA5 */
#endif
#ifndef LED_FAULT_PORT
#define LED_FAULT_PORT     0
#define LED_FAULT_PIN      6          /* PA6 */
#endif
/* External SYSTEM heartbeat LED (blinks together with the on-board PC13 LED):
 * GPIO -> 330 ohm -> LED -> GND, active-HIGH. Same pattern as PC13: fast blink
 * during the PIR warm-up, then a 150 ms blink once per second. */
#ifndef LED_SYSEXT_PORT
#define LED_SYSEXT_PORT    1
#define LED_SYSEXT_PIN     13         /* PB13 */
#endif
#ifndef STATUS_LED_ACTIVE_LOW
#define STATUS_LED_ACTIVE_LOW 0
#endif
#ifndef FAULT_BLINK_MS
#define FAULT_BLINK_MS     125UL      /* toggle period of the fault LED */
#endif

/* ------------------------------------------------------------------ */
/* Warning blink, fan start delay, override, fan enable button          */
/* ------------------------------------------------------------------ */
/* Light switches OFF for LIGHT_WARN_BLINK_MS this long before it times
 * out, so a still person can wave to restart it. 0 disables the warning. */
#ifndef LIGHT_WARN_BEFORE_MS
#define LIGHT_WARN_BEFORE_MS  30000UL
#endif
#ifndef LIGHT_WARN_BLINK_MS
#define LIGHT_WARN_BLINK_MS   600UL
#endif

/* Fan starts this long after the light came ON (avoids the fan for a quick
 * walk-in). Once started it runs for FAN_TIMEOUT_MS after the last motion.
 * 0 = fan starts immediately with the light. */
#ifndef FAN_START_DELAY_MS
#define FAN_START_DELAY_MS    30000UL
#endif

/* Light-override button: press = light ON for this long regardless of the
 * PIR (fan keeps its own logic). Press again to cancel. */
#ifndef LIGHT_OVERRIDE_MS
#define LIGHT_OVERRIDE_MS     1800000UL   /* 30 min */
#endif

/* Buttons wired between the pin and GND (internal pull-up used). */
#ifndef BUTTON_ACTIVE_LOW
#define BUTTON_ACTIVE_LOW     1
#endif
#ifndef BUTTON_DEBOUNCE_MS
#define BUTTON_DEBOUNCE_MS    40UL
#endif
/* LED silent mode: hold BOTH buttons for LED_SILENT_HOLD_MS to switch every LED off
 * except the red FAULT LED (hold again to switch them back on). All LEDs glow
 * together for LED_CONFIRM_MS as confirmation. The choice is saved in flash.
 * A single press still works: it is accepted after BUTTON_CHORD_WINDOW_MS so that
 * pressing both buttons together is not mistaken for two single presses. */
#ifndef LED_SILENT_HOLD_MS
#define LED_SILENT_HOLD_MS    5000UL
#endif
#ifndef LED_CONFIRM_MS
#define LED_CONFIRM_MS        1500UL
#endif
#ifndef BUTTON_CHORD_WINDOW_MS
#define BUTTON_CHORD_WINDOW_MS 300UL
#endif
#ifndef BTN_LIGHT_PORT
#define BTN_LIGHT_PORT        1
#define BTN_LIGHT_PIN         0           /* PB0 : light override */
#endif
#ifndef BTN_FAN_PORT
#define BTN_FAN_PORT          1
#define BTN_FAN_PIN           1           /* PB1 : fan enable / disable */
#endif

/* FAN-ENABLED LED: lit = fan active (works with the PIR), off = fan
 * disabled by the button. State is saved in flash and survives power loss. */
#ifndef LED_FANEN_PORT
#define LED_FANEN_PORT        1
#define LED_FANEN_PIN         11          /* PB11 (PA7 is the LDR ADC input) */
#endif

/* Flash page used to remember the fan enable state (last 1 KB page of the
 * 64 KB region; the linker script keeps firmware out of it). */
#ifndef STORE_PAGE_ADDR
#define STORE_PAGE_ADDR       0x0800FC00UL
#endif

/* ------------------------------------------------------------------ */
/* Optional humidity sensor (DHT11 / DHT22, one data wire)              */
/* ------------------------------------------------------------------ */
/* >>> HUMIDITY SENSOR ON/OFF SWITCH <<<
 *   HUMIDITY_ENABLE 0 = sensor NOT fitted: the feature is completely off
 *                       (PB12 is not touched, no readings, no humidity logic).
 *   HUMIDITY_ENABLE 1 = sensor fitted: set HUMIDITY_TYPE to 11 (DHT11) or 22 (DHT22).
 * With a sensor the fan is held ON while the air is humid and (optionally) started
 * by humidity alone. A reading takes about 4 ms once every HUMIDITY_POLL_MS. */
#ifndef HUMIDITY_ENABLE
#define HUMIDITY_ENABLE       0           /* 0 = off (no sensor added yet), 1 = on */
#endif
#ifndef HUMIDITY_TYPE
#define HUMIDITY_TYPE         22          /* 11 = DHT11, 22 = DHT22 / AM2302 */
#endif
#ifndef HUMIDITY_SENSOR                    /* derived: 0 = off, otherwise 11 or 22 (tests may force it) */
#define HUMIDITY_SENSOR       (HUMIDITY_ENABLE ? HUMIDITY_TYPE : 0)
#endif
#ifndef HUMIDITY_PORT
#define HUMIDITY_PORT         1
#define HUMIDITY_PIN          12          /* PB12 (needs the sensor's pull-up) */
#endif
#ifndef HUMIDITY_POLL_MS
#define HUMIDITY_POLL_MS      5000UL      /* DHT11 >= 1 s, DHT22 >= 2 s */
#endif
#ifndef HUMIDITY_ON_PCT
#define HUMIDITY_ON_PCT       70          /* humid at or above this RH %       */
#endif
#ifndef HUMIDITY_OFF_PCT
#define HUMIDITY_OFF_PCT      62          /* back to normal at or below this   */
#endif
#ifndef HUMIDITY_AUTOSTART
#define HUMIDITY_AUTOSTART    1           /* humidity alone may start the fan  */
#endif
#ifndef HUMIDITY_MAX_RUN_MS
#define HUMIDITY_MAX_RUN_MS   3600000UL   /* humidity holds the fan at most 60 min */
#endif
#ifndef HUMIDITY_FAIL_LIMIT
#define HUMIDITY_FAIL_LIMIT   3           /* failed reads in a row = sensor fault */
#endif

/* ------------------------------------------------------------------ */
/* Optional daylight sensor (bare LDR on ADC, or digital LDR module)    */
/* ------------------------------------------------------------------ */
/* 0 = off, 1 = digital LDR module (DO pin on PB10), 2 = BARE LDR on PA7 (ADC).
 * Daylight lockout: when it is bright (window), a new visit does NOT
 * switch the light on; the fan still works. The override button always
 * turns the light on. The sensor is read only while the lamp is OFF, and
 * a missing or broken sensor reads as dark (light works). 0 = off. */
#ifndef DAYLIGHT_SENSOR
#define DAYLIGHT_SENSOR       2           /* 2 = bare LDR fitted on PA7 (this build); 0 = no sensor */
#endif
#ifndef DAYLIGHT_PORT
#define DAYLIGHT_PORT         1
#define DAYLIGHT_PIN          10          /* PB10 (B10): module DO pin */
#endif
/* Pin level that means BRIGHT. Many LDR modules output LOW in bright light:
 * check yours and set 0 or 1. */
/* Bare LDR (DAYLIGHT_SENSOR 2): 3V3 -> LDR -> PA7 -> 10 kOhm -> GND.
 * Brighter = higher ADC value (0..4095). Hysteresis: bright at or above
 * LDR_BRIGHT_ADC, dark at or below LDR_DARK_ADC. Tune the two numbers
 * from the debug output (LDR=...). LDR_INVERT 1 if wired the other way. */
#ifndef LDR_PORT
#define LDR_PORT              0
#define LDR_PIN               7           /* PA7 = ADC channel 7 */
#define LDR_ADC_CHANNEL       7
#endif
#ifndef LDR_BRIGHT_ADC
#define LDR_BRIGHT_ADC        2000
#endif
#ifndef LDR_DARK_ADC
#define LDR_DARK_ADC          1200
#endif
#ifndef LDR_INVERT
#define LDR_INVERT            0
#endif
#ifndef LDR_POLL_MS
#define LDR_POLL_MS           100UL
#endif
#ifndef LDR_DEBUG_MS
#define LDR_DEBUG_MS          5000UL      /* print LDR=... on the debug UART (0 = never) */
#endif
#ifndef DAYLIGHT_BRIGHT_LEVEL
#define DAYLIGHT_BRIGHT_LEVEL 1
#endif
/* Reading must stay the same this long before it counts (clouds, shadows). */
#ifndef DAYLIGHT_FILTER_MS
#define DAYLIGHT_FILTER_MS    10000UL
#endif

/* ------------------------------------------------------------------ */
/* Watchdog                                                            */
/* ------------------------------------------------------------------ */
/* Independent watchdog: if the main loop hangs the chip resets and the
 * relays go back to OFF. Runs from the internal low-speed oscillator.
 * NOTE: the program is halted by a debugger -> chip may reset after this
 * time unless the debug-freeze bit works on your chip. */
#ifndef ENABLE_WATCHDOG
#define ENABLE_WATCHDOG       1
#endif
#ifndef WATCHDOG_TIMEOUT_MS
#define WATCHDOG_TIMEOUT_MS   4000UL
#endif

#endif /* CONFIG_H */
