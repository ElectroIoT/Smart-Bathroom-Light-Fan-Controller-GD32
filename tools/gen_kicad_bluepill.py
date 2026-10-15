"""Writes KiCad (6/7/8) footprint + symbol for the Blue Pill, from the same constants as the EasyEDA library.
Output: docs/pcb/kicad/BluePill_GD32F103C8T6.kicad_mod and BluePill_GD32F103C8T6.kicad_sym

Pin numbering follows the usual Blue Pill symbol: pins 1-20 on the right, bottom to top (B12 ... 3.3),
pins 21-40 on the left, bottom to top (GND ... VBAT). Row spacing 15.24 mm (0.6 in), pitch 2.54 mm.
KiCad's footprint Y axis points down, the 3D model Y axis points up, hence the sign flip on Y below.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gen_eagle_schematic as g

OUT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "docs", "pcb", "kicad")
os.makedirs(OUT, exist_ok=True)
ROW, PITCH, LEN, W = g.ROW, g.PITCH, g.LEN, g.W
NAME = "BluePill_GD32F103C8T6"
MODEL = "BluePill_GD32F103C8T6_WITH_header_kicad_scale.wrl"


def pin_name(i):                       # i = 1..40
    key = ("L%d" % i) if i <= 20 else ("R%d" % (i - 20))
    return g.BP_PINS[key].replace("​", "")


def footprint():
    o = ['(footprint "%s" (version 20221018) (generator gen_kicad_bluepill)' % NAME,
         '  (layer "F.Cu")',
         '  (descr "Blue Pill (STM32F103C8T6 / GD32F103C8T6) plug-in module, 2 x 20 pins, 2.54 mm pitch, rows 15.24 mm apart")',
         '  (tags "bluepill stm32 gd32 module")',
         '  (attr through_hole)',
         '  (fp_text reference "REF**" (at 0 %.2f) (layer "F.SilkS") (effects (font (size 1 1) (thickness 0.15))))' % (-W / 2 - 1.5),
         '  (fp_text value "%s" (at 0 %.2f) (layer "F.Fab") (effects (font (size 1 1) (thickness 0.15))))' % (NAME, W / 2 + 1.5)]
    hx, hy = LEN / 2, W / 2
    o.append('  (fp_rect (start %.2f %.2f) (end %.2f %.2f) (stroke (width 0.12) (type solid)) (fill none) (layer "F.SilkS"))' % (-hx, -hy, hx, hy))
    o.append('  (fp_rect (start %.2f %.2f) (end %.2f %.2f) (stroke (width 0.05) (type solid)) (fill none) (layer "F.CrtYd"))' % (-hx - 0.25, -hy - 0.25, hx + 0.25, hy + 0.25))
    x0 = (19 * PITCH) / 2
    for i in range(1, 41):
        if i <= 20:
            x, y = x0 - (i - 1) * PITCH, -ROW / 2            # model +Y row -> KiCad -Y
        else:
            x, y = x0 - (i - 21) * PITCH, ROW / 2
        shape = "rect" if i == 1 else "circle"
        o.append('  (pad "%d" thru_hole %s (at %.3f %.3f) (size 1.8 1.8) (drill 1.0) (layers "*.Cu" "*.Mask"))' % (i, shape, x, y))
    o.append('  (model "${KIPRJMOD}/%s" (offset (xyz 0 0 0)) (scale (xyz 1 1 1)) (rotate (xyz 0 0 0)))' % MODEL)
    o.append(')')
    return "\n".join(o) + "\n"


def symbol():
    h = 20 * 2.54 / 2 + 2.54
    o = ['(kicad_symbol_lib (version 20220914) (generator gen_kicad_bluepill)',
         '  (symbol "%s" (in_bom yes) (on_board yes)' % NAME,
         '    (property "Reference" "U" (at 0 %.2f 0) (effects (font (size 1.27 1.27))))' % (h + 2.0),
         '    (property "Value" "%s" (at 0 %.2f 0) (effects (font (size 1.27 1.27))))' % (NAME, -h - 2.0),
         '    (property "Footprint" "%s" (at 0 0 0) (effects (font (size 1.27 1.27)) hide))' % NAME,
         '    (property "Datasheet" "" (at 0 0 0) (effects (font (size 1.27 1.27)) hide))',
         '    (symbol "%s_0_1"' % NAME,
         '      (rectangle (start -7.62 %.2f) (end 7.62 %.2f) (stroke (width 0.254) (type default)) (fill (type background)))' % (h, -h),
         '    )',
         '    (symbol "%s_1_1"' % NAME]
    for i in range(1, 41):
        if i <= 20:                                          # right side, bottom to top
            x, rot, y = 10.16, 180, -h + 2.54 + (i - 1) * 2.54
        else:                                                # left side, bottom to top
            x, rot, y = -10.16, 0, -h + 2.54 + (i - 21) * 2.54
        o.append('      (pin passive line (at %.2f %.2f %d) (length 2.54) (name "%s" (effects (font (size 1.27 1.27)))) (number "%d" (effects (font (size 1.27 1.27)))))' % (x, y, rot, pin_name(i), i))
    o.append('    )')
    o.append('  )')
    o.append(')')
    return "\n".join(o) + "\n"


if __name__ == "__main__":
    open(os.path.join(OUT, NAME + ".kicad_mod"), "w", encoding="utf-8", newline="\n").write(footprint())
    open(os.path.join(OUT, NAME + ".kicad_sym"), "w", encoding="utf-8", newline="\n").write(symbol())
    print("written to", OUT)
