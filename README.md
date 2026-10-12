# Smart Bathroom Light & Exhaust Fan Controller (GD32F103C8T6, PIR)

Replaces a PIR's fixed delay with two independent non-blocking timers:

| Output | Default timeout (after the most recent motion) |
|---|---|
| Light | 7 min (`LIGHT_TIMEOUT_MS = 420000`) |
| Exhaust fan | 10 min (`FAN_TIMEOUT_MS = 600000`) |

Any motion turns both ON and restarts both countdowns. Turning the light off never turns the fan off. All settings live in [`include/config.h`](include/config.h).

> **Limitation:** a PIR cannot reliably detect a completely stationary person. The longer timeout reduces, but does not eliminate, the light switching off on someone sitting still.

## Status (honest)
| Item | State |
|---|---|
| Control logic + unit tests (51 checks × 4 configs) | ✅ passing on PC |
| Firmware compiles/links for Cortex-M3 (1012 bytes) | ✅ |
| GD32 register addresses, clock default, pin mapping | ⚠️ unverified — datasheet/manual lookup was not possible; see [hardware design](docs/hardware_design.md) |
| Running on real hardware | ❌ not tested |
| PIR / relay module specifics | ❓ unknown — configurable |

## Hardware
GD32F103C8T6 board · your PIR · 2-channel relay module · regulated supply · ST-Link V2 (recommended) · optional USB-serial for debug.
Pins: PA0 PIR in, PA1 light relay, PA2 fan relay, PA9 debug TX. Wiring: [docs/wiring_diagram.svg](docs/wiring_diagram.svg).

## Layout
```
include/config.h          all tunables, pins, polarities
src/controller.[ch]       pure logic (no hardware)
src/app.[ch]              glue: PIR polarity, relay polarity, safe init
src/hal.h                 hardware interface
src/hal_gd32f103.c        register-level GD32 implementation
src/startup.c, linker/    vector table, linker script
tests/                    host tests with a fake HAL
tools/build.py            build + test without make/cmake
docs/                     hardware design, programming, test plan, wiring
```

## Build & test
Requires Python 3 and either `arm-none-eabi-gcc` / `gcc`, or `pip install ziglang`.
```
python tools/build.py test        # logic tests, 4 polarity variants
python tools/build.py firmware    # build/firmware.elf, build/firmware.bin
```
Flashing and recovery: [docs/programming.md](docs/programming.md). Bench procedure: [docs/test_plan.md](docs/test_plan.md).

**Toolchain choice:** plain C + `arm-none-eabi-gcc` with direct register access. Advantage: no dependence on an unverified STM32-compatible core, tiny, easy to read. Limitation: register map must be checked against the GD32 manual (listed in the design doc); PlatformIO has no official GD32F103 board (checked), so it is not used.

## Safety
Do not connect mains until every bench test passes, and have the 230 V wiring done or inspected by a qualified electrician. See the design doc §6.
