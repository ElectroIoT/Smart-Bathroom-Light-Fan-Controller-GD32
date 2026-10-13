#!/usr/bin/env python3
"""Live test dashboard for the running board (reads it through the ST-Link).

  python tools/dashboard.py            then open  http://localhost:8765
  python tools/dashboard.py --port 9000 --no-browser

Needs: OpenOCD (see tools/flash.py), the ST-Link plugged in, and build/firmware.elf
built from the SAME source that is flashed (it gives the variable addresses).
The chip keeps running while it is read. Stop with Ctrl+C. Close this before
flashing: only one program can use the ST-Link at a time.

Shows: raw PIR pin, debounced motion, light and fan relays with countdowns, fan start
delay, override, fan enabled, LDR value with thresholds, buttons, faults, an event log
and a test checklist that ticks as you exercise each input.
"""
import json
import os
import re
import socket
import subprocess
import sys
import threading
import time
import webbrowser
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import flash  # noqa: E402
import live_status as ls  # noqa: E402
from build import ROOT  # noqa: E402

RPC_PORT = 6666
GPIOA_ISTAT, GPIOB_ISTAT, ADC_RDATA = 0x40010808, 0x40010C08, 0x4001244C

FIELDS = ls.FIELDS + [
    "start_ms", "last_motion_ms", "light_on_since_ms", "override_start_ms", "humid_since_ms",
    "humid_expired", "fan_by_humid",
    "cfg.light_timeout_ms", "cfg.fan_timeout_ms", "cfg.warmup_ms", "cfg.fan_start_delay_ms",
    "cfg.override_ms", "cfg.warn_before_ms", "cfg.humid_max_run_ms",
]

state = {"ok": False, "msg": "starting...", "sample": None, "events": []}
lock = threading.Lock()


def config_number(name, default):
    try:
        text = open(os.path.join(ROOT, "include", "config.h"), encoding="utf-8").read()
        m = re.search(r"#define\s+" + name + r"\s+(\d+)", text)
        return int(m.group(1)) if m else default
    except OSError:
        return default


# ---------------------------------------------------------------- OpenOCD RPC
class Rpc:
    def __init__(self):
        self.sock = None

    def connect(self, tries=40):
        for _ in range(tries):
            try:
                self.sock = socket.create_connection(("127.0.0.1", RPC_PORT), timeout=3)
                return True
            except OSError:
                time.sleep(0.25)
        return False

    def call(self, cmd):
        self.sock.sendall(cmd.encode() + b"\x1a")
        buf = b""
        while not buf.endswith(b"\x1a"):
            chunk = self.sock.recv(65536)
            if not chunk:
                raise ConnectionError("OpenOCD closed the connection")
            buf += chunk
        return buf[:-1].decode(errors="replace")


def parse_tcl(s):
    """Parse a Tcl list with nested {braces} into Python lists."""
    pos = 0

    def parse_list():
        nonlocal pos
        out = []
        while pos < len(s):
            c = s[pos]
            if c == "{":
                pos += 1
                out.append(parse_list())
            elif c == "}":
                pos += 1
                return out
            elif c.isspace():
                pos += 1
            else:
                end = pos
                while end < len(s) and not s[end].isspace() and s[end] not in "{}":
                    end += 1
                out.append(s[pos:end])
                pos = end
        return out
    return parse_list()


def u32(blob, off):
    return int.from_bytes(bytes(blob[off:off + 4]), "little")


def start_openocd():
    exe, scripts = flash.find_openocd()
    if not exe:
        sys.exit("OpenOCD not found (see tools/flash.py for install steps).")
    cmd = [exe] + (["-s", scripts] if scripts else []) + [
        "-f", "interface/stlink.cfg", "-c", "transport select swd", "-f", "target/stm32f1x.cfg",
        "-c", "gdb_port disabled", "-c", "telnet_port disabled", "-c", f"tcl_port {RPC_PORT}", "-c", "init"]
    os.makedirs(os.path.join(ROOT, "build"), exist_ok=True)
    log = open(os.path.join(ROOT, "build", "openocd_dashboard.log"), "w")
    return subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)


# ---------------------------------------------------------------- polling thread
def poll():
    sym = ls.symbols(ls.ELF)
    off = ls.offsets(FIELDS)
    size = off["sizeof"]
    ldr_bright_at = config_number("LDR_BRIGHT_ADC", 2000)
    ldr_dark_at = config_number("LDR_DARK_ADC", 1200)
    proc = start_openocd()
    rpc = Rpc()
    if not rpc.connect():
        with lock:
            state.update(ok=False, msg="Could not start OpenOCD. Is the ST-Link plugged in? See build/openocd_dashboard.log")
        return
    script = ("list [read_memory %d 32 1] [read_memory %d 32 1] [read_memory %d 32 1] [read_memory %d 32 1] "
              "[read_memory %d 8 %d] [read_memory %d 8 1] [read_memory %d 8 2]"
              % (GPIOA_ISTAT, GPIOB_ISTAT, ADC_RDATA, sym["g_ms"], sym["g_ctrl"], size,
                 sym["g_ldr_raw"], sym["g_day"]))
    prev = None
    events = []

    def add_event(text, kind="info"):
        events.insert(0, {"t": time.strftime("%H:%M:%S"), "text": text, "kind": kind})
        del events[60:]

    while True:
        try:
            r = parse_tcl(rpc.call(script))
            one = lambda x: x if isinstance(x, str) else x[0]      # a 1-element Tcl list is a bare string
            gpioa, gpiob, adc, ms = one(r[0]), one(r[1]), one(r[2]), one(r[3])
            ctrl, ldr, day = r[4], [one(r[5])], (r[6] if isinstance(r[6], list) else [r[6]])
            ctrl = [int(x, 0) for x in ctrl]
            ms = int(ms, 0)
            g = lambda f: ctrl[off[f]]
            gu = lambda f: u32(ctrl, off[f])
            pir_pin = int(gpioa, 0) & 1
            s = {
                "ms": ms, "uptime_s": ms / 1000.0,
                "pir_pin": pir_pin, "pir_stable": g("pir_stable"),
                "adc": int(adc, 0) & 0xFFF, "ldr_bright": int(ldr[0], 0), "day_bright": int(day[0], 0),
                "light_out": g("light_out"), "fan_out": g("fan_out"),
                "light_on": g("light_on"), "fan_on": g("fan_on"), "fan_armed": g("fan_armed"),
                "light_warn": g("light_warn"), "override": g("override_active"),
                "fan_enabled": g("fan_enabled"), "humid": g("humid"), "fault_stuck": g("fault_stuck"),
                "suppressed": g("light_suppressed"), "daylight_bright_in": g("daylight_bright"),
                "btn_light": (int(gpiob, 0) & 1) == 0, "btn_fan": ((int(gpiob, 0) >> 1) & 1) == 0,
                "ldr_bright_at": ldr_bright_at, "ldr_dark_at": ldr_dark_at,
            }
            el = lambda x: (ms - x) & 0xFFFFFFFF
            warm = max(0, gu("cfg.warmup_ms") - el(gu("start_ms")))
            s["warmup_left_s"] = warm / 1000.0
            hold = bool(s["pir_stable"]) and not s["fault_stuck"]
            s["pir_hold"] = hold
            if s["light_on"]:
                s["light_left_s"] = None if hold else max(0, gu("cfg.light_timeout_ms") - el(gu("last_motion_ms"))) / 1000.0
            else:
                s["light_left_s"] = 0
            humid_hold = bool(s["humid"]) and not g("humid_expired") and s["fan_enabled"]
            if s["fan_on"]:
                s["fan_left_s"] = None if (hold or humid_hold) else max(0, gu("cfg.fan_timeout_ms") - el(gu("last_motion_ms"))) / 1000.0
            else:
                s["fan_left_s"] = 0
            s["fan_start_in_s"] = (max(0, gu("cfg.fan_start_delay_ms") - el(gu("light_on_since_ms"))) / 1000.0
                                   if (s["fan_armed"] and s["light_on"] and not s["fan_on"]) else None)
            s["override_left_s"] = (max(0, gu("cfg.override_ms") - el(gu("override_start_ms"))) / 1000.0
                                    if s["override"] else None)
            s["warn_in_s"] = None
            if s["light_on"] and not hold and gu("cfg.warn_before_ms"):
                s["warn_in_s"] = max(0, gu("cfg.light_timeout_ms") - gu("cfg.warn_before_ms") - el(gu("last_motion_ms"))) / 1000.0

            # events (edges)
            if prev is not None:
                if ms < prev["ms"]:
                    add_event("BOARD RESET (uptime restarted)", "warn")
                for key, up, down, kind in (
                        ("pir_pin", "PIR pin HIGH (motion)", "PIR pin LOW (no motion)", "pir"),
                        ("light_out", "Light relay ON", "Light relay OFF", "light"),
                        ("fan_out", "Fan relay ON", "Fan relay OFF", "fan"),
                        ("override", "Light override ON", "Light override OFF", "light"),
                        ("fan_enabled", "Fan ENABLED (LED on)", "Fan DISABLED (LED off)", "fan"),
                        ("ldr_bright", "LDR reads BRIGHT", "LDR reads DARK", "ldr"),
                        ("btn_light", "Light button pressed", "", "btn"),
                        ("btn_fan", "Fan button pressed", "", "btn"),
                        ("fault_stuck", "FAULT: PIR stuck HIGH", "PIR fault cleared", "warn"),
                        ("light_warn", "Warning blink (light off briefly)", "", "light")):
                    if bool(s[key]) != bool(prev[key]):
                        txt = up if s[key] else down
                        if txt:
                            add_event(txt, kind)
            else:
                add_event("Connected to the board", "info")
            prev = s
            with lock:
                state.update(ok=True, msg="connected", sample=s, events=list(events))
        except Exception as e:  # keep trying: ST-Link unplugged, OpenOCD died
            with lock:
                state.update(ok=False, msg=f"read error: {e}")
            try:
                rpc.sock.close()
            except Exception:
                pass
            time.sleep(1)
            if proc.poll() is not None:
                with lock:
                    state["msg"] = "OpenOCD stopped. Check the ST-Link cable, then restart the dashboard."
                return
            rpc.connect(4)
        time.sleep(0.08)


# ---------------------------------------------------------------- web server
PAGE = r"""<!doctype html>
<html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>Live Test Dashboard</title>
<style>
:root{--bg:#0b1220;--card:#131d33;--line:#243456;--tx:#e8eefc;--mut:#8fa3c8;--on:#2ee59d;--off:#3a4868;--warn:#ffb020;--bad:#ff5470;--pir:#ff6b6b;--ldr:#ffd54a;--fan:#4dd0e1}
@media (prefers-color-scheme: light){:root{--bg:#f2f6fd;--card:#fff;--line:#d3dcf0;--tx:#10264d;--mut:#5b6b8a;--off:#c5cfe4}}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--tx);font-family:Segoe UI,Arial,sans-serif}
header{display:flex;flex-wrap:wrap;gap:12px;align-items:center;justify-content:space-between;padding:14px 20px;border-bottom:1px solid var(--line)}
h1{font-size:20px;margin:0}.pill{padding:4px 12px;border-radius:99px;font-size:13px;font-weight:700}
.ok{background:#12482f;color:#7dffc4}.bad{background:#552033;color:#ff9fb3}
main{padding:16px 20px;display:grid;gap:14px;grid-template-columns:repeat(auto-fit,minmax(310px,1fr));max-width:1400px;margin:0 auto}
.card{background:var(--card);border:1px solid var(--line);border-radius:14px;padding:16px}
.card h2{margin:0 0 10px;font-size:13px;letter-spacing:.08em;text-transform:uppercase;color:var(--mut)}
.row{display:flex;align-items:center;gap:14px}.big{font-size:30px;font-weight:800}.sub{color:var(--mut);font-size:13px}
.led{width:58px;height:58px;border-radius:50%;background:var(--off);flex:none;transition:all .12s}
.led.on{background:var(--on);box-shadow:0 0 22px 6px rgba(46,229,157,.55)}
.led.pir.on{background:var(--pir);box-shadow:0 0 24px 8px rgba(255,107,107,.65)}
.led.fan.on{background:var(--fan);box-shadow:0 0 22px 6px rgba(77,208,225,.55)}
.led.light.on{background:var(--ldr);box-shadow:0 0 24px 8px rgba(255,213,74,.6)}
canvas{width:100%;height:84px;display:block;margin-top:10px;background:rgba(127,150,200,.07);border-radius:8px}
.kv{display:grid;grid-template-columns:auto 1fr;gap:4px 12px;font-size:14px;margin-top:8px}.kv b{text-align:right}
.chip{display:inline-block;margin:2px 4px 2px 0;padding:2px 10px;border-radius:99px;font-size:12px;font-weight:700;background:var(--off);color:var(--tx)}
.chip.on{background:#12482f;color:#7dffc4}.chip.warn{background:#5a4210;color:#ffd27a}.chip.bad{background:#552033;color:#ff9fb3}
.bar{height:16px;border-radius:8px;background:var(--off);position:relative;overflow:hidden;margin-top:10px}
.bar i{position:absolute;top:0;bottom:0;left:0;background:var(--ldr);transition:width .12s}
.bar u{position:absolute;top:-2px;bottom:-2px;width:2px;background:var(--tx)}
#log{max-height:300px;overflow:auto;font-size:13px}#log div{padding:3px 0;border-bottom:1px solid var(--line)}
#log span{color:var(--mut);margin-right:8px;font-variant-numeric:tabular-nums}
.k-pir{color:var(--pir)}.k-light{color:var(--ldr)}.k-fan{color:var(--fan)}.k-ldr{color:#ffb86b}.k-warn{color:var(--bad);font-weight:700}.k-btn{color:#c4a1ff}
.check{display:flex;gap:10px;align-items:center;padding:4px 0;font-size:14px}.check i{width:20px;height:20px;border-radius:6px;background:var(--off);flex:none;text-align:center;line-height:20px;font-style:normal;font-size:13px}
.check.done i{background:var(--on);color:#04281a}
.wide{grid-column:1/-1}
</style></head><body>
<header><h1>Bathroom controller: live test dashboard</h1>
<div><span id="conn" class="pill bad">connecting...</span> <span class="sub" id="up"></span></div></header>
<main>
<section class="card"><h2>PIR motion (PA0)</h2>
<div class="row"><div id="pirled" class="led pir"></div><div><div class="big" id="pirtxt">-</div><div class="sub" id="pirsub"></div></div></div>
<canvas id="pirc" width="600" height="84"></canvas><div class="sub">last 60 s of the raw PIR pin</div></section>

<section class="card"><h2>Light relay (PA1)</h2>
<div class="row"><div id="lightled" class="led light"></div><div><div class="big" id="lighttxt">-</div><div class="sub" id="lightsub"></div></div></div>
<div class="kv"><span>turns off in</span><b id="lightleft">-</b><span>warning blink in</span><b id="warnin">-</b><span>override left</span><b id="ovr">-</b></div>
<div id="lightflags"></div></section>

<section class="card"><h2>Fan relay (PA2)</h2>
<div class="row"><div id="fanled" class="led fan"></div><div><div class="big" id="fantxt">-</div><div class="sub" id="fansub"></div></div></div>
<div class="kv"><span>starts in</span><b id="fanstart">-</b><span>turns off in</span><b id="fanleft">-</b></div>
<div id="fanflags"></div></section>

<section class="card"><h2>LDR daylight (PA7)</h2>
<div class="row"><div><div class="big" id="adc">-</div><div class="sub" id="ldrtxt"></div></div></div>
<div class="bar"><i id="adcbar"></i><u id="thb"></u><u id="thd"></u></div>
<canvas id="adcc" width="600" height="84"></canvas><div class="sub">last 60 s of the ADC value (bright line and dark line = thresholds)</div></section>

<section class="card"><h2>Buttons and state</h2>
<div id="flags"></div>
<div class="kv"><span>uptime</span><b id="upt">-</b><span>PIR warm-up left</span><b id="warm">-</b></div></section>

<section class="card"><h2>Test checklist (ticks as you test)</h2><div id="checks"></div></section>

<section class="card wide"><h2>Event log</h2><div id="log"></div></section>
</main>
<script>
const $=id=>document.getElementById(id);
const hist=[];let seen={};
const checks=[["pir_hi","Wave at the PIR: pin goes HIGH"],["pir_lo","Stay still: PIR pin returns LOW"],["light_on","Light relay turned ON"],
["fan_on","Fan relay turned ON (about 30 s after the light)"],["ldr_br","Shine light on the LDR: reads BRIGHT"],["ldr_dk","Cover the LDR: reads DARK"],
["btn_l","Press the light override button"],["btn_f","Press the fan enable/disable button"],["light_off","Light relay turned OFF again"]];
function fmt(s){if(s===null||s===undefined)return"holding (PIR active)";if(s<=0)return"-";const m=Math.floor(s/60);return m>0?m+"m "+Math.round(s%60)+"s":s.toFixed(1)+"s"}
function chip(t,cls){return'<span class="chip '+(cls||"")+'">'+t+'</span>'}
function drawStrip(cv,pts,key,opt){const c=cv.getContext("2d"),w=cv.width,h=cv.height;c.clearRect(0,0,w,h);
 if(pts.length<2)return;const t1=pts[pts.length-1].t,t0=t1-60000;
 const style=getComputedStyle(document.documentElement);
 if(opt.lines)for(const [v,col] of opt.lines){const y=h-6-(h-12)*v/4095;c.strokeStyle=col;c.setLineDash([4,4]);c.beginPath();c.moveTo(0,y);c.lineTo(w,y);c.stroke();c.setLineDash([])}
 c.strokeStyle=opt.color;c.lineWidth=2;c.beginPath();let first=true;
 for(const p of pts){const x=(p.t-t0)/60000*w;if(x<0)continue;const v=p[key];const y=opt.digital?(v?8:h-8):h-6-(h-12)*v/4095;
  if(first){c.moveTo(x,y);first=false}else{if(opt.digital){c.lineTo(x,c.__ly)}c.lineTo(x,y)}c.__ly=y}
 c.stroke()}
function setChecks(d){const t={};
 if(d.pir_pin)seen.pir_hi=1;if(seen.pir_hi&&!d.pir_pin)seen.pir_lo=1;if(d.light_out){seen.light_on=1}if(seen.light_on&&!d.light_out)seen.light_off=1;
 if(d.fan_out)seen.fan_on=1;if(d.ldr_bright)seen.ldr_br=1;if(seen.ldr_br&&!d.ldr_bright)seen.ldr_dk=1;
 if(d.btn_light)seen.btn_l=1;if(d.btn_fan)seen.btn_f=1;
 $("checks").innerHTML=checks.map(([k,txt])=>'<div class="check '+(seen[k]?"done":"")+'"><i>'+(seen[k]?"&#10003;":"")+'</i>'+txt+'</div>').join("")}
function update(d){
 const s=d.sample;$("conn").textContent=d.ok?"connected":"not connected";$("conn").className="pill "+(d.ok?"ok":"bad");
 if(!d.ok){$("up").textContent=d.msg;}
 if(!s)return;
 $("up").textContent="board uptime "+s.uptime_s.toFixed(1)+" s";
 hist.push({t:Date.now(),pir:s.pir_pin,adc:s.adc});while(hist.length&&hist[0].t<Date.now()-62000)hist.shift();
 $("pirled").classList.toggle("on",!!s.pir_pin);
 $("pirtxt").textContent=s.pir_pin?"MOTION (pin HIGH)":"no motion (pin LOW)";
 $("pirsub").textContent=s.warmup_left_s>0?"warm-up: PIR ignored for "+s.warmup_left_s.toFixed(0)+" s":(s.pir_stable?"controller sees motion":"controller sees no motion");
 $("lightled").classList.toggle("on",!!s.light_out);$("lighttxt").textContent=s.light_out?"ON":"OFF";
 $("lightsub").textContent=s.suppressed?"kept off by daylight":(s.light_warn?"warning blink":(s.override?"manual override":(s.light_on?"timer running":"idle")));
 $("lightleft").textContent=s.light_on?fmt(s.light_left_s):"-";$("warnin").textContent=s.warn_in_s===null?"-":fmt(s.warn_in_s);$("ovr").textContent=s.override?fmt(s.override_left_s):"-";
 $("lightflags").innerHTML=chip("timer "+(s.light_on?"running":"idle"),s.light_on?"on":"")+chip("override",s.override?"warn":"")+chip("daylight blocks",s.suppressed?"warn":"");
 $("fanled").classList.toggle("on",!!s.fan_out);$("fantxt").textContent=s.fan_out?"ON":"OFF";
 $("fansub").textContent=!s.fan_enabled?"DISABLED by button":(s.fan_armed&&s.light_on&&!s.fan_on?"waiting for start delay":(s.fan_on?"timer running":"idle"));
 $("fanstart").textContent=s.fan_start_in_s===null?"-":fmt(s.fan_start_in_s);$("fanleft").textContent=s.fan_on?fmt(s.fan_left_s):"-";
 $("fanflags").innerHTML=chip("enabled",s.fan_enabled?"on":"bad")+chip("humid",s.humid?"warn":"");
 $("adc").textContent=s.adc;$("ldrtxt").textContent=(s.ldr_bright?"BRIGHT":"DARK")+" (raw)  |  filtered: "+(s.day_bright?"bright":"dark")+(s.suppressed?"  | light blocked":"");
 $("adcbar").style.width=(s.adc/4095*100)+"%";$("thb").style.left=(s.ldr_bright_at/4095*100)+"%";$("thd").style.left=(s.ldr_dark_at/4095*100)+"%";
 $("flags").innerHTML=chip("light button",s.btn_light?"warn":"")+chip("fan button",s.btn_fan?"warn":"")+chip("PIR fault",s.fault_stuck?"bad":"")+chip("override",s.override?"warn":"")+chip("fan enabled",s.fan_enabled?"on":"bad");
 $("upt").textContent=s.uptime_s.toFixed(1)+" s";$("warm").textContent=s.warmup_left_s>0?s.warmup_left_s.toFixed(0)+" s":"done";
 drawStrip($("pirc"),hist,"pir",{color:"#ff6b6b",digital:true});
 drawStrip($("adcc"),hist,"adc",{color:"#ffd54a",lines:[[s.ldr_bright_at,"#2ee59d"],[s.ldr_dark_at,"#8fa3c8"]]});
 $("log").innerHTML=d.events.map(e=>'<div><span>'+e.t+'</span><span class="k-'+e.kind+'">'+e.text+'</span></div>').join("");
 setChecks(s)}
async function tick(){try{const r=await fetch("/data",{cache:"no-store"});update(await r.json())}catch(e){$("conn").textContent="dashboard stopped";$("conn").className="pill bad"}setTimeout(tick,150)}
tick();
</script></body></html>"""


class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path.startswith("/data"):
            with lock:
                body = json.dumps(state).encode()
            ctype = "application/json"
        else:
            body, ctype = PAGE.encode(), "text/html; charset=utf-8"
        self.send_response(200)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, *a):
        pass


def main():
    port = 8765
    if "--port" in sys.argv:
        port = int(sys.argv[sys.argv.index("--port") + 1])
    if not os.path.exists(ls.ELF):
        sys.exit("build/firmware.elf not found. Build the firmware that is flashed first (python tools/build.py firmware).")
    threading.Thread(target=poll, daemon=True).start()
    srv = ThreadingHTTPServer(("127.0.0.1", port), Handler)
    url = f"http://localhost:{port}"
    print(f"Dashboard running: {url}   (Ctrl+C to stop; close it before flashing)")
    if "--no-browser" not in sys.argv:
        webbrowser.open(url)
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
