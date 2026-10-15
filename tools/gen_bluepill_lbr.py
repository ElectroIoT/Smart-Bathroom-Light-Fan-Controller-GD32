import os
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "docs", "pcb", "3d", "BluePill_GD32F103C8T6.lbr")
L = ["B12","B13","B14","B15","A8","A9","A10","A11","A12","A15","B3","B4","B5","B6","B7","B8","B9","5V","GND","3V3"]
R = ["GND","GND","3V3","NRST","B11","B10","B1","B0","A7","A6","A5","A4","A3","A2","A1","A0","C15","C14","C13","VBAT"]
# header names as on silkscreen: PB12 etc
def nm(s):
    return s if s in ("5V","GND","3V3","NRST","VBAT") else "P"+s
ROW = 20.32; PITCH = 2.54; LEN = 53.34; W = 22.86
pins = []  # (padname, signal)
for i, s in enumerate(L): pins.append(("L%d" % (i+1), nm(s)))
for i, s in enumerate(R): pins.append(("R%d" % (i+1), nm(s)))
o = []
o.append('<?xml version="1.0" encoding="utf-8"?>')
o.append('<!DOCTYPE eagle SYSTEM "eagle.dtd">')
o.append('<eagle version="9.6.2"><drawing><settings><setting alwaysvectorfont="no"/><setting verticaltext="up"/></settings>')
o.append('<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>')
o.append('<layers>')
for n, name, c in [(1,"Top",4),(16,"Bottom",1),(17,"Pads",2),(18,"Vias",2),(20,"Dimension",15),(21,"tPlace",7),(22,"bPlace",7),(25,"tNames",7),(26,"bNames",7),(27,"tValues",7),(28,"bValues",7),(51,"tDocu",7),(52,"bDocu",7),(91,"Nets",2),(92,"Busses",1),(93,"Pins",2),(94,"Symbols",4),(95,"Names",7),(96,"Values",7)]:
    o.append('<layer number="%d" name="%s" color="%d" fill="1" visible="yes" active="yes"/>' % (n, name, c))
o.append('</layers><library>')
o.append('<description>Blue Pill (GD32F103C8T6 / STM32F103C8T6) 2x20 module, 2.54 mm pitch, rows 20.32 mm apart (assumed, measure your board)</description>')
o.append('<packages><package name="BLUEPILL_2X20"><description>Blue Pill module, 53.34 x 22.86 mm</description>')
x0 = -(19*PITCH)/2
for i in range(20):
    x = x0 + i*PITCH
    o.append('<pad name="L%d" x="%.3f" y="%.3f" drill="1.0" diameter="1.8" shape="%s"/>' % (i+1, x, ROW/2, "square" if i==0 else "round"))
    o.append('<pad name="R%d" x="%.3f" y="%.3f" drill="1.0" diameter="1.8" shape="%s"/>' % (i+1, x, -ROW/2, "square" if i==0 else "round"))
hx, hy = LEN/2, W/2
o.append('<wire x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" width="0.2" layer="21"/>' % (-hx,-hy,hx,-hy))
o.append('<wire x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" width="0.2" layer="21"/>' % (hx,-hy,hx,hy))
o.append('<wire x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" width="0.2" layer="21"/>' % (hx,hy,-hx,hy))
o.append('<wire x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" width="0.2" layer="21"/>' % (-hx,hy,-hx,-hy))
o.append('<text x="0" y="0" size="1.27" layer="25" align="center">&gt;NAME</text>')
o.append('<text x="0" y="-2" size="1.27" layer="27" align="center">&gt;VALUE</text>')
o.append('</package></packages>')
o.append('<symbols><symbol name="BLUEPILL">')
h = 20*2.54/2 + 2.54
o.append('<wire x1="-12.7" y1="%.2f" x2="12.7" y2="%.2f" width="0.254" layer="94"/>' % (h, h))
o.append('<wire x1="12.7" y1="%.2f" x2="12.7" y2="%.2f" width="0.254" layer="94"/>' % (h, -h))
o.append('<wire x1="12.7" y1="%.2f" x2="-12.7" y2="%.2f" width="0.254" layer="94"/>' % (-h, -h))
o.append('<wire x1="-12.7" y1="%.2f" x2="-12.7" y2="%.2f" width="0.254" layer="94"/>' % (-h, h))
o.append('<text x="-12.7" y="%.2f" size="1.778" layer="95">&gt;NAME</text>' % (h+1))
o.append('<text x="-12.7" y="%.2f" size="1.778" layer="96" align="top-left">&gt;VALUE</text>' % (-h-1))
for i in range(20):
    y = (9.5 - i) * 2.54
    o.append('<pin name="%s" x="-17.78" y="%.2f" length="middle" direction="pas"/>' % (nm(L[i]) + "_L%d" % (i+1), y))
    o.append('<pin name="%s" x="17.78" y="%.2f" length="middle" direction="pas" rot="R180"/>' % (nm(R[i]) + "_R%d" % (i+1), y))
o.append('</symbol></symbols>')
o.append('<devicesets><deviceset name="BLUEPILL_GD32F103C8T6" prefix="U"><description>Blue Pill GD32F103C8T6 module</description>')
o.append('<gates><gate name="G$1" symbol="BLUEPILL" x="0" y="0"/></gates>')
o.append('<devices><device name="" package="BLUEPILL_2X20"><connects>')
for i in range(20):
    o.append('<connect gate="G$1" pin="%s" pad="L%d"/>' % (nm(L[i]) + "_L%d" % (i+1), i+1))
    o.append('<connect gate="G$1" pin="%s" pad="R%d"/>' % (nm(R[i]) + "_R%d" % (i+1), i+1))
o.append('</connects><technologies><technology name=""/></technologies></device></devices></deviceset></devicesets>')
o.append('</library></drawing></eagle>')
open(OUT, "w", encoding="utf-8", newline="\n").write("\n".join(o) + "\n")
print("written", OUT)
