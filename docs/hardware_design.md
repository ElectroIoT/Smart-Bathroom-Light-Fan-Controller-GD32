# Hardware design

**Status legend:** ✅ verified in this project · ⚠️ believed true, NOT verified here · ❓ unknown, needs your input

> During development the web-research tools were unavailable, so **no figure from the official GigaDevice datasheet / user manual was verified**. Everything marked ⚠️ must be checked against the documents named in the "To verify" list before you trust it.

## 1. MCU

| Item | Value | Status |
|---|---|---|
| Part | GD32F103C8T6, Cortex-M3, LQFP48 | ⚠️ (from your brief) |
| Max clock | 108 MHz | ⚠️ (brief) — **not used**: firmware stays on the reset-default internal oscillator, `SYSCLK_HZ = 8 MHz` |
| Flash / SRAM | 64 KB / 20 KB | ⚠️ (brief). Linker script uses these (smaller = safe; the last 1 KB page is reserved for saved settings). Firmware is about 3 KB. The chip reported 128 KiB when probed |
| Logic level | 3.3 V | ⚠️ treat every GPIO as **3.3 V only**, even if the datasheet marks some pins 5 V tolerant |
| Reset-default clock = 8 MHz IRC | | ⚠️ — if wrong, ms timing is wrong (timers off by a constant factor) |
| Register layout (RCU/GPIO/USART) same as STM32F103 | | ⚠️ — see `src/hal_gd32f103.c` header |
| SysTick registers | ARM architectural | ✅ (Cortex-M3 standard) |
| Toolchain builds & links | `arm-none-eabi`-style, Thumb-2 | ✅ built here with Zig's clang for `cortex_m3`: about 3 KB, vector table checked |
| Firmware runs on the chip | | ✅ flashed and running on the real board (GPIO, clock, SysTick, watchdog read back). ❌ PIR, relays, buttons, LEDs and optional sensors **not bench-tested yet**; flash write, ADC and DHT code untested |

### To verify (official GigaDevice documents: GD32F103 datasheet + GD32F10x user manual)
1. Absolute-max / recommended GPIO voltage; which pins are 5 V tolerant.
2. RCU_APB2EN address (0x40021018), GPIOx base (0x40010800 + 0x400·n), CTL0/CTL1/ISTAT/BOP offsets, USART0 base 0x40013800.
3. Default system clock after reset is the 8 MHz IRC.
4. PA13/PA14 = SWDIO/SWCLK; BOOT0 behaviour; USART0 TX = PA9.
5. Real Flash size of *your* chip (some "C8" parts report more).

### On-chip check (2026-10-12, ST-Link V2 + OpenOCD, board powered, PIR/relays not connected)
* ✅ Cortex-M3 r2p1 detected over SWD (DPIDR 0x2ba01477); probe saw target voltage 3.19 V.
* ✅ Flash read back as **128 KiB** (the 64 KB linker limit is therefore conservative). Firmware (1012 B) programmed and verified.
* ✅ Running firmware matches the assumed register map: RCU_APB2EN = GPIOA + USART0 enabled; GPIOA_CTL0 = PA1/PA2 push-pull output, PA0 input; GPIOA_OCTL has PA1/PA2 = 1 (relay-OFF level for active-LOW) and PA0 pull-down; SysTick enabled with interrupt.
* ✅ RCU_CFG0 = 0: system clock is the internal oscillator, no PLL (so the "reset-default IRC" assumption holds; its exact 8 MHz value is still to be confirmed with the stopwatch timer test).
* Still untested: PIR input, relay switching, real 7/10 min timing.

## 2. Final pin assignment (`include/config.h`)

| Function | MCU pin | Silkscreen | Direction / setting | Notes |
|---|---|---|---|---|
| PIR output | **PA0** | A0 | input, pull-down | ⚠️ must be ≤ 3.3 V (see §4) |
| Light relay IN1 | **PA1** | A1 | push-pull output | OFF level latched *before* output enable |
| Fan relay IN2 | **PA2** | A2 | push-pull output | |
| PIR status LED | **PA3** | A3 | output, active-HIGH | 3 mm LED outside the box: GPIO → 330 Ω → LED → GND |
| Light relay LED | **PA4** | A4 | output, active-HIGH | same wiring |
| Fan relay LED | **PA5** | A5 | output, active-HIGH | same wiring |
| Fault LED | **PA6** | A6 | output, active-HIGH | same wiring; fast blink = PIR stuck, steady = humidity sensor fault |
| LDR analog input (option) | **PA7** | A7 | analog (ADC1 ch 7) | 3V3 → LDR → PA7 → 10 kΩ → GND |
| Light override button | **PB0** | B0 | input, pull-up | normally-open button to GND |
| Fan enable/disable button | **PB1** | B1 | input, pull-up | normally-open button to GND |
| LDR digital module DO (alt. option) | **PB10** | B10 | input, pull toward dark | `DAYLIGHT_SENSOR 1` |
| FAN-ENABLED LED | **PB11** | B11 | output, active-HIGH | lit = fan enabled |
| DHT22 / DHT11 data (option) | **PB12** | B12 | single-wire | 3V3 supply, 4.7–10 kΩ pull-up |
| Debug text (option) | **PA9** (USART0 TX) | A9 | AF output, 9600 8N1 | 3.3 V USB-serial RX |
| System heartbeat LED (external) | **PB13** | B13 | output, active-HIGH | 3 mm LED: GPIO → 330 Ω → LED → GND; blinks together with the on-board LED |
| System heartbeat LED (on-board) | **PC13** | on-board LED | output, active-LOW (assumed) | |
| ST-Link | PA13 / PA14 | SWDIO / SWCLK | SWD header | **not used by the application** |
| BOOT0 / BOOT1 | dedicated | jumpers | leave both at 0 | |

Physical location (board is Blue Pill style, read from the pinout images supplied by the user; USB connector at the top, see `docs/img/pinout.svg`):

| Pin | Header | Position |
|---|---|---|
| PA0 (A0) | right | 5th from bottom |
| PA1 (A1), PA2 (A2) | right | 6th and 7th from bottom |
| PA3 to PA7 | right | the pins above PA2, in order |
| PB0, PB1 | right | above PA7 (PB0 then PB1), see the pinout drawing |
| PB10, PB11 | right | 6th and 5th from the top |
| PB12 (B12) | left | 1st from top |
| PA9 (A9) | left | 6th from top |
| PC13 | on-board LED (labelled "PC13 LED" in the pinout images) | |
| 5V / GND / 3V3 | left | bottom three pins |
| SWD | bottom edge 4-pin header | 3V3, SWDIO, SWCLK, GND |

The silkscreen on your board is the final authority. The firmware avoids PA13, PA14, PA15, PB3, PB4 (debug/JTAG) and BOOT0/BOOT1 (PB2).

**Saved settings:** the fan enable flag and the LED silent-mode flag are stored in flash page `0x0800FC00` (three 16-bit values: magic `0x5AA5`, fan, LED mode; an older page without the third value means LED mode normal). The setting page (the last 1 KB page of the first 64 KB; the linker reserves it). ⚠️ The flash controller registers (FMC) and the independent watchdog (FWDGT) use the STM32-compatible layout and have been seen working (watchdog starts, flag page readable) but flash *writing* from the button has not yet been tested on hardware.

**Daylight sensor (optional, default off).** `DAYLIGHT_SENSOR` = 2: bare LDR on PA7 (ADC1 channel 7): 3V3 → LDR → PA7 → 10 kΩ → GND, thresholds `LDR_BRIGHT_ADC` / `LDR_DARK_ADC` (hysteresis), tuned from the debug print; the ADC code is unverified against the GD32 manual and untested on hardware. `DAYLIGHT_SENSOR` = 1: digital LDR module on PB10 (see `docs/img/ldr-wiring.svg`): VCC to 3V3, GND, DO to PB10, AO unused. Bright = a new visit does not switch the lamp on (decided once per visit); the fan is unaffected; the override button always works; a missing sensor reads dark. Sampled only while the lamp is off. Polarity set by `DAYLIGHT_BRIGHT_LEVEL`. **Untested on hardware.**

**Humidity sensor (optional, default off: `HUMIDITY_ENABLE 0`; to fit one set `HUMIDITY_ENABLE 1` and `HUMIDITY_TYPE` 11 or 22).** Single-wire DHT11/DHT22 on PB12 (see `docs/img/humidity-wiring.svg`), powered from 3V3 with a 4.7-10 kΩ pull-up (3-pin modules include it). DHT11 is only about ±5 % RH, reads at most once per second and dislikes condensation; DHT22 is better. The bit-bang read is timed with SysTick (about 4 ms every 5 s) and is **untested on hardware**. An I2C sensor (AHT20/SHT31) would be more robust and could be added later.

## 3. Power

| Rail | Requirement | Status |
|---|---|---|
| MCU board | 5 V in via its USB/5 V pin → on-board 3.3 V regulator, **or** 3.3 V direct | ❓ depends on board |
| PIR | HC-SR501 type: 5–20 V in (use the 5V pin); output normally about 3.3 V | ⚠️ typical, measure yours |
| Relay module | usually 5 V coil supply (JD-VCC) | ❓ model unknown |
| Supply | one regulated 5 V DC supply (≥1 A suggested; 2 coils ≈ 150–200 mA + PIR + MCU) | ❓ your existing supply |

Use a **common GND** between PIR, MCU and relay-module logic side. If the relay module has an opto-isolator with a separate JD-VCC, remove the JD-VCC jumper only if you provide a separate coil supply — otherwise keep the jumper and share GND (follow the module's own documentation).

## 4. PIR compatibility (HC-SR501-type module, from the user's photo)

Module: 3 pins (5–20 V, OUT, GND), TIME dial, SENSITIVITY dial and an H/L jumper (see `docs/img/pir-setup.svg`). Settings: TIME dial to minimum, jumper at H (repeat trigger), sensitivity mid. Module specs were read from the photo only; the output voltage is **still to be measured**.

Before connecting PA0, measure the PIR OUT pin with a multimeter, PIR powered, MCU **not** connected:
* ≤ 3.3 V when active → connect directly.
* ≈ 5 V when active → do **not** connect directly. Use a resistor divider (e.g. 10 kΩ top / 20 kΩ bottom gives ≈ 3.3 V from 5 V) or a level shifter.
* Open-collector/open-drain output → needs pull-up to 3.3 V; set `PIR_PULL = 2`.
* Output polarity: set `PIR_ACTIVE_HIGH`.
* Many PIRs output HIGH for 30–60 s after power-up → handled by `PIR_WARMUP_MS`.
* Set the PIR's own hold-time pot to **minimum** and (if available) retrigger mode on, so the MCU timers — not the PIR — set the delay.

## 5. Relay module compatibility (❓ model unknown)

Send the module's model/photo to finalise. Until then these are configurable:
* **Polarity:** `RELAY_ACTIVE_LOW` (1 = energises when input LOW). Verify with the bench test in `test_plan.md` §3 before any load.
* **Input threshold:** a 5 V module may not turn on/off reliably from a 3.3 V GPIO. Test both states; if unreliable use a transistor/level-shifter or a module documented for 3.3 V logic.
* **Never** power the coil from a GPIO. GPIO only drives the module's input (opto LED / transistor base).
* **Unpowered/reset state:** a GPIO is floating during reset and bootloader. Add a **10 kΩ resistor from each relay input to the module's "off" level** (to +5 V/VCC for active-LOW modules, to GND for active-HIGH) — ⚠️ check the module's own input network first, since many already include one.
* **Contact rating:** check the printed rating (commonly 10 A/250 VAC resistive) is for *resistive* loads. Derate for: LED driver inrush (can be many times the steady current) and the fan motor (inductive). ❓ need your lamp wattage and fan rating.

## 6. Safety

* Low-voltage side (PIR, MCU, relay inputs) and 230 V side (relay contacts, loads) are separate subsystems. **No mains on breadboards or loose wires.**
* Mains wiring: suitable insulated enclosure, strain relief, correct terminals/clearances, fuse or MCB protection, correct conductor sizes. **Have a qualified electrician install or inspect it.**
* The controller switches **live (phase)** only through relay contacts, never neutral/earth.
* Fail-safe: on reset/power-up relays are OFF in firmware; the external pull resistors in §5 keep them OFF while the MCU pins float.
* Watch for: relay contact welding with inductive loads (consider an RC snubber / MOV across the fan contacts if sparking or glitches appear).
* **PIR limitation:** a PIR cannot reliably detect a completely stationary person. This controller only extends timers when motion is seen; it does not guarantee occupancy detection, and the lights may still go off on a motionless occupant (just with a longer, 7-minute margin than before).

## 7. Open questions
1. Exact GD32 board name / header labels / USB connector function.
2. PIR: type is HC-SR501-style; measured OUT voltage and output polarity still to be confirmed.
3. Relay module model, coil voltage, opto-isolation, active polarity.
4. Lamp wattage/type; fan power rating.
5. Do you own an ST-Link V2 (or other SWD probe)? See `programming.md`.
