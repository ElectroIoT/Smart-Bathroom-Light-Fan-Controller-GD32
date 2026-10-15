"""Generates (1) the detailed Blue Pill EAGLE library and (2) a full schematic .sch
for the AutoBath-Light-Fan controller, from one description. Output into docs/pcb/eda/."""
import math, os
ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..") if "__file__" in globals() else "."
OUTDIR = os.path.normpath(os.path.join(ROOT, "docs", "pcb", "eda"))
os.makedirs(OUTDIR, exist_ok=True)
ROW, PITCH, LEN, W = 15.24, 2.54, 53.34, 22.5   # row spacing and size measured from a photo (see docs/pcb/3d/README.md)

def esc(s): return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")

# ---------------- Blue Pill detailed pins (name shown on symbol) ----------------
# (header pin label, function/role in this project)
LEFT = [("PB12","DHT_DATA"),("PB13","LED_HEARTBEAT"),("PB14",""),("PB15",""),("PA8",""),("PA9","UART_TX"),("PA10",""),
        ("PA11","USB_DM"),("PA12","USB_DP"),("PA15",""),("PB3",""),("PB4",""),("PB5",""),("PB6",""),("PB7",""),("PB8",""),("PB9",""),
        ("5V","5V_IN"),("GND","GND"),("3V3","3V3_OUT")]
RIGHT = [("GND","GND"),("GND","GND"),("3V3","3V3"),("NRST","RESET"),("PB11","LED_FAN_ENABLED"),("PB10","LDR_DIGITAL_IN"),
         ("PB1","BTN_FAN"),("PB0","BTN_LIGHT"),("PA7","LDR_ADC"),("PA6","LED_FAULT"),("PA5","LED_FAN"),("PA4","LED_LIGHT"),
         ("PA3","LED_PIR"),("PA2","RELAY2_FAN"),("PA1","RELAY1_LIGHT"),("PA0","PIR_OUT"),("PC15",""),("PC14",""),("PC13","LED_ONBOARD"),("VBAT","VBAT")]

def short(lab):
    return {"3V3":"3.3","NRST":"RES","VBAT":"VBT"}.get(lab, lab[1:] if lab[0] == "P" else lab)

# symbol numbering as in the usual Blue Pill symbol: right side 1..20 bottom-to-top = LEFT header list,
# left side 21..40 bottom-to-top = RIGHT header list.
BP_PINS = {}   # key (L1..L20 / R1..R20) -> pin name
BP_PADNUM = {} # key -> pad number "1".."40"
_seen = {}
for key, lab in [("L%d" % (i+1), LEFT[i][0]) for i in range(20)] + [("R%d" % (i+1), RIGHT[i][0]) for i in range(20)]:
    n = short(lab)
    _seen[n] = _seen.get(n, 0) + 1
_cnt = {}
for i in range(40):
    key = ("L%d" % (i+1)) if i < 20 else ("R%d" % (i-19))
    lab = (LEFT[i][0] if i < 20 else RIGHT[i-20][0])
    n = short(lab)
    if _seen[n] > 1:
        _cnt[n] = _cnt.get(n, 0) + 1
        n = n + "​" * (_cnt[n] - 1)   # invisible, keeps pin names unique while showing plain GND / 3.3
    BP_PINS[key] = n
    BP_PADNUM[key] = str(i+1)

LAYERS = [(1,"Top",4),(16,"Bottom",1),(17,"Pads",2),(18,"Vias",2),(20,"Dimension",15),(21,"tPlace",7),(22,"bPlace",7),
          (25,"tNames",7),(26,"bNames",7),(27,"tValues",7),(28,"bValues",7),(51,"tDocu",7),(52,"bDocu",7),
          (91,"Nets",2),(92,"Busses",1),(93,"Pins",2),(94,"Symbols",4),(95,"Names",7),(96,"Values",7),(97,"Info",7),(98,"Guide",6)]
def layers_xml():
    return "<layers>" + "".join('<layer number="%d" name="%s" color="%d" fill="1" visible="yes" active="yes"/>' % l for l in LAYERS) + "</layers>"

def wire(x1,y1,x2,y2,layer,w=0.2):
    return '<wire x1="%.3f" y1="%.3f" x2="%.3f" y2="%.3f" width="%s" layer="%d"/>' % (x1,y1,x2,y2,w,layer)
def rect_wires(x1,y1,x2,y2,layer,w):
    return [wire(x1,y1,x2,y1,layer,w),wire(x2,y1,x2,y2,layer,w),wire(x2,y2,x1,y2,layer,w),wire(x1,y2,x1,y1,layer,w)]

# ---------------- packages ----------------
def pkg_bluepill():
    o = ['<package name="BLUEPILL_40P"><description>Blue Pill module 53.34 x 22.5 mm, rows 15.24 mm apart (measured from a photo, check yours)</description>']
    x0 = (19*PITCH)/2
    for i in range(20):
        x = x0 - i*PITCH      # pin 1 / pin 21 at +X, exactly where the 3D model has B12 and GND (USB end of the model = +X)
        for side, y, base in (("L", ROW/2, 0), ("R", -ROW/2, 20)):
            o.append('<pad name="%d" x="%.3f" y="%.3f" drill="1.0" diameter="1.8" shape="%s"/>' % (base+i+1, x, y, "square" if i == 0 and side == "L" else "round"))
    o += rect_wires(-LEN/2,-W/2,LEN/2,W/2,21,0.2)
    o.append('<text x="0" y="0" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-2" size="1.27" layer="27" align="center">&gt;VALUE</text></package>')
    return "\n".join(o), "BLUEPILL_40P"

def pkg_pads(name, pads, bw, bh, desc, drill=1.0, dia=1.8):
    """pads: list of (padname, x, y)"""
    o = ['<package name="%s"><description>%s</description>' % (name, esc(desc))]
    for j, (pn, x, y) in enumerate(pads):
        o.append('<pad name="%s" x="%.3f" y="%.3f" drill="%.2f" diameter="%.2f" shape="%s"/>' % (pn, x, y, drill, dia, "square" if j == 0 else "round"))
    o += rect_wires(-bw/2,-bh/2,bw/2,bh/2,21,0.2)
    o.append('<text x="0" y="%.2f" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="%.2f" size="1.27" layer="27" align="center">&gt;VALUE</text></package>' % (bh/2+1.5, -bh/2-1.5))
    return "\n".join(o)

def row_pads(n, pitch, start=1):
    x0 = -(n-1)*pitch/2
    return [(str(start+i), x0+i*pitch, 0) for i in range(n)]

PACKAGES = {}   # name -> xml
def addpkg(name, pads, bw, bh, desc, **kw):
    PACKAGES[name] = pkg_pads(name, pads, bw, bh, desc, **kw)
    return name
addpkg("HDR_1X2", row_pads(2,2.54), 5.08, 2.6, "1x2 pin header 2.54 mm")
addpkg("HDR_1X3", row_pads(3,2.54), 7.62, 2.6, "1x3 pin header 2.54 mm")
addpkg("TERM_3P_5.08", row_pads(3,5.08), 15.5, 10, "3-pole screw terminal 5.08 mm", drill=1.3, dia=2.6)
addpkg("AXIAL_10.16", row_pads(2,10.16), 8.5, 2.6, "axial resistor 0.4in (0207)")
addpkg("DO41_10.16", row_pads(2,10.16), 8.5, 3.5, "DO-41 diode, pad 1 is the cathode (banded end)")
addpkg("RADIAL_2.5", row_pads(2,2.5), 6.5, 6.5, "radial electrolytic 2.5 mm lead pitch (6.3 mm can)")
addpkg("DISC_5.0", row_pads(2,5.0), 6, 3, "radial ceramic 5 mm pitch")
addpkg("VARISTOR_7.5", row_pads(2,7.5), 9, 4, "varistor disc, 7.5 mm pitch")
addpkg("FUSE_5X20_22.5", row_pads(2,22.5), 26, 8, "5x20 mm fuse holder, 22.5 mm pitch (check yours)", drill=1.3, dia=2.6)
addpkg("TO92_2.54", [("1",-2.54,0),("2",0,0),("3",2.54,0)], 5, 4.5, "TO-92 (pin order E,B,C for S8050 flat face to you; check the datasheet)")
addpkg("RELAY_SRD_5PIN", [("1",0,0),("2",0,-7.5),("3",14.2,-7.5),("4",14.2,0),("5",-7.5,0)], 19, 16, "SRD-05VDC relay style, PLACEHOLDER pad positions: verify against the maker drawing")
addpkg("ACDC_4PIN", [("1",-15,5),("2",-15,-5),("3",15,-5),("4",15,5)], 35, 20, "AC-DC module style (HLK-PM01), PLACEHOLDER pad positions: verify against the datasheet", drill=1.2, dia=2.4)

# ---------------- symbols: generic box ----------------
SYMBOLS = {}  # name -> (xml, {pinname: (x, y, side)})
def box_symbol(name, left, right, title=">NAME"):
    maxl = max([len(p) for p in left] + [3]); maxr = max([len(p) for p in right] + [3])
    w = max(12.7, math.ceil((maxl + maxr) * 1.5 / 2.54) * 2.54)
    n = max(len(left), len(right), 1)
    h = (n - 1) * 2.54 + 5.08
    top = h/2
    xml = ['<symbol name="%s">' % name]
    xml += rect_wires(-w/2, -top, w/2, top, 94, 0.254)
    xml.append('<text x="%.2f" y="%.2f" size="1.778" layer="95">&gt;NAME</text>' % (-w/2, top+1.0))
    xml.append('<text x="%.2f" y="%.2f" size="1.778" layer="96" align="top-left">&gt;VALUE</text>' % (-w/2, -top-1.0))
    pins = {}
    for i, p in enumerate(left):
        y = top - 2.54 - i*2.54
        xml.append('<pin name="%s" x="%.2f" y="%.2f" length="short" direction="pas"/>' % (p, -w/2-2.54, y)); pins[p] = (-w/2-2.54, y, "L")
    for i, p in enumerate(right):
        y = top - 2.54 - i*2.54
        xml.append('<pin name="%s" x="%.2f" y="%.2f" length="short" direction="pas" rot="R180"/>' % (p, w/2+2.54, y)); pins[p] = (w/2+2.54, y, "R")
    xml.append('</symbol>')
    SYMBOLS[name] = ("\n".join(xml), pins)
    return name

# bluepill symbol (detailed), built directly
def bp_symbol():
    names_l = [BP_PINS["R%d" % (i+1)] for i in range(19, -1, -1)]   # pins 40..21 top to bottom
    names_r = [BP_PINS["L%d" % (i+1)] for i in range(19, -1, -1)]   # pins 20..1 top to bottom
    # display names: strip the __Xn suffix is not possible in pin names, so keep it
    return box_symbol("BLUEPILL", names_l, names_r)
bp_symbol()

DEVICESETS = {}  # name -> dict(prefix, symbol, package, pinpad: {pin: pad}, desc)
def addset(name, prefix, left, right, package, desc, pinpad=None, symname=None):
    sym = box_symbol(symname or ("SYM_" + name), left, right)
    pins = left + right
    DEVICESETS[name] = dict(prefix=prefix, symbol=sym, package=package, desc=desc,
                            pinpad=pinpad or {p: p for p in pins})
    return name

# Blue Pill
DEVICESETS["BLUEPILL_GD32F103C8T6"] = dict(prefix="U", symbol="BLUEPILL", package="BLUEPILL_40P",
    desc="Blue Pill GD32F103C8T6 module (pin names carry this project's pin roles)",
    pinpad={BP_PINS[k]: BP_PADNUM[k] for k in BP_PINS})
PACKAGES["BLUEPILL_40P"] = pkg_bluepill()[0]

addset("RES", "R", ["1"], ["2"], "AXIAL_10.16", "Resistor")
addset("CAP", "C", ["+"], ["-"], "RADIAL_2.5", "Electrolytic capacitor", {"+": "1", "-": "2"})
addset("CAP_CER", "C", ["1"], ["2"], "DISC_5.0", "Ceramic capacitor")
addset("DIODE", "D", ["A"], ["K"], "DO41_10.16", "Diode (K = banded end)", {"A": "2", "K": "1"})
addset("VARISTOR", "RV", ["1"], ["2"], "VARISTOR_7.5", "Varistor")
addset("FUSE", "F", ["1"], ["2"], "FUSE_5X20_22.5", "Fuse in a holder")
addset("NPN", "Q", ["B"], ["C", "E"], "TO92_2.54", "NPN transistor TO-92", {"B": "2", "C": "3", "E": "1"})
addset("HDR2", "J", ["1", "2"], [], "HDR_1X2", "2-pin header")
addset("HDR3", "J", ["1", "2", "3"], [], "HDR_1X3", "3-pin header")
addset("TERM3", "J", ["1", "2", "3"], [], "TERM_3P_5.08", "3-pole screw terminal")
addset("RELAY", "K", ["COIL_P", "COIL_N"], ["COM", "NO", "NC"], "RELAY_SRD_5PIN", "SPDT relay 5 V coil, use COM and NO",
       {"COIL_P": "1", "COIL_N": "2", "COM": "3", "NO": "4", "NC": "5"})
addset("ACDC5V", "PS", ["ACL", "ACN"], ["VO_P", "VO_N"], "ACDC_4PIN", "AC-DC 5V 3W module (HLK-PM01 style)",
       {"ACL": "1", "ACN": "2", "VO_N": "3", "VO_P": "4"})

# ---------------- part list ----------------
# ref, deviceset, value
PARTS = [
 ("U1","BLUEPILL_GD32F103C8T6","GD32F103C8T6"),
 ("J_AC","TERM3","AC IN L N PE"),("F1","FUSE","1A slow 250V"),("RV1","VARISTOR","275VAC"),("PS1","ACDC5V","5V 600mA"),
 ("F2","FUSE","2A slow 250V"),("K1","RELAY","5V SPDT (light)"),("K2","RELAY","5V SPDT (fan)"),
 ("J_LIGHT","TERM3","LIGHT L N PE"),("J_FAN","TERM3","FAN L N PE"),
 ("Q1","NPN","S8050"),("Q2","NPN","S8050"),
 ("R_B1","RES","1k"),("R_B2","RES","1k"),("R_PD1","RES","10k"),("R_PD2","RES","10k"),
 ("D1","DIODE","1N4007"),("D2","DIODE","1N4007"),("D3","DIODE","1N5819"),
 ("C1","CAP","470uF 10V"),("C2","CAP_CER","100nF"),
 ("J_PIR","HDR3","PIR 5V OUT GND"),("R_PIR_A","RES","1k"),("R_PIR_B","RES","20k DNP"),
 ("R_L1","RES","330R"),("R_L2","RES","330R"),("R_L3","RES","330R"),("R_L4","RES","330R"),("R_L5","RES","330R"),("R_L6","RES","330R"),
 ("J_LED1","HDR2","LED PIR"),("J_LED2","HDR2","LED LIGHT"),("J_LED3","HDR2","LED FAN"),("J_LED4","HDR2","LED FAULT"),
 ("J_LED5","HDR2","LED FAN ENABLED"),("J_LED6","HDR2","LED HEARTBEAT"),
 ("J_BTN1","HDR2","BTN LIGHT"),("J_BTN2","HDR2","BTN FAN"),
 ("J_LDR","HDR2","LDR"),("R_LDR","RES","10k"),
 ("J_DHT","HDR3","DHT DNP"),("R_DHT","RES","10k DNP"),("J_LDRD","HDR3","LDR DIGITAL DNP"),("J_UART","HDR2","UART TX GND"),
]
BPN = lambda lab_pad: BP_PINS[lab_pad]
def bp(label, side=None):
    """find Blue Pill pin name by header label (first match, or specified pad)"""
    for k, v in BP_PINS.items():
        if v.split("__")[0].split("_")[0] == label and (side is None or k == side): return v
    raise KeyError(label)
def bpr(pad): return BP_PINS[pad]

NETS = {}
def net(name, *pins):
    NETS.setdefault(name, []).extend(pins)
N = net
# mains
N("L_IN", ("J_AC","1"), ("F1","1"), ("F2","1"))
N("N", ("J_AC","2"), ("PS1","ACN"), ("RV1","2"), ("J_LIGHT","2"), ("J_FAN","2"))
N("PE", ("J_AC","3"), ("J_LIGHT","3"), ("J_FAN","3"))
N("L_FUSED_PS", ("F1","2"), ("PS1","ACL"), ("RV1","1"))
N("L_LOAD", ("F2","2"), ("K1","COM"), ("K2","COM"))
N("LIGHT_SWITCHED", ("K1","NO"), ("J_LIGHT","1"))
N("FAN_SWITCHED", ("K2","NO"), ("J_FAN","1"))
# 5 V
N("+5V", ("PS1","VO_P"), ("D3","A"), ("C1","+"), ("C2","1"), ("K1","COIL_P"), ("K2","COIL_P"), ("D1","K"), ("D2","K"), ("J_PIR","1"))
N("5V_MCU", ("D3","K"), (("U1"), bpr("L18")))
N("GND", ("PS1","VO_N"), ("C1","-"), ("C2","2"), ("Q1","E"), ("Q2","E"), ("R_PD1","2"), ("R_PD2","2"), ("J_PIR","3"),
  ("R_PIR_B","2"), ("R_LDR","2"), ("J_BTN1","2"), ("J_BTN2","2"), ("J_DHT","3"), ("J_LDRD","3"), ("J_UART","2"),
  ("J_LED1","2"), ("J_LED2","2"), ("J_LED3","2"), ("J_LED4","2"), ("J_LED5","2"), ("J_LED6","2"),
  ("U1", bpr("L19")), ("U1", bpr("R1")), ("U1", bpr("R2")))
N("3V3", ("U1", bpr("L20")), ("U1", bpr("R3")), ("J_LDR","1"), ("J_DHT","1"), ("R_DHT","1"), ("J_LDRD","1"))
# relay drivers
N("RELAY1_DRV", ("U1", bpr("R15")), ("R_B1","1"))
N("RELAY2_DRV", ("U1", bpr("R14")), ("R_B2","1"))
N("Q1_BASE", ("R_B1","2"), ("Q1","B"), ("R_PD1","1"))
N("Q2_BASE", ("R_B2","2"), ("Q2","B"), ("R_PD2","1"))
N("K1_COIL_LOW", ("K1","COIL_N"), ("Q1","C"), ("D1","A"))
N("K2_COIL_LOW", ("K2","COIL_N"), ("Q2","C"), ("D2","A"))
# PIR
N("PIR_RAW", ("J_PIR","2"), ("R_PIR_A","1"))
N("PIR_OUT", ("R_PIR_A","2"), ("R_PIR_B","1"), ("U1", bpr("R16")))
# LEDs: pad, resistor, header
for i, (pad) in enumerate(["R13","R12","R11","R10","R5","L2"]):
    n = i + 1
    N("LED%d_DRV" % n, ("U1", bpr(pad)), ("R_L%d" % n, "1"))
    N("LED%d_ANODE" % n, ("R_L%d" % n, "2"), ("J_LED%d" % n, "1"))
# inputs
N("BTN_LIGHT", ("U1", bpr("R8")), ("J_BTN1","1"))
N("BTN_FAN", ("U1", bpr("R7")), ("J_BTN2","1"))
N("LDR_ADC", ("U1", bpr("R9")), ("J_LDR","2"), ("R_LDR","1"))
N("DHT_DATA", ("U1", bpr("L1")), ("J_DHT","2"), ("R_DHT","2"))
N("LDR_DIGITAL", ("U1", bpr("R6")), ("J_LDRD","2"))
N("UART_TX", ("U1", bpr("L6")), ("J_UART","1"))
# normalise: pin entries given as ("U1", pin) or (ref, pin) or ((ref),pin) bug guard
def norm(p):
    ref, pin = p
    return (ref if isinstance(ref, str) else ref[0] if isinstance(ref, tuple) else ref, pin)
for k in list(NETS): NETS[k] = [norm(p) if not (isinstance(p[0], str) and p[0] == "U1" and False) else p for p in NETS[k]]
# fix the one tuple-in-tuple: ("D3","K") + (("U1"), pad) parses as ("U1", pad) already

# ---------------- write library xml ----------------
def library_xml(libname):
    o = ['<library name="%s"><description>AutoBath-Light-Fan parts (symbols are simple boxes; footprints marked PLACEHOLDER must be verified)</description>' % libname]
    o.append("<packages>" + "\n".join(PACKAGES.values()) + "</packages>")
    o.append("<symbols>" + "\n".join(v[0] for v in SYMBOLS.values()) + "</symbols>")
    o.append("<devicesets>")
    for name, d in DEVICESETS.items():
        o.append('<deviceset name="%s" prefix="%s"><description>%s</description>' % (name, d["prefix"], esc(d["desc"])))
        o.append('<gates><gate name="G$1" symbol="%s" x="0" y="0"/></gates>' % d["symbol"])
        o.append('<devices><device name="" package="%s"><connects>' % d["package"])
        for pin, pad in d["pinpad"].items():
            o.append('<connect gate="G$1" pin="%s" pad="%s"/>' % (pin, pad))
        o.append('</connects><technologies><technology name=""/></technologies></device></devices></deviceset>')
    o.append("</devicesets></library>")
    return "\n".join(o)

HEAD = '<?xml version="1.0" encoding="utf-8"?>\n<!DOCTYPE eagle SYSTEM "eagle.dtd">\n<eagle version="9.6.2"><drawing><settings><setting alwaysvectorfont="no"/><setting verticaltext="up"/></settings>\n<grid distance="2.54" unitdist="mm" unit="mm" style="lines" multiple="1" display="no" altdistance="0.254" altunitdist="mm" altunit="mm"/>\n'

def write_lbr():
    xml = HEAD + layers_xml() + library_xml("AutoBath") + "</drawing></eagle>\n"
    p = os.path.join(OUTDIR, "AutoBath_parts_and_BluePill.lbr")
    open(p, "w", encoding="utf-8", newline="\n").write(xml); return p

# ---------------- schematic ----------------
def write_sch():
    # layout: Blue Pill at origin; other parts in a grid to the right
    place = {}
    place["U1"] = (0.0, 0.0)
    others = [p for p in PARTS if p[0] != "U1"]
    cols, spacingx, spacingy = 4, 95.0, 30.0
    for idx, (ref, ds, val) in enumerate(others):
        c, r = idx % cols, idx // cols
        place[ref] = (110.0 + c*spacingx, 55.0 - r*spacingy)
    o = [HEAD.replace("<drawing>", "<drawing>")]
    o.append(layers_xml())
    o.append('<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R"><libraries>' + library_xml("AutoBath") + '</libraries>')
    o.append('<attributes/><variantdefs/><classes><class number="0" name="default" width="0" drill="0"/></classes>')
    o.append("<parts>")
    for ref, ds, val in PARTS:
        o.append('<part name="%s" library="AutoBath" deviceset="%s" device="" value="%s"/>' % (ref, ds, esc(val)))
    o.append("</parts><sheets><sheet><description>AutoBath-Light-Fan controller</description><plain>")
    o.append('<text x="-30" y="75" size="3" layer="97">AutoBath-Light-Fan: PIR bathroom light + exhaust fan controller (GD32F103C8T6 Blue Pill)</text>')
    o.append('<text x="-30" y="70" size="2" layer="97">Nets joined by name labels. Mains parts need a clearance review before any fabrication. Footprints marked PLACEHOLDER are unverified.</text>')
    o.append("</plain><instances>")
    for ref, ds, val in PARTS:
        x, y = place[ref]
        o.append('<instance part="%s" gate="G$1" x="%.2f" y="%.2f"/>' % (ref, x, y))
    o.append("</instances><busses/><nets>")
    psets = {ref: ds for ref, ds, _ in PARTS}
    pinpos = {}
    for ref, ds, val in PARTS:
        sym = DEVICESETS[ds]["symbol"]
        for pin, (px, py, side) in SYMBOLS[sym][1].items():
            pinpos[(ref, pin)] = (place[ref][0] + px, place[ref][1] + py, side)
    used = set()
    for name, pins in NETS.items():
        o.append('<net name="%s" class="0">' % esc(name))
        for ref, pin in pins:
            assert (ref, pin) in pinpos, (name, ref, pin)
            x, y, side = pinpos[(ref, pin)]
            used.add((ref, pin))
            L = 4.0 + 1.3*len(name)
            x2 = x - L if side == "L" else x + L
            o.append('<segment><pinref part="%s" gate="G$1" pin="%s"/>' % (ref, esc(pin)))
            o.append('<wire x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" width="0.1524" layer="91"/>' % (x, y, x2, y))
            lx = x2 if side == "L" else x
            o.append('<label x="%.2f" y="%.2f" size="1.27" layer="95"/></segment>' % (lx + (1.0 if side == "L" else 1.0), y))
        o.append("</net>")
    o.append("</nets></sheet></sheets></schematic></drawing></eagle>\n")
    p = os.path.join(OUTDIR, "AutoBath-Light-Fan_schematic.sch")
    open(p, "w", encoding="utf-8", newline="\n").write("\n".join(o)); return p

if __name__ == "__main__":
    print(write_lbr()); print(write_sch())
    print("parts", len(PARTS), "nets", len(NETS), "pinrefs", sum(len(v) for v in NETS.values()))
