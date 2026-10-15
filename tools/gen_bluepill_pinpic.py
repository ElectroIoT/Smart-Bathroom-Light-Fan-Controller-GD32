import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gen_eagle_schematic as g
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "docs", "pcb", "3d", "BluePill_pinout_all_pins.svg")
W, H = 1000, 1240
top = 150; pitch = 48
bx0, bx1 = 360, 640
cat = {"5V":"#c62828","3V3":"#ef6c00","VBAT":"#ef6c00","GND":"#212121","NRST":"#6a1b9a"}
def color(lab, role):
    if lab in cat: return cat[lab]
    if "LED" in role: return "#2e7d32"
    if "RELAY" in role: return "#1565c0"
    if role in ("PIR_OUT","LDR_ADC","BTN_FAN","BTN_LIGHT","LDR_DIGITAL_IN","DHT_DATA","UART_TX"): return "#00838f"
    if "USB" in role: return "#8d6e63"
    return "#9e9e9e"
o = ['<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 %d %d" width="%d" height="%d" font-family="Segoe UI, Arial, sans-serif">' % (W, H, W, H)]
o.append('<rect width="100%" height="100%" fill="#ffffff"/>')
o.append('<text x="500" y="44" text-anchor="middle" font-size="30" font-weight="700" fill="#0d2a5c">Blue Pill GD32F103C8T6 - all 40 pins</text>')
o.append('<text x="500" y="74" text-anchor="middle" font-size="16" fill="#555">Pin name (bold) and the role it has in the AutoBath-Light-Fan controller. USB end at the top.</text>')
bh = top + 19*pitch + 60
o.append('<rect x="%d" y="%d" width="%d" height="%d" rx="10" fill="#1b3a8a" stroke="#0b1f55" stroke-width="3"/>' % (bx0, top-60, bx1-bx0, bh-(top-60)+40))
# usb
o.append('<rect x="%d" y="%d" width="120" height="48" fill="#cfd8dc" stroke="#607d8b" stroke-width="2"/>' % (500-60, top-76))
o.append('<text x="500" y="%d" text-anchor="middle" font-size="14" fill="#263238">USB</text>' % (top-46))
o.append('<rect x="%d" y="%d" width="90" height="90" fill="#222" stroke="#000"/><text x="500" y="%d" text-anchor="middle" font-size="15" fill="#eee">GD32</text><text x="500" y="%d" text-anchor="middle" font-size="13" fill="#bbb">F103C8T6</text>' % (455, top+250, top+300, top+320))
o.append('<text x="500" y="%d" text-anchor="middle" font-size="16" fill="#cfe3ff">8 MHz</text>' % (top+190))
o.append('<text x="500" y="%d" text-anchor="middle" font-size="14" fill="#cfe3ff">SWD</text>' % (top+19*pitch+30))
o.append('<text x="500" y="%d" text-anchor="middle" font-size="14" fill="#cfe3ff">PC13 LED   BOOT0 / BOOT1</text>' % (top+19*pitch+8))
for side, data, x_pin, x_text, anchor in (("L", g.LEFT, bx0, bx0-50, "end"), ("R", g.RIGHT, bx1, bx1+50, "start")):
    for i, (lab, role) in enumerate(data):
        y = top + i*pitch
        c = color(lab, role)
        xe = x_pin - 30 if side == "L" else x_pin + 30
        o.append('<rect x="%d" y="%d" width="30" height="14" fill="#d4af37"/>' % ((x_pin-30) if side == "L" else x_pin, y-7))
        o.append('<circle cx="%d" cy="%d" r="5" fill="#b88a00"/>' % ((x_pin-15) if side == "L" else (x_pin+15), y))
        n = "%s%d" % (side, i+1)
        tx = x_text if side == "R" else x_text
        o.append('<text x="%d" y="%d" text-anchor="%s" font-size="20" font-weight="700" fill="%s">%s</text>' % (tx, y+7, anchor, c, lab))
        if role and role != lab:
            o.append('<text x="%d" y="%d" text-anchor="%s" font-size="14" fill="%s">%s</text>' % (tx, y+22, anchor, c, role))
        o.append('<text x="%d" y="%d" text-anchor="middle" font-size="12" fill="#9fb4e6">%d</text>' % ((x_pin+18) if side == "L" else (x_pin-18), y+4, i+1))
ly = H - 70
leg = [("#c62828","5V"),("#ef6c00","3.3 V / VBAT"),("#212121","GND"),("#2e7d32","LED output"),("#1565c0","Relay driver"),("#00838f","Sensor / button / UART"),("#9e9e9e","Unused")]
x = 40
for c, t in leg:
    o.append('<rect x="%d" y="%d" width="18" height="18" fill="%s"/><text x="%d" y="%d" font-size="14" fill="#333">%s</text>' % (x, ly, c, x+26, ly+14, t))
    x += 26 + 8*len(t) + 14
o.append('<text x="500" y="%d" text-anchor="middle" font-size="12" fill="#777">Header order follows the usual Blue Pill silkscreen. Check against your own board before wiring.</text>' % (H-20))
o.append('</svg>')
open(OUT, "w", encoding="utf-8").write("\n".join(o))
print(OUT)
