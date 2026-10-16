"""Per-part wiring diagrams (part -> Blue Pill pin, with pin names), same look as docs/img/led-wiring.svg.
Writes docs/img/{leds,relays,pir,buttons,ldr,humidity}-bluepill.svg.   Run: python tools/gen_part_wiring.py
Pins come from include/config.h; the relay stage is the opto-isolated one drawn in the AutoBath schematic."""
import os
import textwrap

OUT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "docs", "img")
BG, INK, BLUE, RED = "#f6f9ff", "#10264d", "#2a62c9", "#c62828"
COL = {"PA0": "#8e24aa", "PA1": "#f9a825", "PA2": "#00acc1", "PA3": "#d81b60", "PA4": "#f9a825", "PA5": "#00acc1",
       "PA6": "#e53935", "PA7": "#6d4c41", "PB0": "#3949ab", "PB1": "#3949ab", "PB10": "#6d4c41", "PB11": "#43a047",
       "PB12": "#00897b", "PB13": "#7dffc4", "5V": "#c62828", "3V3": "#ef6c00", "GND": "#000000"}
DARK_TEXT = {"PA1", "PA4", "PB13"}
STYLE = ("<style>.blink { animation: blink 1s steps(1) infinite; } "
         "@keyframes blink { 0%,100% { opacity: 1; } 50% { opacity: 0.15; } }</style>")


def esc(s):
    return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace("Ohm", "&#937;")


class Svg:
    def __init__(self, w, h, title):
        self.w, self.o = w, []
        self.o.append('<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 %d @H@" font-family="Segoe UI, Arial, Helvetica, sans-serif">' % w)
        self.o.append(STYLE)
        self.o.append('<rect width="%d" height="@H@" rx="14" fill="%s" stroke="#c9d6ee"/>' % (w, BG))
        self.text(w / 2, 32, title, 20, 700, INK, "middle")

    def text(self, x, y, s, size=13, weight=400, fill=INK, anchor="start"):
        self.o.append('<text x="%g" y="%g" font-size="%g" font-weight="%d" fill="%s" text-anchor="%s">%s</text>' % (x, y, size, weight, fill, anchor, esc(s)))

    def line(self, x1, y1, x2, y2, c="#444", w=2, dash=None):
        self.o.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"%s/>' % (x1, y1, x2, y2, c, w, ' stroke-dasharray="%s"' % dash if dash else ""))

    def poly(self, pts, c="#444", w=2):
        self.o.append('<polyline points="%s" fill="none" stroke="%s" stroke-width="%g"/>' % (" ".join("%g,%g" % p for p in pts), c, w))

    def box(self, x, y, w, h, fill="#fff", stroke="#444", rx=4, sw=2, dash=None):
        self.o.append('<rect x="%g" y="%g" width="%g" height="%g" rx="%g" fill="%s" stroke="%s" stroke-width="%g"%s/>' % (x, y, w, h, rx, fill, stroke, sw, ' stroke-dasharray="%s"' % dash if dash else ""))

    def dot(self, x, y):
        self.o.append('<circle cx="%g" cy="%g" r="3.5" fill="#444"/>' % (x, y))

    def zone(self, x, y, w, h, label, c):
        self.box(x, y, w, h, "none", c, 10, 1.5, "7 4")
        self.text(x + 14, y + 22, label, 14, 700, c)

    def pin(self, x, y, name, label=None):
        """Blue Pill pin tag; (x, y) = middle of its right edge, the wire starts there."""
        c = COL[name]
        self.box(x - 104, y - 15, 104, 30, c, c, 5, 1)
        self.text(x - 52, y + 5, label or name, 14, 700, "#000" if name in DARK_TEXT else "#fff", "middle")

    def board(self, x, y, w, h):
        self.box(x, y, w, h, "#e3edff", BLUE, 10, 2)
        self.text(x + 12, y + 24, "BLUE PILL", 15, 700, BLUE)
        self.text(x + 12, y + 42, "GD32F103C8T6", 12, 400, BLUE)
        self.text(x + 12, y + h - 12, "USB end at the top", 11, 400, "#5a6f99")

    def res(self, x, y, label, above=True):
        self.box(x, y - 10, 70, 20, "#fff", "#444", 0)
        self.text(x + 35, y - 16 if above else y + 28, label, 12, 400, INK, "middle")

    def led(self, x, y, c, blink=False, r=20):
        self.o.append('<circle cx="%g" cy="%g" r="%g" fill="%s"%s/>' % (x, y, r, c, ' class="blink"' if blink else ""))

    def gnd(self, x, y):
        self.line(x - 14, y, x + 14, y, "#000", 3); self.line(x - 9, y + 5, x + 9, y + 5, "#000", 3); self.line(x - 4, y + 10, x + 4, y + 10, "#000", 3)

    def notes(self, y, lines):
        rows = []
        for l in lines:
            rows += textwrap.wrap(l, 146, subsequent_indent="   ")
        h = 14 + 20 * len(rows)
        self.box(20, y, self.w - 40, h, "#fff6d6", "#b58900", 8, 1)
        for i, l in enumerate(rows):
            self.text(35, y + 22 + 20 * i, l, 13, 400, "#6b4e00")
        self.end = y + h + 18

    def save(self, name):
        open(os.path.join(OUT, name), "w", encoding="utf-8", newline="\n").write(("\n".join(self.o) + "\n</svg>\n").replace("@H@", str(self.end)))
        print(name)


# ---------------------------------------------------------------- LEDs
def leds():
    s = Svg(1000, 610, "All status LEDs: each LED to its own Blue Pill pin")
    s.zone(20, 55, 360, 420, "INSIDE the plastic box", BLUE)
    s.zone(380, 55, 600, 420, "OUTSIDE (box lid / front panel)", "#555")
    s.board(40, 90, 150, 370)
    rows = [("PA3", "PA3 (A3)", "PIR - lit while motion is seen", False), ("PA4", "PA4 (A4)", "LIGHT - lit while the light relay is ON", False),
            ("PA5", "PA5 (A5)", "FAN - lit while the fan relay is ON", False), ("PA6", "PA6 (A6)", "FAULT (red) - fast blink: PIR stuck HIGH", True),
            ("PB11", "PB11 (B11)", "FAN-ENABLED - steady on: fan active", False), ("PB13", "PB13 (B13)", "SYSTEM heartbeat - blinks once a second", True)]
    gx = 940
    for i, (p, lab, txt, bl) in enumerate(rows):
        y = 125 + i * 50
        s.pin(290, y, p, lab)
        s.line(290, y, 480, y); s.res(480, y, "330 Ohm"); s.line(550, y, 650, y)
        s.led(670, y, COL[p], bl); s.text(705, y - 8, txt, 13); s.line(690, y, gx, y, "#000", 2.5)
    s.poly([(290, 425), (gx, 425), (gx, 125)], "#000", 2.5)
    s.pin(290, 425, "GND")
    s.text(615, 417, "one common GND wire to every LED short leg (cathode)", 12, 400, "#000", "middle")
    s.notes(490, ["LED long leg (anode) on the resistor side, short leg to GND. 330 Ohm for red/green/yellow (100 Ohm for blue/white). A few mA each, no transistor.",
                  "Active HIGH: the pin goes HIGH to light the LED (STATUS_LED_ACTIVE_LOW 0). The board's own heartbeat LED is on PC13 (on-board, nothing to wire).",
                  "Six external LEDs are wired. Turn them all off in firmware with ENABLE_STATUS_LEDS 0. Real pin positions on the board: pinout.svg."])
    s.save("leds-bluepill.svg")


# ---------------------------------------------------------------- relays
def relay_channel(s, y0, ch, pin, name):
    b = y0 + 25
    s.zone(20, y0, 760, 245, "INSIDE the box - low voltage (Blue Pill + opto driver)", BLUE)
    s.box(780, y0, 200, 245, "none", RED, 10, 1.5, "7 4"); s.text(794, y0 + 22, "230 V side (terminal block)", 13, 700, RED)
    s.text(400, y0 + 22, "%s relay - channel %d" % (name, ch), 15, 700, INK)
    r5, rg, yi = b + 30, b + 190, b + 115
    s.pin(140, r5, "5V"); s.pin(140, yi, pin, "%s (A%s)" % (pin, pin[2:])); s.pin(140, rg, "GND")
    s.line(140, r5, 730, r5, COL["5V"], 2.5); s.text(300, r5 - 8, "+5V", 12, 700, COL["5V"])
    s.line(140, rg, 700, rg, "#000", 2.5); s.text(520, rg + 18, "GND", 12, 700, "#000")
    s.line(140, yi, 330, yi); s.text(160, yi - 8, "IN%d (active LOW)" % ch, 12, 700)
    # R2 from +5V to the opto LED anode (pin 1)
    s.line(240, r5, 240, b + 75); s.dot(240, r5); s.line(240, b + 75, 255, b + 75); s.res(255, b + 75, "R2 470 Ohm"); s.line(325, b + 75, 330, b + 75)
    # PC-817
    s.box(330, b + 55, 90, 85, "#fff", "#444", 4, 2); s.text(375, b + 102, "PC-817", 13, 700, INK, "middle")
    for t, x, yy, a in (("1", 337, b + 79, "start"), ("2", 337, b + 119, "start"), ("4", 413, b + 79, "end"), ("3", 413, b + 119, "end")):
        s.text(x, yy, t, 11, 700, "#888", a)
    s.line(420, b + 75, 440, b + 75); s.line(440, b + 75, 440, r5); s.dot(440, r5)
    # R1 to the S8050 base, R3 base -> GND
    s.line(420, yi, 450, yi); s.res(450, yi, "R1 2.2 k"); s.line(520, yi, 600, yi); s.dot(560, yi)
    s.line(560, yi, 560, b + 145); s.box(550, b + 145, 20, 35, "#fff", "#444", 0); s.text(578, b + 168, "R3 10 k", 12); s.line(560, b + 180, 560, rg); s.dot(560, rg)
    # S8050
    s.box(600, b + 100, 80, 36, "#fff", "#444", 4, 2); s.text(640, b + 123, "S8050 NPN", 12, 700, INK, "middle")
    s.text(606, b + 96, "B", 11, 700, "#888"); s.text(668, b + 96, "C", 11, 700, "#888"); s.text(668, b + 150, "E", 11, 700, "#888")
    s.line(660, b + 136, 660, rg); s.dot(660, rg)
    # coil + flyback diode
    s.box(645, r5 + 15, 30, 45, "#fff", "#444", 0); s.text(638, r5 + 40, "COIL", 12, 700, INK, "end"); s.text(638, r5 + 54, "5 V", 11, 400, INK, "end")
    s.line(660, r5, 660, r5 + 15); s.dot(660, r5); s.line(660, r5 + 60, 660, b + 100); s.dot(660, r5 + 75)
    s.line(730, r5, 730, r5 + 28); s.dot(730, r5)
    s.poly([(720, r5 + 50), (740, r5 + 50), (730, r5 + 30), (720, r5 + 50)]); s.line(719, r5 + 28, 741, r5 + 28, "#444", 3)
    s.line(730, r5 + 50, 730, r5 + 75); s.line(660, r5 + 75, 730, r5 + 75)
    s.text(742, r5 + 44, "1N4007", 10, 400, INK)
    # relay contacts -> terminal block
    s.box(790, b + 45, 90, 105, "#fff", RED, 4, 2); s.text(835, b + 68, "SRD-05VDC", 11, 700, RED, "middle"); s.text(835, b + 82, "-SL-C contacts", 11, 700, RED, "middle")
    for t, yy, c in (("COM", b + 100, RED), ("NO", b + 120, "#2e7d32"), ("NC", b + 140, "#777")):
        s.line(880, yy, 925, yy, c, 2.5); s.text(884, yy - 4, t, 11, 700, c)
    s.box(925, b + 85, 45, 65, "#2e7d32", "#1b5e20", 4, 1)
    for t, yy in (("COM", b + 103), ("NO", b + 123), ("NC", b + 143)):
        s.text(947, yy, t, 10, 700, "#fff", "middle")
    s.text(870, b + 172, "DG305-5.0-03P", 10, 400, INK, "middle")
    s.text(870, b + 186, "NO + COM switch the %s" % ("lamp" if ch == 1 else "fan"), 10, 400, INK, "middle")


def relays():
    s = Svg(1000, 800, "Both relays: Blue Pill pin -> opto driver -> relay -> terminal block")
    relay_channel(s, 52, 1, "PA1", "LIGHT")
    relay_channel(s, 307, 2, "PA2", "FAN")
    s.notes(565, ["Active LOW (RELAY_ACTIVE_LOW 1): pin goes LOW -> PC-817 LED lights (5 V through R2 470 Ohm) -> its transistor feeds the S8050 base through R1 2.2 k -> relay pulls in.",
                  "R3 10 k from the S8050 base to GND keeps the relay off while the Blue Pill boots. 1N4007 across the coil, bar to +5V. The 230 V side never touches the Blue Pill.",
                  "Contacts: COM = common (mains in), NO = closed while the relay is ON (lamp / fan), NC = closed while OFF. Stage as drawn in the AutoBath schematic: bench test with NO mains first.",
                  "A ready-made 2-channel relay module instead? IN1 -> PA1, IN2 -> PA2, VCC -> 5V, GND -> GND (see connections.svg).",
                  "Mains wiring only by a qualified electrician; keep mains copper well away from the Blue Pill side."])
    s.save("relays-bluepill.svg")


# ---------------------------------------------------------------- PIR
def pir():
    s = Svg(1000, 470, "PIR motion sensor (HC-SR501 type) to the Blue Pill")
    s.zone(20, 55, 960, 280, "Low voltage only", BLUE)
    s.board(40, 90, 150, 230)
    s.pin(290, 130, "5V"); s.pin(290, 190, "PA0", "PA0 (A0)"); s.pin(290, 250, "GND")
    s.box(620, 100, 200, 190, "#e8f5e9", "#2e7d32", 8, 2); s.text(720, 126, "PIR module", 15, 700, "#1b5e20", "middle")
    s.text(720, 146, "HC-SR501 type", 12, 400, "#1b5e20", "middle")
    s.o.append('<circle cx="720" cy="215" r="42" fill="#fff" stroke="#2e7d32" stroke-width="2"/><circle cx="720" cy="215" r="26" fill="#c8e6c9" stroke="#2e7d32"/>')
    for yy, t, c in ((130, "VCC", COL["5V"]), (190, "OUT", COL["PA0"]), (250, "GND", "#000")):
        s.line(290, yy, 620, yy, c, 2.5); s.box(620, yy - 10, 40, 20, "#fff", "#444", 3, 1.5); s.text(640, yy + 5, t, 11, 700, INK, "middle")
    s.text(455, 122, "+5 V", 12, 700, COL["5V"], "middle"); s.text(455, 182, "signal: HIGH = motion", 12, 700, COL["PA0"], "middle"); s.text(455, 242, "0 V", 12, 700, "#000", "middle")
    s.text(840, 140, "sensitivity pot", 12); s.text(840, 160, "time pot: minimum", 12); s.text(840, 180, "jumper: H (repeat)", 12)
    s.notes(350, ["PIR output is 3.3 V HIGH on motion (PIR_ACTIVE_HIGH 1); PA0 is set as input with pull-down (PIR_PULL). The firmware ignores the PIR for 60 s after power-up.",
                  "Set the PIR time pot to its minimum and the jumper to H (retrigger); the 7 min / 10 min timers are in firmware, not on the PIR. Power the PIR from 5V.",
                  "Mount it looking at the room, not at the lamp or a window. A PIR output stuck HIGH for 30 min raises the red FAULT LED (PA6).",
                  "PIR set-up and mounting: pir-setup.svg. Real pin positions on the board: pinout.svg."])
    s.save("pir-bluepill.svg")


# ---------------------------------------------------------------- buttons
def buttons():
    s = Svg(1000, 560, "Two push buttons and the FAN-ENABLED LED to the Blue Pill")
    s.zone(20, 55, 360, 330, "INSIDE the plastic box", BLUE)
    s.zone(380, 55, 600, 330, "OUTSIDE (box lid / front panel)", "#555")
    s.board(40, 90, 150, 280)
    items = [("PB0", "PB0 (B0)", "LIGHT override", "ON 30 min, again = cancel"),
             ("PB1", "PB1 (B1)", "FAN enable / disable", "toggle, kept in flash")]
    for i, (p, lab, nm, txt) in enumerate(items):
        y = 125 + i * 65
        s.pin(290, y, p, lab); s.line(290, y, 600, y)
        s.box(600, y - 14, 70, 28, "#cfd8dc", "#444", 14, 2); s.text(635, y + 5, "BUTTON", 11, 700, INK, "middle")
        s.line(670, y, 900, y); s.text(320, y - 22, nm + ": " + txt, 12)
        s.line(900, y, 900, 290, "#000", 2.5)
    s.pin(290, 290, "GND"); s.line(290, 290, 900, 290, "#000", 2.5); s.text(600, 282, "button other leg to GND (no resistor: internal pull-up)", 12, 400, "#000", "middle")
    y = 335
    s.pin(290, y, "PB11", "PB11 (B11)"); s.line(290, y, 480, y); s.res(480, y, "330 Ohm"); s.line(550, y, 650, y)
    s.led(670, y, COL["PB11"]); s.line(690, y, 900, y, "#000", 2.5); s.line(900, y, 900, 290, "#000", 2.5)
    s.text(705, y - 8, "FAN-ENABLED: on = fan active", 12)
    s.notes(405, ["Buttons are active LOW (BUTTON_ACTIVE_LOW 1): the pin is pulled up inside the chip, pressing connects it to GND. Debounce 40 ms in firmware.",
                  "Hold BOTH buttons for 5 s = LED silent mode (all LEDs dark except the red FAULT LED); hold again to bring them back.",
                  "Use buttons with their own cable up to a few metres; for long runs add a 100 nF capacitor across each button. Keep away from the 230 V wiring.",
                  "LED wiring for the other five LEDs: leds-bluepill.svg."])
    s.save("buttons-bluepill.svg")


# ---------------------------------------------------------------- LDR
def ldr():
    s = Svg(1000, 560, "Daylight sensor (bare LDR, or a digital LDR module) to the Blue Pill")
    s.zone(20, 55, 960, 290, "Low voltage only (optional, DAYLIGHT_SENSOR)", BLUE)
    s.board(40, 90, 150, 240)
    # option A: bare LDR divider
    s.text(330, 100, "Option A: bare LDR + 10 k divider (DAYLIGHT_SENSOR 2)", 13, 700, INK)
    s.pin(290, 130, "3V3"); s.pin(290, 185, "PA7", "PA7 (A7) ADC"); s.pin(290, 290, "GND")
    s.line(290, 130, 560, 130, COL["3V3"], 2.5)
    s.box(560, 112, 90, 36, "#ffe0b2", "#e65100", 18, 2); s.text(605, 135, "LDR", 13, 700, "#e65100", "middle")
    s.line(650, 130, 700, 130, COL["3V3"], 2.5); s.line(700, 130, 700, 160, COL["3V3"], 2.5)
    s.line(290, 185, 700, 185, COL["PA7"], 2.5); s.dot(700, 185)
    s.line(700, 160, 700, 185, "#444", 2)
    s.line(700, 185, 700, 215); s.box(690, 215, 20, 45, "#fff", "#444", 0); s.text(680, 243, "10 k", 12, 400, INK, "end"); s.line(700, 260, 700, 290)
    s.line(290, 290, 700, 290, "#000", 2.5)
    s.text(430, 177, "node between LDR and 10 k", 12, 700, COL["PA7"], "middle")
    s.text(760, 130, "bright = low resistance = higher voltage", 12); s.text(760, 150, "face it at the window, shield from the lamp", 12)
    # option B: digital module
    s.text(740, 215, "Option B: digital LDR module", 13, 700, INK); s.text(740, 232, "(DAYLIGHT_SENSOR 1)", 12)
    s.text(740, 252, "VCC to 3V3, GND to GND, DO to PB10", 12)
    s.text(740, 270, "set DAYLIGHT_BRIGHT_LEVEL to match", 12)
    s.box(740, 285, 100, 28, COL["PB10"], COL["PB10"], 5, 1); s.text(790, 304, "PB10 (B10)", 13, 700, "#fff", "middle")
    s.notes(365, ["Bare LDR: 3V3 -> LDR -> PA7 -> 10 k -> GND, read with the ADC (12 bit). Open the debug serial (PA9, 9600) to print the ADC value every 5 s, note day and night,",
                  "then set LDR_BRIGHT_ADC and LDR_DARK_ADC between them (LDR_INVERT 1 if wired the other way). Many modules output LOW when bright: set DAYLIGHT_BRIGHT_LEVEL.",
                  "When the room is bright, a visit does not switch the light on, but the fan still runs. The sensor is read only while the lamp is off; a reading must hold for 10 s.",
                  "A missing sensor reads as dark, so the light keeps working. PA7 is an analog input: never feed it more than 3.3 V.",
                  "Option A and B are alternatives: wire one of them."])
    s.save("ldr-bluepill.svg")


# ---------------------------------------------------------------- humidity (DHT)
def humidity():
    s = Svg(1000, 470, "Optional humidity sensor DHT11 / DHT22 to the Blue Pill")
    s.zone(20, 55, 960, 270, "Low voltage only (optional, HUMIDITY_ENABLE 1)", BLUE)
    s.board(40, 90, 150, 220)
    s.pin(290, 130, "3V3"); s.pin(290, 190, "PB12", "PB12 (B12)"); s.pin(290, 270, "GND")
    s.box(620, 100, 200, 190, "#e1f5fe", "#0277bd", 8, 2); s.text(745, 135, "DHT22 (AM2302)", 15, 700, "#01579b", "middle"); s.text(745, 155, "or DHT11 module", 12, 400, "#01579b", "middle")
    for yy, t, c in ((130, "VCC", COL["3V3"]), (190, "DATA", COL["PB12"]), (270, "GND", "#000")):
        s.line(290, yy, 620, yy, c, 2.5); s.box(620, yy - 10, 44, 20, "#fff", "#444", 3, 1.5); s.text(642, yy + 5, t, 11, 700, INK, "middle")
    s.line(500, 130, 500, 150, COL["3V3"], 2.5); s.box(490, 150, 20, 22, "#fff", "#444", 0); s.line(500, 172, 500, 190, COL["PB12"], 2.5); s.dot(500, 190)
    s.dot(500, 130); s.text(520, 166, "10 k pull-up", 12); s.text(405, 122, "3.3 V", 12, 700, COL["3V3"], "middle")
    s.text(405, 182, "one-wire data", 12, 700, COL["PB12"], "middle")
    s.notes(345, ["Off by default (HUMIDITY_ENABLE 0). Set it to 1 and pick HUMIDITY_TYPE 11 (DHT11) or 22 (DHT22). Humid at 70 % RH or more starts the fan, normal again at 62 % or less.",
                  "The data line needs the 10 k pull-up to 3V3 (bare 4-pin sensors; many 3-pin modules already have it). A failing sensor lights the red FAULT LED steady.",
                  "Real pin positions on the board: pinout.svg."])
    s.save("humidity-bluepill.svg")


if __name__ == "__main__":
    for f in (leds, relays, pir, buttons, ldr, humidity):
        f()
