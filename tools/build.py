#!/usr/bin/env python3
"""Build helper (works with no make/cmake).

  python tools/build.py test       run host unit tests (4 config variants)
  python tools/build.py firmware   build build/firmware.elf/.bin for the GD32

Compiler search order: $CC / arm-none-eabi-gcc (firmware) / gcc / cc, and
as a fallback `python -m ziglang cc` (pip install ziglang) for both.
"""
import os, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD = os.path.join(ROOT, "build")
CORE = ["src/controller.c", "src/button.c", "src/humidity.c", "src/daylight.c", "src/app.c"]
WARN = ["-std=c99", "-Wall", "-Wextra", "-Werror", "-Os", "-Iinclude", "-Isrc"]


def host_cc():
    for c in (os.environ.get("CC"), "gcc", "cc", "clang"):
        if c and shutil.which(c):
            return [c]
    return [sys.executable, "-m", "ziglang", "cc"]


def run(cmd):
    print(" ".join(cmd))
    subprocess.run(cmd, cwd=ROOT, check=True)


def test():
    os.makedirs(BUILD, exist_ok=True)
    # Core timer tests run with the optional features switched off so the
    # original timing is checked exactly; features have their own test file.
    off = ["-DLIGHT_WARN_BEFORE_MS=0", "-DFAN_START_DELAY_MS=0"]
    variants = {
        "default (relay active-LOW, PIR active-HIGH)": [],
        "relay active-HIGH": ["-DRELAY_ACTIVE_LOW=0"],
        "PIR active-LOW": ["-DPIR_ACTIVE_HIGH=0"],
        "relay active-HIGH + PIR active-LOW": ["-DRELAY_ACTIVE_LOW=0", "-DPIR_ACTIVE_HIGH=0"],
    }
    ok = True
    exe = os.path.join(BUILD, "tests.exe" if os.name == "nt" else "tests")
    for name, defs in variants.items():
        run(host_cc() + WARN + off + defs + CORE +
            ["tests/fake_hal.c", "tests/test_main.c", "-o", exe])
        print(f"=== core timers: {name} ===")
        ok &= subprocess.run([exe], cwd=ROOT).returncode == 0
    for name, defs in {"relay active-LOW": [], "relay active-HIGH": ["-DRELAY_ACTIVE_LOW=0"]}.items():
        run(host_cc() + WARN + defs + CORE +
            ["tests/fake_hal.c", "tests/test_features.c", "-o", exe])
        print(f"=== features (default config): {name} ===")
        ok &= subprocess.run([exe], cwd=ROOT).returncode == 0
    for sensor in ("11", "22"):
        run(host_cc() + WARN + ["-DHUMIDITY_SENSOR=" + sensor] + CORE +
            ["tests/fake_hal.c", "tests/test_humidity.c", "-o", exe])
        print(f"=== humidity option: DHT{sensor} ===")
        ok &= subprocess.run([exe], cwd=ROOT).returncode == 0
    run(host_cc() + WARN + ["-DDAYLIGHT_SENSOR=1"] + CORE +
        ["tests/fake_hal.c", "tests/test_daylight.c", "-o", exe])
    print("=== daylight option ===")
    ok &= subprocess.run([exe], cwd=ROOT).returncode == 0
    print("ALL TESTS PASSED" if ok else "TESTS FAILED")
    return 0 if ok else 1


def firmware():
    os.makedirs(BUILD, exist_ok=True)
    elf = os.path.join(BUILD, "firmware.elf")
    srcs = CORE + ["src/main.c", "src/hal_gd32f103.c", "src/startup.c"]
    if shutil.which("arm-none-eabi-gcc"):
        cc = ["arm-none-eabi-gcc", "-mcpu=cortex-m3", "-mthumb"]
        run(cc + WARN + ["-ffunction-sections", "-fdata-sections", "-nostdlib",
                         "-Wl,--gc-sections", "-Tlinker/gd32f103c8.ld"] + srcs + ["-o", elf])
        run(["arm-none-eabi-objcopy", "-O", "binary", elf, os.path.join(BUILD, "firmware.bin")])
        run(["arm-none-eabi-size", elf])
    else:
        cc = [sys.executable, "-m", "ziglang", "cc", "-target", "thumb-freestanding-eabi",
              "-mcpu=cortex_m3"]
        run(cc + WARN + ["-ffunction-sections", "-fdata-sections", "-nostdlib",
                         "-Wl,--gc-sections", "-Tlinker/gd32f103c8.ld"] + srcs + ["-o", elf])
        run([sys.executable, "-m", "ziglang", "objcopy", "-O", "binary", elf,
             os.path.join(BUILD, "firmware.bin")])
    print("size:", os.path.getsize(os.path.join(BUILD, "firmware.bin")), "bytes ->", BUILD)
    return 0


if __name__ == "__main__":
    cmd = sys.argv[1] if len(sys.argv) > 1 else ""
    sys.exit({"test": test, "firmware": firmware}.get(cmd, lambda: print(__doc__) or 2)())
