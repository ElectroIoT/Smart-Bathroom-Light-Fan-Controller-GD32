#!/usr/bin/env python3
"""LED walk test: blinks each external status LED in turn so you can check the wiring.

  python tools/led_test.py

Blinks (3 times each): PIR LED (PA3), light relay LED (PA4), fan relay LED (PA5),
FAULT LED (PA6), FAN-ENABLED LED (PB11), external SYSTEM heartbeat LED (PB13). The on-board system LED (PC13) already
blinks by itself. Each LED is put back to the state the controller wants afterwards.
Uses the running chip through the ST-Link (starts OpenOCD, or reuses the dashboard's).
Relays are not touched.
"""
import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dashboard as dash  # noqa: E402  (OpenOCD helpers)

LEDS = [("PIR LED", "PA3", dash.GPIOA_OCTL, 3), ("Light relay LED", "PA4", dash.GPIOA_OCTL, 4),
        ("Fan relay LED", "PA5", dash.GPIOA_OCTL, 5), ("FAULT LED", "PA6", dash.GPIOA_OCTL, 6),
        ("FAN-ENABLED LED", "PB11", dash.GPIOB_OCTL, 11), ("SYSTEM heartbeat LED (external)", "PB13", dash.GPIOB_OCTL, 13)]


def main():
    rpc = dash.Rpc()
    proc = None
    if not rpc.connect(2):                      # dashboard not running: start our own OpenOCD
        proc = dash.start_openocd()
        if not rpc.connect():
            print("Could not talk to the board. Is the ST-Link plugged in?")
            return 1
    one = lambda s: int((s.split() or ["0"])[0].strip("{}"), 0)
    try:
        for name, pin, octl, n in LEDS:
            bop = octl + 4                      # BOP register follows OCTL
            orig = (one(rpc.call(f"read_memory {octl} 32 1")) >> n) & 1
            print(f"Watch the {name} on {pin}: blinking 3 times ...", flush=True)
            for i in range(6):
                level = orig ^ (1 if i % 2 == 0 else 0)
                rpc.call(f"mww {bop} {(1 << n) if level else (1 << (n + 16))}")
                time.sleep(0.35)
            rpc.call(f"mww {bop} {(1 << n) if orig else (1 << (n + 16))}")   # restore
            time.sleep(0.4)
        print("Done. Every LED that blinked is wired correctly. Tell me which did not.")
    finally:
        try:
            rpc.sock.close()
        except Exception:
            pass
        if proc:
            proc.terminate()
    return 0


if __name__ == "__main__":
    sys.exit(main())
