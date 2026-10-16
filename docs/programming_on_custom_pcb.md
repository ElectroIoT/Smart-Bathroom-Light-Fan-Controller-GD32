# Programming and first power-up of the AutoBath PCB

> Written from the schematic, not from a built board. Nothing here is tested on hardware yet.

The board has two 5 V sources that must never feed each other: the **HLK-5M05** (from mains) and the Blue Pill's **USB connector** (from the PC). On the PCB the HLK's +5V net is wired to the Blue Pill 5V pin, so a USB cable plugged in while mains is on connects both together.

## Rule
**Mains OFF and unplugged, and no USB cable, whenever you touch the board or flash it.**
Mains and USB are never connected at the same time.

## A. Flash with the Blue Pill fitted on the board (recommended)
1. Unplug the mains cord. Wait one minute.
2. Make sure no USB cable is in the Blue Pill.
3. Connect the ST-Link V2 to the Blue Pill SWD header: **SWDIO, SWCLK, GND, 3V3** (match by name, see `docs/img/stlink-connection.svg`).
4. The ST-Link's 3V3 powers only the Blue Pill and the 3.3 V parts (LDR, DHT). The relays and opto LEDs have no power, so nothing switches. That is expected.
5. Flash: `python tools/flash.py` (or the VS Code task *Flash to board (ST-Link)*).
6. Unplug the ST-Link. Power-up is described below.

If you only have USB and no ST-Link, the Blue Pill USB is for power, not for programming this firmware (see `docs/programming.md`).

## B. Blue Pill removed from the board
Only possible if you fitted sockets (female headers). The kit footprint is a **solder-in** part, so if you soldered the Blue Pill in, use option A. With sockets:
1. Mains off and unplugged; wait one minute.
2. Pull the Blue Pill out and flash it on the bench with the ST-Link (or USB-serial and the BOOT0 bootloader, see `docs/programming.md`).
3. Push it back in the right way round: USB end towards the marked USB end on the PCB. Check the pin-1 mark before power.

## First power-up (no load)
1. Visual check: no solder bridges, HLK pins not shorted, nothing loose near the mains terminals.
2. **Bench 5 V test first, without mains:** feed 5 V from a bench supply or a phone charger into the +5V and GND pads. With a multimeter check +3.3 V at the Blue Pill 3.3 pin. Current should be small (tens of mA, relays off).
3. Check PA1 and PA2 sit at **3.3 V** (relay-OFF level) after boot. Pull PA1 to GND with a wire: relay 1 and its red LED must switch on. Same for PA2. Release: they switch off.
4. Remove the bench supply. Only then connect mains, **with no lamp and no fan on the relay terminals**, inside an enclosure, and with a fuse in the mains line.
5. Check +5V at the HLK output is 5 V (about 4.9 to 5.3 V is normal) before connecting anything else.
6. Test the PIR (60 s warm-up), the buttons and the LEDs with the relays unloaded: listen for relay clicks.
7. Only then connect the lamp and the fan, with the mains wiring done or checked by a qualified electrician.

## Advice
- **Fuse:** put a fuse (for example 1 A slow-blow, rated for 230 V) in the live wire before the HLK-5M05, even if the module has its own protection. I did not verify what protection the HLK-5M05 has inside; check its datasheet.
- **Backfeed protection (optional, small change):** put a Schottky diode (1N5819, anode to the HLK +5V, cathode to the Blue Pill 5V pin) and keep the relay and opto 5 V on the HLK net directly. Then a USB cable can no longer push 5 V into the board net. The Blue Pill then sees about 4.7 V, which its regulator accepts. Not tested.
- **Isolation:** the HLK is an isolated supply, so the low-voltage side does not touch mains. Keep mains copper and the relay contact area away from the Blue Pill and low-voltage traces (a slot in the board between them is good practice), and keep wire ends tight in the terminal block.
- **Indicator LEDs:** the LED in series with each opto input must be **red** (low forward voltage). Blue or white LEDs drop too much voltage and the relay may not pull in.
- **Test with a bulb limiter** (a normal lamp in series with the live wire) for the very first mains power-up if you can.
- **Mark** the board "USB: unplug when mains is connected".
