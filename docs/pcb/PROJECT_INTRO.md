# AutoBath-Light-Fan R1: Smart Bathroom Light & Exhaust Fan Controller

*An open-source board that switches a bathroom light and exhaust fan from one PIR sensor, with a separate timer for each.*

(Text for the EasyEDA / OSHWLab project page. The name was chosen by the project owner; a trademark or name clash search has not been done.)

## Introduction
AutoBath-Light-Fan replaces the fixed delay of a normal PIR light switch with a small microcontroller (a GD32F103C8T6 on a Blue Pill style board, which plugs into the PCB). When someone walks in, the light turns on at once and the fan starts 30 seconds later. The light switches off 7 minutes after the last movement and the fan after 10 minutes, each on its own timer. About 30 seconds before the light goes off, it blinks off briefly: if you are sitting still, wave your hand to keep it on, which solves the usual "PIR loses a motionless person" problem.

## Features
- **Two relays** switch the lamp and the fan (phase wire only), with fuses and surge protection on the board.
- **On-board 5 V supply** from the mains (isolated AC-DC module), so no external adapter is needed.
- **Two push buttons:** light override (holds the light on for 30 minutes) and fan enable/disable (remembered after a power cut).
- **Six status LEDs** (motion, light, fan, fan-enabled, heartbeat, red fault). Holding both buttons for 5 seconds turns every LED off except the fault LED.
- **Optional sensors:** a bare LDR for a daylight lockout (the light stays off in bright daylight while the fan still runs) and a humidity sensor that keeps the fan running while the air is humid.
- **Safety:** relays stay off while the chip resets, and a watchdog restarts the chip if the firmware ever hangs.
- **Open-source firmware** with a PC test suite and a live test dashboard.

## Status
- The **firmware is tested** on a real board with a PIR, an LDR, LEDs, buttons and relays.
- The **PCB is a draft**: it has not been built or reviewed yet.
- **Mains safety:** this board switches **230 V mains**. Test it with no mains first, build it into a closed enclosure, and have a qualified electrician review it before installation.

Code, docs and PCB design files: https://github.com/ElectroIoT/Smart-Bathroom-Light-Fan-Controller-GD32
