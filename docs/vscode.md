# Getting started with VS Code

Everything runs from VS Code with four one-click tasks. No Arduino IDE or PlatformIO is needed.

## 1. Install once

| Tool | Windows | Linux | macOS |
|---|---|---|---|
| **VS Code** | code.visualstudio.com | same | same |
| **Python 3** | python.org (tick "Add to PATH") | `sudo apt install python3` | `brew install python` |
| **Compiler** (pick one) | `pip install ziglang` (simplest) or the Arm GNU Toolchain | same | same |
| **OpenOCD** (flashing) | `winget install xpack-dev-tools.openocd-xpack` | `sudo apt install openocd` | `brew install openocd` |
| **ST-Link V2** driver | ST's ST-LINK driver (the probe shows as "STM32 STLink") | udev rules for ST-Link | none |

On Linux or macOS, if `python` is not found, change `python` to `python3` in `.vscode/tasks.json`.
When VS Code asks, accept the recommended extensions (C/C++ and Python).

## 2. Open the project
File → Open Folder → the repository folder.

## 3. Use it (Terminal → Run Task...)

| Task | What it does |
|---|---|
| **1. Test (PC logic tests)** | Runs all automated tests. Every line should end in `0 failed` and then `ALL TESTS PASSED`. |
| **2. Build firmware** | Creates `build/firmware.elf` and `build/firmware.bin`. Also the default build shortcut: `Ctrl+Shift+B`. |
| **3. Flash to board (ST-Link)** | Builds, then programs and verifies the board and resets it. |
| **Probe board (read only, no flashing)** | Checks that the ST-Link can see the chip. Good first test after wiring. |

| **Live status (PIR, LDR, timers via ST-Link)** | Reads the running board through the ST-Link for 20 s and prints the raw PIR pin, the LDR ADC value, daylight state, relays, timers and buttons. No serial adapter needed. Wave at the PIR or cover the LDR while it runs. |

Build with an option on, for example the bare LDR: in a terminal run `FW_DEFINES="-DDAYLIGHT_SENSOR=2" python tools/build.py firmware` (Windows PowerShell: `$env:FW_DEFINES="-DDAYLIGHT_SENSOR=2"; python tools/build.py firmware`), or simply edit `include/config.h`.

## 4. Change a setting
Open `include/config.h`, edit a value (for example `LIGHT_TIMEOUT_MS`, `RELAY_ACTIVE_LOW`, `HUMIDITY_SENSOR`), then run task 3. The settings file is the only place you normally need to edit.

## 5. Wiring and first flash
1. Wire the ST-Link to the board's 4-pin SWD header by name (see the diagram in the README).
2. Run **Probe board**. You should see the chip detected and its flash size.
3. Run **3. Flash to board**. You should see `Verified OK`.
4. The system LED (PC13) blinks fast for 60 s while the PIR warms up.

If something fails, see [programming.md](programming.md) (recovery steps).
