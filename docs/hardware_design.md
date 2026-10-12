# Hardware design

**Status legend:** ✅ verified in this project · ⚠️ believed true, NOT verified here · ❓ unknown, needs your input

> During development the web-research tools were unavailable, so **no figure from the official GigaDevice datasheet / user manual was verified**. Everything marked ⚠️ must be checked against the documents named in the "To verify" list before you trust it.

## 1. MCU

| Item | Value | Status |
|---|---|---|
| Part | GD32F103C8T6, Cortex-M3, LQFP48 | ⚠️ (from your brief) |
| Max clock | 108 MHz | ⚠️ (brief) — **not used**: firmware stays on the reset-default internal oscillator, `SYSCLK_HZ = 8 MHz` |
| Flash / SRAM | 64 KB / 20 KB | ⚠️ (brief). Linker script uses these (smaller = safe). Firmware is ~1 KB |
| Logic level | 3.3 V | ⚠️ treat every GPIO as **3.3 V only**, even if the datasheet marks some pins 5 V tolerant |
| Reset-default clock = 8 MHz IRC | | ⚠️ — if wrong, ms timing is wrong (timers off by a constant factor) |
| Register layout (RCU/GPIO/USART) same as STM32F103 | | ⚠️ — see `src/hal_gd32f103.c` header |
| SysTick registers | ARM architectural | ✅ (Cortex-M3 standard) |
| Toolchain builds & links | `arm-none-eabi`-style, Thumb-2 | ✅ built here with Zig's clang for `cortex_m3`: 1012 bytes, vector table checked |
| Firmware runs on the chip | | ❌ **not tested on hardware** |

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

## 2. Pin assignment (proposal — `include/config.h`)

| Function | MCU pin | Direction | Notes |
|---|---|---|---|
| PIR output | **PA0** | input, pull-down | ❓ voltage must be ≤ 3.3 V (see §4) |
| Light relay IN | **PA1** | push-pull output | OFF level latched *before* output enable |
| Fan relay IN | **PA2** | push-pull output | |
| Debug TX | PA9 (USART0 TX) | AF output | optional, 9600 8N1 |
| SWDIO / SWCLK | PA13 / PA14 | — | **not used by application** |
| BOOT0 | dedicated pin | — | leave per your board; not used |

Physical location (board is Blue Pill style, read from the pinout images supplied by the user; USB connector at the top, see `docs/img/pinout.svg`):

| Pin | Header | Position |
|---|---|---|
| PA0 (A0) | right | 5th from bottom |
| PA1 (A1) | right | 6th from bottom |
| PA2 (A2) | right | 7th from bottom |
| PA9 (A9) | left | 6th from top |
| PC13 | on-board LED (labelled "PC13 LED" in the pinout images; the firmware heartbeat assumes active-LOW, to be confirmed by watching it blink) | |
| 5V / GND / 3V3 | left | bottom three pins |
| SWD | bottom edge 4-pin header | 3V3, SWDIO, SWCLK, GND |

The silkscreen on your board is the final authority. Avoid PA13, PA14, PA15, PB3, PB4 (debug/JTAG), PC13 (typically on-board LED — often fine, but a weak driver), and BOOT0/BOOT1 (PB2).

Not implemented in this release: manual-override button, status LEDs (config comments only).

## 3. Power

| Rail | Requirement | Status |
|---|---|---|
| MCU board | 5 V in via its USB/5 V pin → on-board 3.3 V regulator, **or** 3.3 V direct | ❓ depends on board |
| PIR | module-dependent (HC-SR501: 5–20 V in, 3.3 V logic out — ⚠️ typical, verify yours) | ❓ model unknown |
| Relay module | usually 5 V coil supply (JD-VCC) | ❓ model unknown |
| Supply | one regulated 5 V DC supply (≥1 A suggested; 2 coils ≈ 150–200 mA + PIR + MCU) | ❓ your existing supply |

Use a **common GND** between PIR, MCU and relay-module logic side. If the relay module has an opto-isolator with a separate JD-VCC, remove the JD-VCC jumper only if you provide a separate coil supply — otherwise keep the jumper and share GND (follow the module's own documentation).

## 4. PIR compatibility (❓ model unknown)

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
2. PIR model, supply voltage, measured OUT voltage, output polarity.
3. Relay module model, coil voltage, opto-isolation, active polarity.
4. Lamp wattage/type; fan power rating.
5. Do you own an ST-Link V2 (or other SWD probe)? See `programming.md`.
