# AutoBath-Light-Fan: custom PCB (EasyEDA), rev 1 design package

> **Status: DESIGN DRAFT. Nothing here has been built or reviewed.** Part details were not checked against datasheets (the datasheet lookups were not possible), so every part and footprint below has a "verify" flag. This board switches **230 V mains**: build and test it in stages (section 8) and have a **qualified electrician review the mains section** before it is installed.

## 1. Decisions

| Choice | Decision |
|---|---|
| Microcontroller | Your Blue Pill style GD32F103C8T6 **plugs into two 20-pin sockets** (same pins as today, firmware as tested) |
| Mains | **On this PCB**: input terminal, fuses, varistor, two relays, load terminals |
| Power | **AC-DC 5 V module on the PCB** (isolated, 100-240 VAC in) |
| Assembly | **Through-hole, hand soldering** (order bare boards only) |

![Block diagram](pcb-block-diagram.svg)

Files in this folder:
- [`netlist.csv`](netlist.csv): every signal, the Blue Pill socket pin and where it goes.
- [`bom.csv`](bom.csv): every part with value, footprint hint, notes and a "must verify" flag.
- [`pcb-block-diagram.svg`](pcb-block-diagram.svg): zones, slot and blocks.
- [`PROJECT_INTRO.md`](PROJECT_INTRO.md): the project introduction text for the EasyEDA / OSHWLab page.
- [`EASYEDA_PAGE.md`](EASYEDA_PAGE.md): the ready-to-paste EasyEDA / OSHWLab project page (name, short description, description, tags, license options) and `AutoBath-Light-Fan-cover.png` as the cover image.

## 2. One firmware change for this board

The relay modules used so far are driven **active-LOW**. On this PCB each relay coil is switched by an **NPN transistor**, which is **active-HIGH** (GPIO HIGH = relay ON). Set in `include/config.h`:

```c
#define RELAY_ACTIVE_LOW   0
```

This is also the safer arrangement: a 10 kΩ resistor from each transistor base to GND keeps both relays **OFF** while the microcontroller is in reset or unprogrammed.

## 3. Schematic notes

- **Relay driver (x2):** GPIO (PA1 / PA2) → 1 kΩ → NPN base (S8050 or 2N2222); 10 kΩ base to GND; relay coil between +5 V and the collector; **1N4007 flyback diode across the coil** (cathode to +5 V); emitter to GND.
- **Power:** AC-DC module +5 V → bulk capacitor (470 µF) and 100 nF → relay coils, PIR, and through a **Schottky diode (D3)** into the Blue Pill 5V pin. D3 stops the module back-feeding a PC when a USB cable is connected.
- **Mains path:** `L → F1 (module fuse) → RV1 (varistor) → AC-DC module`; and `L → F2 (load fuse) → K1 COM and K2 COM`; `K1 NO → J_LIGHT (switched L)`, `K2 NO → J_FAN (switched L)`. Neutral goes straight to the module and both load terminals. **PE is only passed through** to the load terminals.
- **PIR:** PA0 through a 1 kΩ series resistor. Add the 10k/20k divider footprints only if your PIR output is 5 V.
- **LDR:** 3V3 → LDR → PA7 → 10 kΩ (on the PCB) → GND.
- **LEDs:** each LED has its 330 Ω on the PCB, with a 2-pin header for a panel LED on short wires. The board's own PC13 LED is on the Blue Pill.
- **Buttons:** 2-pin headers (pin and GND), the firmware uses the internal pull-ups.
- **Optional, not populated:** DHT humidity sensor (`HUMIDITY_ENABLE 0`), digital LDR module, debug UART.

Current budget (5 V rail): two relay coils about 70 mA each, plus the Blue Pill and the PIR, is roughly 300 mA at most, inside a 600 mA module. Do not add large loads to the 5 V rail.

## 4. Building it in EasyEDA

1. Create a project, then a schematic. Place parts from the BOM. Search the library for each part (the standard library and LCSC parts both work; for through-hole parts a library footprint close to the real part is enough). **Compare each footprint with the datasheet drawing** (pin spacing, hole size, body size) before routing.
2. Draw the nets from `netlist.csv`. Give the nets a name that matches the signal column so the PCB view stays readable.
3. In the PCB editor set the board size about **100 mm x 80 mm** (a bit more if the terminals need room). Draw the **board outline**.
4. **Design rules:** create net classes: `MAINS` (L, N, switched L) with a **large clearance (at least 3 mm between mains nets, 6 mm to any low-voltage net)** and track width **2.5 mm or more**; `LV` with normal rules (0.25 mm track, 0.2 mm clearance). Set `+5V` tracks to 0.8-1 mm.
5. **Copper:** if the lamp and fan draw more than about 1 A each, order **2 oz copper** and widen the mains tracks, or move heavy loads off the PCB.
6. Run **DRC** and look at the **JLCPCB DFM** check before ordering.

## 5. Layout rules (important)

- **Two zones:** mains on one side, low voltage on the other, with a **milled isolation slot** (at least 1 mm wide) between them. Draw the slot as a closed shape on the board-outline layer or with the slot-hole tool, then confirm it in the DFM preview.
- **Spacing targets** (conservative hobby design values, not a compliance claim): mains to low-voltage at least **6 mm** copper-free plus the slot; L to N at least **3 mm** (4 mm is better).
- **Relays:** coil pins on the low-voltage side, contact pins on the mains side. No copper between them under the relay body.
- **AC-DC module:** AC pins in the mains zone, DC pins in the low-voltage zone, slot between its two pin groups. No tracks under the module body.
- **No low-voltage track crosses the mains zone**, and no mains copper goes under the Blue Pill.
- **Fuses and varistor close to the input terminal.** Put F1 first.
- **Mounting holes** for plastic M3 standoffs only; keep mains copper well away from the screws.
- **Silkscreen:** print the project name and revision (**AutoBath-Light-Fan R1**) and the date, mark the mains area with "230 V" and a warning, and label every terminal (L, N, PE, LIGHT, FAN).
- **Bathroom:** after assembly and testing, apply conformal coating (not on relays, headers or terminals), and mount the board in a closed enclosure outside the wet zone.

## 6. Before you order (checklist)

- [ ] Every footprint compared with its datasheet drawing.
- [ ] Blue Pill socket row spacing measured on your own board.
- [ ] AC-DC module pinout, size and fuse recommendation checked in its datasheet.
- [ ] Relay contact rating checked against your lamp (inrush) and fan, and F2 sized for them.
- [ ] Terminal blocks rated for 250 V and the load current.
- [ ] DRC clean, the `MAINS` clearance rule applied, slot visible in the DFM preview.
- [ ] Silkscreen readable and polarity marks (LEDs, capacitor, diodes) correct.
- [ ] An electrician has looked at the schematic and layout of the mains section.

## 7. Firmware settings for the PCB

```c
#define RELAY_ACTIVE_LOW   0     /* NPN drivers are active-HIGH */
```
Everything else (pins, LEDs, buttons, LDR) matches the current `config.h`: see `netlist.csv`.

## 8. Bring-up plan (do it in this order)

1. **No mains, no AC-DC module fitted:** inspect, then check for shorts between +5 V and GND and between mains and low-voltage nets with a multimeter.
2. **Low voltage only:** feed 5 V from a bench supply into the +5 V rail. Plug in the Blue Pill, flash and run the LED, button and fault tests (`BathroomController.bat`).
3. **Relays on the bench:** confirm both relays are **OFF at power-up and while the board resets**, and click ON when the controller asks. Check with a continuity meter on the contacts (still no mains).
4. **Mains for the first time:** only with the board **inside a closed enclosure**, through an isolation transformer or a series lamp (bulb limiter) and a correctly rated fuse, with a dummy load (a lamp). Never touch the board while it is powered.
5. **Run it for several days** on the bench with the dashboard watching for resets before installing it.
6. Install with the electrician's check, RCD protection and a closed enclosure.

## 9. What I can and cannot do for you

I can generate the netlist, BOM, rules and checklists (done here), and update them as you pick parts. I cannot place and route inside EasyEDA for you or certify the mains design. If you send a screenshot of the schematic or the PCB layout from EasyEDA, I can review it against these rules.
