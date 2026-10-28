# PCBWay contest page text (copy into the project page)

**Title:** AutoBath: a PIR bathroom light and exhaust-fan controller with its own 230 V relay board

**Theme:** Open Theme (or Edge AI & IoT)

**Short description:** A small GD32 Blue Pill controller that gives a bathroom a 7-minute light timer and a 10-minute exhaust-fan timer, on one 100 x 80 mm PCB with two opto-isolated relays, an on-board 5 V supply and six status LEDs.

## The problem
Bathroom PIR switches turn the light and fan off together after a short fixed delay. The light should go off quickly, but the fan should keep running to clear the damp. Many boards also cannot warn you before the light goes out while you sit still.

## What AutoBath does
- **Two independent timers:** light off 7 min after the last motion, fan off 10 min after the last motion. Every motion restarts both. Times are one line each in `config.h`.
- **Warning blink:** 30 s before the light times out it blinks once, wave a hand to restart the 7 minutes.
- **Fan start delay, override and enable buttons:** light override for 30 min, fan enable/disable saved in flash.
- **Daylight sensor (LDR):** in bright daylight the light stays off but the fan still runs.
- **Optional DHT22/11 humidity input** starts the fan on humid air.
- **Six status LEDs:** PIR, LIGHT, FAN, FAULT (stuck PIR or sensor failure), FAN-EN, SYSTEM heartbeat. A silent mode turns them off.
- **Watchdog**, 60 s PIR warm-up, relays default OFF at reset.

## The hardware
- GD32F103C8T6 "Blue Pill" (soldered in), PIR, optional LDR and DHT on 3-pin headers.
- Each relay is driven by a PC-817 optocoupler and an S8050 transistor (active LOW). A red indicator LED sits in series with each opto, so you see the command.
- HLK-5M05 gives 5 V from mains. Relay contacts go to 3-way screw terminals (COM, NO, NC).
- 100 x 80 mm, 2 layers, GND pour, 3 mm clearance rule on the mains nets, no copper under the relay contacts.
- Custom 3D models and footprints for the Blue Pill and HLK-5M05 are open source: https://github.com/ElectroIoT/EasyEDA-3D-Parts

## Files
Gerber, EasyEDA project, schematic (PDF/SVG), BOM, netlist and the firmware are in the repository:
https://github.com/ElectroIoT/Smart-Bathroom-Light-Fan-Controller-GD32

## Honest status
- Firmware: written in bare registers, tested on the author's own build.
- PCB: designed in EasyEDA Pro, DRC clean (0 errors), **not built yet**, so not tested.
- It switches 230 V: it has no fuse and no milled isolation slot yet. Have a qualified electrician check the mains side before use.

## Images to upload
`pcb-top-3d.png`, `pcb-iso-3d.png`, `how-it-works.gif` (timers animation), `banner.svg` (use a screenshot), schematic PDF.
