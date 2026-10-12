# Test plan

Automated tests prove the *logic*. They do not prove the hardware. Do the bench tests in order and **never skip to mains**.

## 0. Automated logic tests (PC)
```
python tools/build.py test
```
Runs 64 checks × 4 configurations (relay active-LOW/HIGH × PIR active-HIGH/LOW). Covers: startup OFF, warm-up, motion → both ON, debounce, 7/10 min timeouts at exact boundary, independence of light and fan, spec example (0:00/4:00/11:00/14:00), motion during either countdown, PIR held HIGH, stuck-HIGH fault, reset while energised, 32-bit tick rollover, polarity.
Last run: all passed (this session).

## 1. Safe first power-up (no PIR, no relay, no mains)
1. Flash firmware. Power only the MCU board.
2. Measure PA1 and PA2 with a multimeter: both must sit at the **relay-OFF level** (3.3 V for active-LOW, 0 V for active-HIGH).
3. Optional: view debug text on PA9 — expect `BOOT: relays OFF, PIR warm-up`.

## 2. PIR signal test (no relay)
1. Power PIR from its supply, measure OUT with the meter, MCU disconnected: note idle and active voltage (see `hardware_design.md` §4). Wait out its warm-up.
2. If OUT ≤ 3.3 V, connect to PA0. Else add a divider first.
3. After the firmware's 60 s warm-up, wave a hand: PA1/PA2 should change to the ON level and stay until timeout. Debug log shows `LIGHT ON` / `FAN ON`.
4. Set `PIR_ACTIVE_HIGH` to match if behaviour is inverted.

## 2b. Status LED test
With the LEDs wired: PIR LED lights when motion is seen; light/fan LEDs follow the relays; fault LED stays off (it blinks fast only if the PIR stays HIGH for 30 min). System LED (PC13) blinks fast for 60 s, then a blip every 2 s.

## 3. Relay logic test (module powered, **no mains, no load**)
1. Wire relay module VCC/GND/IN1/IN2 only; contacts unconnected.
2. Confirm at power-up both relay LEDs are OFF and no click is heard. **If they are ON, flip `RELAY_ACTIVE_LOW`.**
3. Trigger motion: both relays click ON. Confirm with a continuity meter on the contacts.
4. Confirm it switches OFF and ON reliably from 3.3 V GPIO (several cycles). If unreliable: needs a level shifter / transistor stage.
5. Reset the board (button) while energised: relays must drop OFF immediately and stay off through warm-up.

## 4. Timer verification (no mains)
For a quick check, temporarily set `-DLIGHT_TIMEOUT_MS=10000 -DFAN_TIMEOUT_MS=20000 -DPIR_WARMUP_MS=5000` — **do not leave these in the final build**. Check: light off at ~10 s, fan at ~20 s, fan still on after light off, new motion restarts both. Then rebuild with the defaults and confirm 7 min / 10 min once with a stopwatch.

## 5. Reset and power-failure behaviour
* Unplug power while both relays are ON: relays drop (coils de-energise). Re-apply power: relays stay OFF until motion after the warm-up.
* Hold NRST: relays stay OFF (needs the external pull resistors from §5 of the design doc — verify by measuring the relay inputs with the MCU in reset).

## 6. Final installation checklist
- [ ] PIR output voltage measured and compatible with PA0
- [ ] Relay polarity confirmed on the bench
- [ ] Relay contact ratings checked against the real lamp and fan (including inrush/inductive)
- [ ] Pull resistors fitted so relays are OFF during reset
- [ ] Common GND, regulated supply, wiring secured
- [ ] Mains parts in insulated enclosure, strain relief, correct clearances, fuse/MCB
- [ ] **Qualified electrician installed or inspected the mains wiring**
- [ ] First live test with the enclosure closed; a 7-minute and 10-minute run checked
- [ ] PIR hold-time pot set to minimum (MCU sets the delay)
- [ ] Understood: PIR cannot detect a perfectly still person
