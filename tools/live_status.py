#!/usr/bin/env python3
"""Read the live state of the running board through the ST-Link (no serial adapter).

  python tools/live_status.py            20 samples, 0.5 s apart
  python tools/live_status.py 60 1000    60 samples, 1000 ms apart

Needs build/firmware.elf (the firmware that is flashed) and OpenOCD. The chip keeps
running while it is read. Shows the raw PIR pin, the LDR ADC value, daylight state
and the controller state (light/fan timers, relays, buttons, humidity).
"""
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import flash  # noqa: E402  (find_openocd)
from build import host_cc, ROOT  # noqa: E402

ELF = os.path.join(ROOT, "build", "firmware.elf")
FIELDS = ["light_out", "fan_out", "light_on", "fan_on", "fan_armed", "light_warn", "warned",
          "override_active", "fan_enabled", "humid", "fault_stuck", "pir_stable",
          "daylight_bright", "light_suppressed"]


ALL_FIELDS = FIELDS + [
    "start_ms", "last_motion_ms", "light_on_since_ms", "override_start_ms", "humid_since_ms",
    "humid_expired", "fan_by_humid",
    "cfg.light_timeout_ms", "cfg.fan_timeout_ms", "cfg.warmup_ms", "cfg.fan_start_delay_ms",
    "cfg.override_ms", "cfg.warn_before_ms", "cfg.humid_max_run_ms",
]
CACHE = os.path.join(ROOT, "tools", "offsets.json")


def _header_hash():
    text = open(os.path.join(ROOT, "src", "controller.h"), encoding="utf-8").read()
    return hashlib.sha1(text.replace("\r\n", "\n").encode()).hexdigest()


def symbols(path):
    d = open(path, "rb").read()
    shoff = struct.unpack_from("<I", d, 0x20)[0]
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
    secs = [struct.unpack_from("<IIIIIIIIII", d, shoff + i * shentsize) for i in range(shnum)]
    out = {}
    for s in secs:
        if s[1] == 2:                                   # SHT_SYMTAB
            strtab = secs[s[6]]
            for off in range(s[4], s[4] + s[5], 16):
                name, value = struct.unpack_from("<II", d, off)
                nm = d[strtab[4] + name:d.index(b"\0", strtab[4] + name)].decode()
                out[nm] = value
    return out


def offsets(fields=None):
    """Struct offsets: from tools/offsets.json when it matches controller.h, else compiled (needs a C compiler)."""
    fields = fields or ALL_FIELDS
    try:
        data = json.load(open(CACHE))
        if data.get("hash") == _header_hash() and all(f in data["offsets"] for f in fields + ["sizeof"]):
            return data["offsets"]
    except (OSError, ValueError, KeyError):
        pass
    res = _compile_offsets(ALL_FIELDS)
    try:
        json.dump({"hash": _header_hash(), "offsets": res}, open(CACHE, "w"), indent=1)
    except OSError:
        pass
    return res


def _compile_offsets(fields):
    """Struct offsets from a tiny C program, so they always match controller.h."""
    code = ('#include <stdio.h>\n#include <stddef.h>\n#include "controller.h"\n#include "daylight.h"\n'
            "int main(void){\n")
    for f in fields:
        code += f'printf("{f} %d\\n", (int)offsetof(ctrl_t, {f}));\n'
    code += 'printf("sizeof %d\\n", (int)sizeof(ctrl_t));\nreturn 0;}\n'
    tmp = tempfile.mkdtemp()
    c = os.path.join(tmp, "off.c")
    exe = os.path.join(tmp, "off.exe" if os.name == "nt" else "off")
    open(c, "w").write(code)
    subprocess.run(host_cc() + ["-Isrc", "-Iinclude", c, "-o", exe], cwd=ROOT, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    res = subprocess.run([exe], capture_output=True, text=True).stdout
    return {k: int(v) for k, v in (line.split() for line in res.splitlines())}


def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 20
    gap = int(sys.argv[2]) if len(sys.argv) > 2 else 500
    exe, scripts = flash.find_openocd()
    if not exe or not os.path.exists(ELF):
        print("Need OpenOCD and build/firmware.elf (run tools/build.py firmware first).")
        return 2
    sym = symbols(ELF)
    off = offsets()
    ctrl, size = sym["g_ctrl"], off["sizeof"]
    body = "; ".join([
        'echo "PIN [format %08x [read_memory 0x40010808 32 1]]"',
        'echo "ADC [format %08x [read_memory 0x4001244C 32 1]]"',
        f'echo "MS [format %08x [read_memory {sym["g_ms"]} 32 1]]"',
        f'echo "CTRL [read_memory {ctrl} 8 {size}]"',
        f'echo "LDR [read_memory {sym["g_ldr_raw"]} 8 1]"',
        f'echo "DAY [read_memory {sym["g_day"]} 8 2]"',
        'echo "END"',
        f"sleep {gap}",
    ])
    cmds = ["init", f"for {{set i 0}} {{$i < {n}}} {{incr i}} {{ {body} }}", "exit"]
    cmd = [exe] + (["-s", scripts] if scripts else []) + [
        "-f", "interface/stlink.cfg", "-c", "transport select swd", "-f", "target/stm32f1x.cfg"]
    for c in cmds:
        cmd += ["-c", c]
    out = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True).stdout
    if "Error" in out and "PIN" not in out:
        print(out[-600:])
        print("Could not read the board. Is the ST-Link connected?")
        return 1

    rec, rows = {}, []
    for line in out.splitlines():
        m = re.match(r"(PIN|ADC|MS|CTRL|LDR|DAY|END)\s*(.*)", line.strip())
        if not m:
            continue
        k, v = m.groups()
        if k == "END":
            rows.append(rec)
            rec = {}
        else:
            rec[k] = v.split()
    if not rows:
        print(out[-1500:])
    print(" t(s)  PIR_pin  ADC   LDR     light fan  PIRstable  override fan_en  humid  suppressed")
    for r in rows:
        try:
            pin = int(r["PIN"][0], 16) & 1
            adc = int(r["ADC"][0], 16) & 0xFFF
            ms = int(r["MS"][0], 16)
            c = [int(x, 0) for x in r["CTRL"]]
            ldr = int(r["LDR"][0], 0)
            day = [int(x, 0) for x in r["DAY"]]
            g = lambda f: c[off[f]]
            print(f"{ms/1000:6.1f}  {'HIGH' if pin else 'low ':4}   {adc:4d}  "
                  f"{'bright' if ldr else 'dark  '}  light={g('light_out')} fan={g('fan_out')}  "
                  f"pir={g('pir_stable')}      ovr={g('override_active')}  fan_en={g('fan_enabled')}  "
                  f"hum={g('humid')}  suppr={g('light_suppressed')} (filtered bright={day[0]})")
        except (KeyError, IndexError, ValueError) as e:
            print("parse error:", repr(e), r)
    return 0


if __name__ == "__main__":
    sys.exit(main())
