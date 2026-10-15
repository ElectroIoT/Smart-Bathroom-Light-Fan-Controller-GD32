#!/usr/bin/env python3
"""Generate 3D models of a Blue Pill style GD32F103C8T6 board (with and without header pins) for EasyEDA / KiCad.

  python tools/gen_bluepill_3d.py

Main models, written to docs/pcb/3d/ (each as VRML in mm "_mm.wrl", VRML in KiCad scale "_kicad_scale.wrl" where
1 unit = 2.54 mm, and an OBJ+MTL zip):

  BluePill_GD32F103C8T6_WITHOUT_header   the board only (PCB, pads, GD32F103C8T6, crystals, USB, reset, BOOT jumpers,
                                         LEDs, SWD header, silkscreen text). No pin headers.
  BluePill_GD32F103C8T6_WITH_header      the same board plus both 1x20 male pin headers (black plastic under the board,
                                         long gold pins down, tips above the board)

Extras, written to docs/pcb/3d/extras/ (reference or building blocks):
  PinHeader_1x20_male_strip, Socket_1x20_female_strip, WITH_header_and_sockets (assembly view),
  WITH_header_pinout_tags (colour-coded project pin tags beside the headers: for pictures, not for the PCB)

Origin of every model: the TOP surface of the host PCB under the module, at the centre of the board. The board floats at
Z0 = SOCKET_H + HDR_PLASTIC above it (the lift is built in, so no Z offset is needed in the 3D model manager).

ASSUMPTIONS TO CHECK ON YOUR BOARD (edit the constants and re-run): board 53.34 x 22.86 mm (2.1 x 0.9 in), header row
spacing ROW_MM = 20.32 mm (0.8 in), socket height SOCKET_H = 8.5 mm. Models are approximate, not an official drawing.
"""
import math
import os
import zipfile

# ---------------------------------------------------------------- dimensions (edit and re-run)
L_MM, W_MM, T_MM = 53.34, 22.86, 1.6     # board length (X, USB end = +X), width (Y), thickness
ROW_MM = 20.32                           # distance between the two header rows (ASSUMED: measure!)
PITCH = 2.54
SOCKET_H = 8.5                           # height of the female sockets on the host PCB (measure)
HDR_PLASTIC = 2.5                        # male header plastic block under the board
PIN_TIP = 3.0                            # header pin tip above the board top
PIN_IN = 6.0                             # pin length below the plastic (inserted into the socket)
Z0 = SOCKET_H + HDR_PLASTIC              # underside of the board (= lift above the host PCB)
ZT = Z0 + T_MM                           # top of the board

OUT_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "docs", "pcb", "3d")

# ---------------------------------------------------------------- materials (name: (r, g, b, shininess))
MATS = {
    "pcb":    (0.03, 0.20, 0.60, 0.25), "gold": (0.85, 0.65, 0.13, 0.60), "black": (0.06, 0.06, 0.06, 0.20),
    "chip":   (0.04, 0.04, 0.05, 0.30), "metal": (0.78, 0.78, 0.80, 0.80), "silk": (0.97, 0.97, 0.97, 0.10),
    "yellow": (0.95, 0.80, 0.10, 0.30), "green": (0.15, 0.85, 0.30, 0.50), "red": (0.90, 0.10, 0.10, 0.50),
    "tan":    (0.62, 0.52, 0.36, 0.20),
    "tag_pow": (0.75, 0.10, 0.10, 0.2), "tag_in": (0.08, 0.50, 0.22, 0.2), "tag_relay": (0.70, 0.48, 0.0, 0.2),
    "tag_led": (0.72, 0.08, 0.38, 0.2), "tag_ana": (0.85, 0.40, 0.02, 0.2), "tag_com": (0.42, 0.25, 0.72, 0.2),
    "tag_opt": (0.38, 0.38, 0.42, 0.2),
}
mesh = {m: ([], []) for m in MATS}        # material -> (points, quad faces)


def clear():
    for m in mesh:
        mesh[m][0].clear()
        mesh[m][1].clear()


def _rot(dx, dy, a):
    c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
    return dx * c - dy * s, dx * s + dy * c


def box(mat, cx, cy, z0, sx, sy, sz, rot=0.0, pivot=None):
    """Box centred on (cx, cy), base at z0, optionally rotated about `pivot` (default its own centre)."""
    px, py = (cx, cy) if pivot is None else pivot
    pts, faces = mesh[mat]
    b = len(pts)
    for z in (z0, z0 + sz):
        for dx, dy in ((-sx / 2, -sy / 2), (sx / 2, -sy / 2), (sx / 2, sy / 2), (-sx / 2, sy / 2)):
            rx, ry = _rot(cx + dx - px, cy + dy - py, rot)
            pts.append((px + rx, py + ry, z))
    faces += [(b, b + 3, b + 2, b + 1), (b + 4, b + 5, b + 6, b + 7), (b, b + 1, b + 5, b + 4),
              (b + 1, b + 2, b + 6, b + 5), (b + 2, b + 3, b + 7, b + 6), (b + 3, b, b + 4, b + 7)]


def quad(mat, p):
    pts, faces = mesh[mat]
    base = len(pts)
    pts.extend(p)
    faces.append((base, base + 1, base + 2, base + 3))


# ---------------------------------------------------------------- 5x7 pixel font (columns, bit0 = top row)
FONT = {
    "0": (0x3E, 0x51, 0x49, 0x45, 0x3E), "1": (0x00, 0x42, 0x7F, 0x40, 0x00), "2": (0x42, 0x61, 0x51, 0x49, 0x46),
    "3": (0x21, 0x41, 0x45, 0x4B, 0x31), "4": (0x18, 0x14, 0x12, 0x7F, 0x10), "5": (0x27, 0x45, 0x45, 0x45, 0x39),
    "6": (0x3C, 0x4A, 0x49, 0x49, 0x30), "7": (0x01, 0x71, 0x09, 0x05, 0x03), "8": (0x36, 0x49, 0x49, 0x49, 0x36),
    "9": (0x06, 0x49, 0x49, 0x29, 0x1E),
    "A": (0x7E, 0x11, 0x11, 0x11, 0x7E), "B": (0x7F, 0x49, 0x49, 0x49, 0x36), "C": (0x3E, 0x41, 0x41, 0x41, 0x22),
    "D": (0x7F, 0x41, 0x41, 0x22, 0x1C), "E": (0x7F, 0x49, 0x49, 0x49, 0x41), "F": (0x7F, 0x09, 0x09, 0x09, 0x01),
    "G": (0x3E, 0x41, 0x49, 0x49, 0x7A), "H": (0x7F, 0x08, 0x08, 0x08, 0x7F), "I": (0x00, 0x41, 0x7F, 0x41, 0x00),
    "K": (0x7F, 0x08, 0x14, 0x22, 0x41), "L": (0x7F, 0x40, 0x40, 0x40, 0x40), "M": (0x7F, 0x02, 0x0C, 0x02, 0x7F),
    "N": (0x7F, 0x04, 0x08, 0x10, 0x7F), "O": (0x3E, 0x41, 0x41, 0x41, 0x3E), "P": (0x7F, 0x09, 0x09, 0x09, 0x06),
    "R": (0x7F, 0x09, 0x19, 0x29, 0x46), "S": (0x46, 0x49, 0x49, 0x49, 0x31), "T": (0x01, 0x01, 0x7F, 0x01, 0x01),
    "U": (0x3F, 0x40, 0x40, 0x40, 0x3F), "V": (0x1F, 0x20, 0x40, 0x20, 0x1F), "W": (0x3F, 0x40, 0x38, 0x40, 0x3F),
    "X": (0x63, 0x14, 0x08, 0x14, 0x63), "Y": (0x07, 0x08, 0x70, 0x08, 0x07),
    ".": (0x00, 0x60, 0x60, 0x00, 0x00), "/": (0x20, 0x10, 0x08, 0x04, 0x02), "-": (0x08, 0x08, 0x08, 0x08, 0x08),
    "+": (0x08, 0x08, 0x3E, 0x08, 0x08), " ": (0, 0, 0, 0, 0),
}


def text(mat, s, ox, oy, z, d, u, ps, centre=True, mid=False):
    """Pixel text. (ox, oy) = baseline start (middle if centre). d = baseline direction, u = 'up' direction."""
    width = len(s) * 6 * ps - ps
    start = -width / 2 if centre else 0.0
    vshift = -3.5 * ps if mid else 0.0
    for ci, ch in enumerate(s):
        cols = FONT[ch]
        for col in range(5):
            for row in range(7):
                if (cols[col] >> row) & 1:
                    a = start + (ci * 6 + col) * ps
                    b = (6 - row) * ps + vshift
                    x0, y0 = ox + d[0] * a + u[0] * b, oy + d[1] * a + u[1] * b
                    quad(mat, [(x0, y0, z), (x0 + d[0] * ps, y0 + d[1] * ps, z),
                               (x0 + (d[0] + u[0]) * ps, y0 + (d[1] + u[1]) * ps, z), (x0 + u[0] * ps, y0 + u[1] * ps, z)])


LEFT = ["B12", "B13", "B14", "B15", "A8", "A9", "A10", "A11", "A12", "A15", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "5V", "G", "3.3"]
RIGHT = ["G", "G", "3.3", "R", "B11", "B10", "B1", "B0", "A7", "A6", "A5", "A4", "A3", "A2", "A1", "A0", "C15", "C14", "C13", "VB"]
XS = [(9.5 - i) * PITCH for i in range(20)]               # +24.13 ... -24.13 (USB end = +X)


# ---------------------------------------------------------------- parts
def part_board():
    """The Blue Pill board without its header pins."""
    box("pcb", 0, 0, Z0, L_MM, W_MM, T_MM)
    for y in (ROW_MM / 2, -ROW_MM / 2):
        for x in XS:
            box("gold", x, y, ZT, 1.7, 1.7, 0.05)                                   # pad
    ps = 0.13
    for i, name in enumerate(LEFT):                                                  # pin names (silkscreen)
        text("silk", name, XS[i], ROW_MM / 2 - 2.55, ZT + 0.01, (1, 0), (0, 1), ps)
    for i, name in enumerate(RIGHT):
        text("silk", name, XS[i], -ROW_MM / 2 + 2.55, ZT + 0.01, (-1, 0), (0, -1), ps)

    # micro USB (+X end), overhanging the edge
    box("metal", L_MM / 2 - 3.9, 0, ZT, 5.6, 7.5, 2.6)
    box("metal", L_MM / 2 - 0.1, 0, ZT, 1.9, 7.5, 2.6)
    box("black", L_MM / 2 + 0.85, 0, ZT + 0.55, 0.12, 5.8, 1.1)
    text("silk", "USB", L_MM / 2 - 8.2, 0, ZT + 0.01, (0, -1), (1, 0), 0.11)

    # GD32F103C8T6 LQFP-48 on the diagonal
    cx, cy, rot = 2.0, 0.0, 45.0
    box("chip", cx, cy, ZT, 7.0, 7.0, 1.4, rot)
    for k in range(12):
        off = (k - 5.5) * 0.5
        for lx, ly, sx, sy in ((3.95, off, 1.0, 0.22), (-3.95, off, 1.0, 0.22), (off, 3.95, 0.22, 1.0), (off, -3.95, 0.22, 1.0)):
            box("metal", cx + lx, cy + ly, ZT + 0.05, sx, sy, 0.16, rot, pivot=(cx, cy))
    c45 = math.cos(math.radians(45))
    box("silk", cx - 2.5 * c45, cy - 2.5 * c45, ZT + 1.4, 0.45, 0.45, 0.02, rot)      # pin-1 mark
    d45 = (c45, c45)
    u45 = (-c45, c45)
    text("silk", "GD32", cx + u45[0] * 0.9, cy + u45[1] * 0.9, ZT + 1.41, d45, u45, 0.12, mid=True)
    text("silk", "F103C8T6", cx - u45[0] * 0.9, cy - u45[1] * 0.9, ZT + 1.41, d45, u45, 0.11, mid=True)

    # crystals, reset button
    box("metal", -12.0, 0, ZT, 4.6, 10.4, 3.4)
    box("black", -12.0, 0, ZT + 3.4, 4.0, 9.8, 0.05)
    text("silk", "8.000", -12.0, 0, ZT + 3.46, (0, -1), (1, 0), 0.12, mid=True)
    box("metal", -6.5, 5.5, ZT, 1.5, 3.2, 0.9)
    box("black", 17.5, 4.5, ZT, 3.6, 5.0, 3.3)
    box("metal", 17.5, 4.5, ZT + 3.3, 2.6, 3.8, 0.3)
    box("black", 17.5, 4.5, ZT + 3.6, 2.0, 2.0, 0.8)
    text("silk", "RESET", 14.7, 4.5, ZT + 0.01, (0, -1), (1, 0), 0.10, mid=True)

    # BOOT0 / BOOT1 header (2x3) with two jumpers
    box("black", 14.0, -4.5, ZT, 7.62, 5.08, 2.5)
    for ix in (-2.54, 0.0, 2.54):
        for iy in (-1.27, 1.27):
            box("gold", 14.0 + ix, -4.5 + iy, ZT + 2.5, 0.64, 0.64, 3.0)
    for ix in (0.0, 2.54):
        box("yellow", 14.0 + ix, -4.5, ZT + 2.5, 2.4, 5.0, 5.5)
    text("silk", "BOOT", 8.7, -4.5, ZT + 0.01, (0, -1), (1, 0), 0.10, mid=True)

    # 3.3 V regulator (SOT-223), LEDs, small passives
    box("black", 12.0, 1.5, ZT, 3.5, 6.5, 1.6)
    box("metal", 9.7, 1.5, ZT, 1.2, 3.0, 0.3)
    box("green", -17.5, -5.2, ZT, 1.2, 2.0, 0.6)
    text("silk", "PC13", -20.5, -5.2, ZT + 0.01, (0, -1), (1, 0), 0.10, mid=True)
    box("red", -17.5, 5.2, ZT, 1.2, 2.0, 0.6)
    for (x, y, r) in ((-14.5, -5.2, 0), (-14.5, 5.2, 0), (-3.0, -6.2, 0), (-3.0, 6.2, 0), (7.0, -4.5, 90), (7.0, 4.5, 90),
                      (-9.0, 4.0, 0), (-9.0, -4.0, 0), (-14.5, 0.0, 90), (5.0, -6.2, 0), (5.0, 6.2, 0), (-6.5, -2.5, 90)):
        box("tan", x, y, ZT, 1.6, 0.8, 0.45, r)
    for (x, y) in ((0.0, 5.2), (0.0, -5.2), (-5.0, 0.0), (10.0, -4.0), (10.0, 4.5)):
        box("tan", x, y, ZT, 0.8, 1.6, 0.45)

    # SWD header (4 pins, bottom edge)
    sx = -L_MM / 2 + 3.0
    box("black", sx, 0, ZT, 2.5, 4 * PITCH, 2.5)
    for yy, name in zip((3.81, 1.27, -1.27, -3.81), ("3.3", "DIO", "CLK", "GND")):
        box("gold", sx, yy, ZT, 1.7, 1.7, 0.05)
        box("gold", sx, yy, ZT + 0.5, 0.64, 0.64, 8.0)
        text("silk", name, sx + 2.6, yy, ZT + 0.01, (1, 0), (0, 1), 0.075)
    text("silk", "SWD", sx + 4.6, 0, ZT + 0.01, (0, -1), (1, 0), 0.10, mid=True)


def part_header_strip(y=0.0):
    """One 1x20 male header: plastic block (sits on the socket) and long gold pins (down into the socket, tip above the board)."""
    box("black", 0, y, SOCKET_H, 20 * PITCH, 2.5, HDR_PLASTIC)
    for x in XS:
        box("gold", x, y, SOCKET_H - PIN_IN, 0.64, 0.64, PIN_IN + HDR_PLASTIC + T_MM + PIN_TIP)


def part_socket_strip(y=0.0):
    """One 1x20 female socket strip on the host PCB."""
    box("black", 0, y, 0.0, 20 * PITCH, 2.54, SOCKET_H)
    for x in XS:
        box("chip", x, y, SOCKET_H + 0.01, 1.0, 1.0, 0.02)
        box("gold", x, y, -1.5, 0.64, 0.64, 1.5)


def part_both_headers():
    for y in (ROW_MM / 2, -ROW_MM / 2):
        part_header_strip(y)


def part_both_sockets():
    for y in (ROW_MM / 2, -ROW_MM / 2):
        part_socket_strip(y)


# pin silk name -> (project function, tag material): only pins the AutoBath-Light-Fan firmware uses
TAGS_LEFT = {"B12": ("DHT OPT", "tag_opt"), "B13": ("LED HEART", "tag_led"), "A9": ("DEBUG TX", "tag_com"),
             "5V": ("+5V IN", "tag_pow"), "G": ("GND", "tag_pow"), "3.3": ("3V3", "tag_pow")}
TAGS_RIGHT = {"G": ("GND", "tag_pow"), "3.3": ("3V3", "tag_pow"), "B11": ("LED FANEN", "tag_led"), "B10": ("LDR DO OPT", "tag_opt"),
              "B1": ("BTN FAN", "tag_in"), "B0": ("BTN LIGHT", "tag_in"), "A7": ("LDR ADC", "tag_ana"), "A6": ("LED FAULT", "tag_led"),
              "A5": ("LED FAN", "tag_led"), "A4": ("LED LIGHT", "tag_led"), "A3": ("LED PIR", "tag_led"), "A2": ("RELAY2 FAN", "tag_relay"),
              "A1": ("RELAY1 LIGHT", "tag_relay"), "A0": ("PIR OUT", "tag_in"), "C13": ("ONBOARD LED", "tag_led")}


def part_tags():
    """Colour-coded project pin tags beside the headers, flat, reading left to right (reference pictures only)."""
    ps = 0.13
    for side in ("L", "R"):
        names = LEFT if side == "L" else RIGHT
        table = TAGS_LEFT if side == "L" else TAGS_RIGHT
        seen_ground = 0
        for i, nm in enumerate(names):
            if nm not in table:
                continue
            if side == "R" and nm == "G":
                seen_ground += 1
                if seen_ground > 1:
                    continue
            label, mat = table[nm]
            x = XS[i]
            length = len(label) * 6 * ps - ps + 1.0
            if side == "L":
                y0, y1 = W_MM / 2 + 0.6, W_MM / 2 + 0.6 + length          # plate outside the left edge
            else:
                y0, y1 = -W_MM / 2 - 0.6 - length, -W_MM / 2 - 0.6        # plate outside the right edge
            box(mat, x, (y0 + y1) / 2, ZT - 0.05, 2.2, y1 - y0, 0.3)
            text("silk", label, x, y1 - 0.5, ZT + 0.26, (0, -1), (1, 0), ps, centre=False, mid=True)   # reads left to right
            edge = ROW_MM / 2 if side == "L" else -ROW_MM / 2
            far = y0 if side == "L" else y1
            box("silk", x, (edge + far) / 2, ZT, 0.08, abs(far - edge), 0.02)


# ---------------------------------------------------------------- writers
def write_wrl(path, scale):
    out = ["#VRML V2.0 utf8", "# Blue Pill style GD32F103C8T6 board, generated by tools/gen_bluepill_3d.py (approximate model)",
           "# unit scale: 1 unit = %.2f mm, origin = top surface of the host PCB" % scale, ""]
    for name, (pts, faces) in mesh.items():
        if not faces:
            continue
        r, g, b, sh = MATS[name]
        out.append("DEF %s Shape {" % name.upper())
        out.append("  appearance Appearance { material Material { diffuseColor %.3f %.3f %.3f specularColor 0.4 0.4 0.4 shininess %.2f } }" % (r, g, b, sh))
        out.append("  geometry IndexedFaceSet {")
        out.append("    coord Coordinate { point [")
        out.append(",\n".join("      %.4f %.4f %.4f" % (x / scale, y / scale, z / scale) for x, y, z in pts))
        out.append("    ] }")
        out.append("    coordIndex [")
        out.append(",\n".join("      %d, %d, %d, %d, -1" % f for f in faces))
        out.append("    ]")
        out.append("    solid FALSE creaseAngle 0.5")
        out.append("  }")
        out.append("}")
    open(path, "w", encoding="ascii", newline="\n").write("\n".join(out) + "\n")


def write_obj_zip(zip_path, base):
    mtl = []
    used = [n for n, (_, f) in mesh.items() if f]
    for name in used:
        r, g, b, sh = MATS[name]
        mtl.append("newmtl %s\nKd %.3f %.3f %.3f\nKa 0.1 0.1 0.1\nKs 0.4 0.4 0.4\nNs %.0f\nd 1.0\n" % (name, r, g, b, sh * 100))
    lines = ["# generated by tools/gen_bluepill_3d.py (mm)", "mtllib %s.mtl" % base]
    off = 1
    for name in used:
        pts, faces = mesh[name]
        lines.append("o %s\nusemtl %s" % (name, name))
        lines += ["v %.4f %.4f %.4f" % p for p in pts]
        lines += ["f %d %d %d %d" % tuple(i + off for i in f) for f in faces]
        off += len(pts)
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr(base + ".obj", "\n".join(lines) + "\n")
        z.writestr(base + ".mtl", "\n".join(mtl))


MAIN = [("BluePill_GD32F103C8T6_WITHOUT_header", [part_board]),
        ("BluePill_GD32F103C8T6_WITH_header", [part_board, part_both_headers])]
EXTRAS = [("PinHeader_1x20_male_strip", [part_header_strip]),
          ("Socket_1x20_female_strip", [part_socket_strip]),
          ("BluePill_WITH_header_and_sockets_assembly", [part_board, part_both_headers, part_both_sockets]),
          ("BluePill_WITH_header_pinout_tags_reference", [part_board, part_both_headers, part_tags])]


def emit(folder, base, parts, full):
    clear()
    for fn in parts:
        fn()
    write_wrl(os.path.join(folder, base + "_mm.wrl"), 1.0)
    if full:
        write_wrl(os.path.join(folder, base + "_kicad_scale.wrl"), 2.54)
        write_obj_zip(os.path.join(folder, base + "_obj.zip"), base.lower())
    print("  %-52s faces %5d" % (base, sum(len(f) for _, f in mesh.values())))


def main():
    extras_dir = os.path.join(OUT_DIR, "extras")
    os.makedirs(extras_dir, exist_ok=True)
    for d in (OUT_DIR, extras_dir):                                  # remove files of older versions
        for f in os.listdir(d):
            if f.endswith((".wrl", ".zip")):
                os.remove(os.path.join(d, f))
    for base, parts in MAIN:
        emit(OUT_DIR, base, parts, True)
    for base, parts in EXTRAS:
        emit(extras_dir, base, parts, False)
    print("written to", OUT_DIR, "| board underside z =", Z0, "mm")


if __name__ == "__main__":
    main()
