#!/usr/bin/env python3
"""Fake-fault test: makes the red FAULT LED (PA6) blink, without waiting 30 minutes.

  python tools/fault_test.py          fake the "PIR stuck HIGH" fault for 8 seconds
  python tools/fault_test.py 15       ... for 15 seconds

It sets the controller's fault_stuck flag through the ST-Link, exactly the state the
firmware enters when the PIR output stays HIGH for PIR_STUCK_HIGH_MS (30 minutes).
The FAULT LED should blink fast (about 4 times a second) while the fake fault is on,
then go dark again. Nothing is written to flash and the relays are not touched.
(A real PIR going LOW would also clear the fault.) Needs build/firmware.elf.
"""
import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dashboard as dash  # noqa: E402
import live_status as ls  # noqa: E402


def main():
    secs = float(sys.argv[1]) if len(sys.argv) > 1 else 8.0
    sym = ls.symbols(ls.ELF)
    addr = sym["g_ctrl"] + ls.offsets()["fault_stuck"]
    rpc = dash.Rpc()
    proc = None
    if not rpc.connect(2):                      # dashboard not running: start our own OpenOCD
        proc = dash.start_openocd()
        if not rpc.connect():
            print("Could not talk to the board. Is the ST-Link plugged in?")
            return 1
    try:
        print(f"FAKE FAULT ON for {secs:.0f} s: watch the red FAULT LED (PA6) blink fast ...", flush=True)
        rpc.call(f"mwb {addr} 1")
        end = time.time() + secs
        while time.time() < end:                # keep it set (a real falling PIR edge would clear it)
            rpc.call(f"mwb {addr} 1")
            time.sleep(0.2)
    finally:
        try:
            rpc.call(f"mwb {addr} 0")
            print("Fake fault cleared: the FAULT LED should be dark again.")
        except Exception:
            pass
        try:
            rpc.sock.close()
        except Exception:
            pass
        if proc:
            proc.terminate()
    return 0


if __name__ == "__main__":
    sys.exit(main())
