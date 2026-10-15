# EasyEDA / OSHWLab project page: AutoBath-Light-Fan

Copy each block into the matching field when you create the project on OSHWLab (Project name, Introduction / Description, Tags, License). Cover image: `AutoBath-Light-Fan-cover.png` in this folder.

## Project name
AutoBath-Light-Fan R1

## Short description (one or two lines)
Smart bathroom light and exhaust-fan controller: one PIR sensor, two independent timers, two relays, on-board 5 V supply. Open-source firmware (GD32F103 Blue Pill).

## Description (paste this)

**AutoBath-Light-Fan R1: Smart Bathroom Light & Exhaust Fan Controller**

A normal PIR light switch uses one fixed delay, and it switches the light off on someone who sits still. AutoBath-Light-Fan replaces that delay with a small microcontroller that runs **two independent timers** from one PIR sensor.

**How it behaves**
- Someone walks in: the **light turns on at once**, and the **fan starts 30 seconds later**.
- The light switches off **7 minutes** after the last movement, the fan after **10 minutes**. Each has its own countdown, so turning the light off never turns the fan off.
- About **30 seconds before the light goes off it blinks off briefly**: wave your hand to keep it on.
- Optional **daylight lockout** (bare LDR): in bright daylight the light stays off but the fan still runs. Optional **humidity sensor** keeps the fan running while the air is humid.

**What is on the board**
- Socket for a **Blue Pill style GD32F103C8T6** board (the firmware runs unchanged on it).
- **Two relays** switching the lamp and the fan (phase wire only), with fuses and a varistor.
- **On-board 5 V AC-DC module** (isolated), no external adapter needed.
- Relay drivers with pull-downs so both relays stay **off while the chip resets**.
- Connectors for the PIR, LDR, optional DHT humidity sensor, two push buttons, six panel LEDs and a debug serial port.

**Controls and indicators**
- Button 1: light override (light on for 30 minutes). Button 2: fan enable / disable, remembered after a power cut.
- Six LEDs: motion, light relay, fan relay, fan enabled, heartbeat and a red fault LED. Hold both buttons for 5 seconds and all LEDs glow once, then every LED turns off except the fault LED (hold again to turn them back on).

**Firmware and tools** (open source, GitHub link below): written in C, with a PC test suite (470 checks), a watchdog, and a live test dashboard that shows motion, relays, LEDs and switches in real time through an ST-Link.

**Status**
- The firmware is tested on a real board with a PIR, an LDR, LEDs, buttons and relays.
- **This PCB is a design draft and has not been built or reviewed yet.** Part footprints must be checked against datasheets.
- **The board switches 230 V mains.** Build it into a closed enclosure, test with no mains first, and have a qualified electrician review the mains section before installation.

Code, documents, netlist and BOM: https://github.com/ElectroIoT/Smart-Bathroom-Light-Fan-Controller-GD32

## Tags
bathroom, exhaust fan, light controller, PIR, motion sensor, relay, home automation, GD32, Blue Pill, STM32, ESP-free, mains, open source, timer

## License (choose one)
Not decided. Common choices: **CERN-OHL-P v2** or **CC BY-SA 4.0** for the hardware files, and **MIT** for the firmware. The GitHub repository does not have a LICENSE file yet: tell me your choice and I will add it.

## Suggested images for the project page
1. `AutoBath-Light-Fan-cover.png` (block diagram, as the cover).
2. Screenshot of the schematic and of the PCB layout from EasyEDA (when ready).
3. `../img/dashboard.png` (the live test dashboard).
4. `../img/connections.svg` (connections overview).
