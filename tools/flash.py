#!/usr/bin/env python3
"""Flash build/firmware.elf to the board through an ST-Link V2 (SWD).

  python tools/flash.py          flash, verify, reset
  python tools/flash.py --probe  only connect and print chip info (no writing)

Finds OpenOCD on PATH, or in the winget xPack install (Windows). Install it with:
  Windows:  winget install xpack-dev-tools.openocd-xpack
  Linux:    sudo apt install openocd
  macOS:    brew install openocd
"""
import glob
import os
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ELF = os.path.join(ROOT, "build", "firmware.elf")


def find_openocd():
    exe = shutil.which("openocd")
    if exe:
        return exe, None
    local = os.environ.get("LOCALAPPDATA", "")
    pattern = os.path.join(local, "Microsoft", "WinGet", "Packages",
                           "xpack-dev-tools.openocd-xpack*", "xpack-openocd-*")
    for base in sorted(glob.glob(pattern), reverse=True):
        exe = os.path.join(base, "bin", "openocd.exe")
        scripts = os.path.join(base, "openocd", "scripts")
        if os.path.exists(exe):
            return exe, scripts
    return None, None


def main():
    probe = "--probe" in sys.argv
    exe, scripts = find_openocd()
    if not exe:
        print("OpenOCD not found. Install it (see the top of this file), then retry.")
        return 2
    if not probe and not os.path.exists(ELF):
        print("build/firmware.elf not found. Run 'python tools/build.py firmware' first.")
        return 2

    cmd = [exe]
    if scripts:
        cmd += ["-s", scripts]
    cmd += ["-f", "interface/stlink.cfg", "-c", "transport select swd",
            "-f", "target/stm32f1x.cfg"]
    if probe:
        cmd += ["-c", "init; halt; flash probe 0; flash info 0; resume; exit"]
    else:
        # braces keep a path with spaces together as one OpenOCD argument
        cmd += ["-c", "program {" + ELF.replace("\\", "/") + "} verify reset exit"]
    print("Running:", " ".join(cmd))
    rc = subprocess.run(cmd).returncode
    if rc == 0 and not probe:
        print("\nFlashed and verified. The board has been reset and is running.")
    elif rc != 0:
        print("\nFlashing failed. Check the ST-Link wiring (SWDIO, SWCLK, GND) and see "
              "docs/programming.md, 'Recovery if programming fails'.")
    return rc


if __name__ == "__main__":
    sys.exit(main())
