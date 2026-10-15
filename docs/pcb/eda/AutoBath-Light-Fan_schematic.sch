<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2"><drawing><settings><setting alwaysvectorfont="no"/><setting verticaltext="up"/></settings>
<grid distance="2.54" unitdist="mm" unit="mm" style="lines" multiple="1" display="no" altdistance="0.254" altunitdist="mm" altunit="mm"/>

<layers><layer number="1" name="Top" color="4" fill="1" visible="yes" active="yes"/><layer number="16" name="Bottom" color="1" fill="1" visible="yes" active="yes"/><layer number="17" name="Pads" color="2" fill="1" visible="yes" active="yes"/><layer number="18" name="Vias" color="2" fill="1" visible="yes" active="yes"/><layer number="20" name="Dimension" color="15" fill="1" visible="yes" active="yes"/><layer number="21" name="tPlace" color="7" fill="1" visible="yes" active="yes"/><layer number="22" name="bPlace" color="7" fill="1" visible="yes" active="yes"/><layer number="25" name="tNames" color="7" fill="1" visible="yes" active="yes"/><layer number="26" name="bNames" color="7" fill="1" visible="yes" active="yes"/><layer number="27" name="tValues" color="7" fill="1" visible="yes" active="yes"/><layer number="28" name="bValues" color="7" fill="1" visible="yes" active="yes"/><layer number="51" name="tDocu" color="7" fill="1" visible="yes" active="yes"/><layer number="52" name="bDocu" color="7" fill="1" visible="yes" active="yes"/><layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/><layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/><layer number="93" name="Pins" color="2" fill="1" visible="yes" active="yes"/><layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/><layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/><layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/><layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/><layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/></layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R"><libraries><library name="AutoBath"><description>AutoBath-Light-Fan parts (symbols are simple boxes; footprints marked PLACEHOLDER must be verified)</description>
<packages><package name="HDR_1X2"><description>1x2 pin header 2.54 mm</description>
<pad name="1" x="-1.270" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="1.270" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-2.540" y1="-1.300" x2="2.540" y2="-1.300" width="0.2" layer="21"/>
<wire x1="2.540" y1="-1.300" x2="2.540" y2="1.300" width="0.2" layer="21"/>
<wire x1="2.540" y1="1.300" x2="-2.540" y2="1.300" width="0.2" layer="21"/>
<wire x1="-2.540" y1="1.300" x2="-2.540" y2="-1.300" width="0.2" layer="21"/>
<text x="0" y="2.80" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-2.80" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="HDR_1X3"><description>1x3 pin header 2.54 mm</description>
<pad name="1" x="-2.540" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="0.000" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<pad name="3" x="2.540" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-3.810" y1="-1.300" x2="3.810" y2="-1.300" width="0.2" layer="21"/>
<wire x1="3.810" y1="-1.300" x2="3.810" y2="1.300" width="0.2" layer="21"/>
<wire x1="3.810" y1="1.300" x2="-3.810" y2="1.300" width="0.2" layer="21"/>
<wire x1="-3.810" y1="1.300" x2="-3.810" y2="-1.300" width="0.2" layer="21"/>
<text x="0" y="2.80" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-2.80" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="TERM_3P_5.08"><description>3-pole screw terminal 5.08 mm</description>
<pad name="1" x="-5.080" y="0.000" drill="1.30" diameter="2.60" shape="square"/>
<pad name="2" x="0.000" y="0.000" drill="1.30" diameter="2.60" shape="round"/>
<pad name="3" x="5.080" y="0.000" drill="1.30" diameter="2.60" shape="round"/>
<wire x1="-7.750" y1="-5.000" x2="7.750" y2="-5.000" width="0.2" layer="21"/>
<wire x1="7.750" y1="-5.000" x2="7.750" y2="5.000" width="0.2" layer="21"/>
<wire x1="7.750" y1="5.000" x2="-7.750" y2="5.000" width="0.2" layer="21"/>
<wire x1="-7.750" y1="5.000" x2="-7.750" y2="-5.000" width="0.2" layer="21"/>
<text x="0" y="6.50" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-6.50" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="AXIAL_10.16"><description>axial resistor 0.4in (0207)</description>
<pad name="1" x="-5.080" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="5.080" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-4.250" y1="-1.300" x2="4.250" y2="-1.300" width="0.2" layer="21"/>
<wire x1="4.250" y1="-1.300" x2="4.250" y2="1.300" width="0.2" layer="21"/>
<wire x1="4.250" y1="1.300" x2="-4.250" y2="1.300" width="0.2" layer="21"/>
<wire x1="-4.250" y1="1.300" x2="-4.250" y2="-1.300" width="0.2" layer="21"/>
<text x="0" y="2.80" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-2.80" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="DO41_10.16"><description>DO-41 diode, pad 1 is the cathode (banded end)</description>
<pad name="1" x="-5.080" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="5.080" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-4.250" y1="-1.750" x2="4.250" y2="-1.750" width="0.2" layer="21"/>
<wire x1="4.250" y1="-1.750" x2="4.250" y2="1.750" width="0.2" layer="21"/>
<wire x1="4.250" y1="1.750" x2="-4.250" y2="1.750" width="0.2" layer="21"/>
<wire x1="-4.250" y1="1.750" x2="-4.250" y2="-1.750" width="0.2" layer="21"/>
<text x="0" y="3.25" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-3.25" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="RADIAL_2.5"><description>radial electrolytic 2.5 mm lead pitch (6.3 mm can)</description>
<pad name="1" x="-1.250" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="1.250" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-3.250" y1="-3.250" x2="3.250" y2="-3.250" width="0.2" layer="21"/>
<wire x1="3.250" y1="-3.250" x2="3.250" y2="3.250" width="0.2" layer="21"/>
<wire x1="3.250" y1="3.250" x2="-3.250" y2="3.250" width="0.2" layer="21"/>
<wire x1="-3.250" y1="3.250" x2="-3.250" y2="-3.250" width="0.2" layer="21"/>
<text x="0" y="4.75" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-4.75" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="DISC_5.0"><description>radial ceramic 5 mm pitch</description>
<pad name="1" x="-2.500" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="2.500" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-3.000" y1="-1.500" x2="3.000" y2="-1.500" width="0.2" layer="21"/>
<wire x1="3.000" y1="-1.500" x2="3.000" y2="1.500" width="0.2" layer="21"/>
<wire x1="3.000" y1="1.500" x2="-3.000" y2="1.500" width="0.2" layer="21"/>
<wire x1="-3.000" y1="1.500" x2="-3.000" y2="-1.500" width="0.2" layer="21"/>
<text x="0" y="3.00" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-3.00" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="VARISTOR_7.5"><description>varistor disc, 7.5 mm pitch</description>
<pad name="1" x="-3.750" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="3.750" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-4.500" y1="-2.000" x2="4.500" y2="-2.000" width="0.2" layer="21"/>
<wire x1="4.500" y1="-2.000" x2="4.500" y2="2.000" width="0.2" layer="21"/>
<wire x1="4.500" y1="2.000" x2="-4.500" y2="2.000" width="0.2" layer="21"/>
<wire x1="-4.500" y1="2.000" x2="-4.500" y2="-2.000" width="0.2" layer="21"/>
<text x="0" y="3.50" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-3.50" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="FUSE_5X20_22.5"><description>5x20 mm fuse holder, 22.5 mm pitch (check yours)</description>
<pad name="1" x="-11.250" y="0.000" drill="1.30" diameter="2.60" shape="square"/>
<pad name="2" x="11.250" y="0.000" drill="1.30" diameter="2.60" shape="round"/>
<wire x1="-13.000" y1="-4.000" x2="13.000" y2="-4.000" width="0.2" layer="21"/>
<wire x1="13.000" y1="-4.000" x2="13.000" y2="4.000" width="0.2" layer="21"/>
<wire x1="13.000" y1="4.000" x2="-13.000" y2="4.000" width="0.2" layer="21"/>
<wire x1="-13.000" y1="4.000" x2="-13.000" y2="-4.000" width="0.2" layer="21"/>
<text x="0" y="5.50" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-5.50" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="TO92_2.54"><description>TO-92 (pin order E,B,C for S8050 flat face to you; check the datasheet)</description>
<pad name="1" x="-2.540" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="0.000" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<pad name="3" x="2.540" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-2.500" y1="-2.250" x2="2.500" y2="-2.250" width="0.2" layer="21"/>
<wire x1="2.500" y1="-2.250" x2="2.500" y2="2.250" width="0.2" layer="21"/>
<wire x1="2.500" y1="2.250" x2="-2.500" y2="2.250" width="0.2" layer="21"/>
<wire x1="-2.500" y1="2.250" x2="-2.500" y2="-2.250" width="0.2" layer="21"/>
<text x="0" y="3.75" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-3.75" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="RELAY_SRD_5PIN"><description>SRD-05VDC relay style, PLACEHOLDER pad positions: verify against the maker drawing</description>
<pad name="1" x="0.000" y="0.000" drill="1.00" diameter="1.80" shape="square"/>
<pad name="2" x="0.000" y="-7.500" drill="1.00" diameter="1.80" shape="round"/>
<pad name="3" x="14.200" y="-7.500" drill="1.00" diameter="1.80" shape="round"/>
<pad name="4" x="14.200" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<pad name="5" x="-7.500" y="0.000" drill="1.00" diameter="1.80" shape="round"/>
<wire x1="-9.500" y1="-8.000" x2="9.500" y2="-8.000" width="0.2" layer="21"/>
<wire x1="9.500" y1="-8.000" x2="9.500" y2="8.000" width="0.2" layer="21"/>
<wire x1="9.500" y1="8.000" x2="-9.500" y2="8.000" width="0.2" layer="21"/>
<wire x1="-9.500" y1="8.000" x2="-9.500" y2="-8.000" width="0.2" layer="21"/>
<text x="0" y="9.50" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-9.50" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="ACDC_4PIN"><description>AC-DC module style (HLK-PM01), PLACEHOLDER pad positions: verify against the datasheet</description>
<pad name="1" x="-15.000" y="5.000" drill="1.20" diameter="2.40" shape="square"/>
<pad name="2" x="-15.000" y="-5.000" drill="1.20" diameter="2.40" shape="round"/>
<pad name="3" x="15.000" y="-5.000" drill="1.20" diameter="2.40" shape="round"/>
<pad name="4" x="15.000" y="5.000" drill="1.20" diameter="2.40" shape="round"/>
<wire x1="-17.500" y1="-10.000" x2="17.500" y2="-10.000" width="0.2" layer="21"/>
<wire x1="17.500" y1="-10.000" x2="17.500" y2="10.000" width="0.2" layer="21"/>
<wire x1="17.500" y1="10.000" x2="-17.500" y2="10.000" width="0.2" layer="21"/>
<wire x1="-17.500" y1="10.000" x2="-17.500" y2="-10.000" width="0.2" layer="21"/>
<text x="0" y="11.50" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-11.50" size="1.27" layer="27" align="center">&gt;VALUE</text></package>
<package name="BLUEPILL_40P"><description>Blue Pill module 53.34 x 22.86 mm, rows 20.32 mm apart (assumed, measure yours)</description>
<pad name="1" x="24.130" y="10.160" drill="1.0" diameter="1.8" shape="square"/>
<pad name="21" x="24.130" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="2" x="21.590" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="22" x="21.590" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="3" x="19.050" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="23" x="19.050" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="4" x="16.510" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="24" x="16.510" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="5" x="13.970" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="25" x="13.970" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="6" x="11.430" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="26" x="11.430" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="7" x="8.890" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="27" x="8.890" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="8" x="6.350" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="28" x="6.350" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="9" x="3.810" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="29" x="3.810" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="10" x="1.270" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="30" x="1.270" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="11" x="-1.270" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="31" x="-1.270" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="12" x="-3.810" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="32" x="-3.810" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="13" x="-6.350" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="33" x="-6.350" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="14" x="-8.890" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="34" x="-8.890" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="15" x="-11.430" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="35" x="-11.430" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="16" x="-13.970" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="36" x="-13.970" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="17" x="-16.510" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="37" x="-16.510" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="18" x="-19.050" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="38" x="-19.050" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="19" x="-21.590" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="39" x="-21.590" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="20" x="-24.130" y="10.160" drill="1.0" diameter="1.8" shape="round"/>
<pad name="40" x="-24.130" y="-10.160" drill="1.0" diameter="1.8" shape="round"/>
<wire x1="-26.670" y1="-11.430" x2="26.670" y2="-11.430" width="0.2" layer="21"/>
<wire x1="26.670" y1="-11.430" x2="26.670" y2="11.430" width="0.2" layer="21"/>
<wire x1="26.670" y1="11.430" x2="-26.670" y2="11.430" width="0.2" layer="21"/>
<wire x1="-26.670" y1="11.430" x2="-26.670" y2="-11.430" width="0.2" layer="21"/>
<text x="0" y="0" size="1.27" layer="25" align="center">&gt;NAME</text><text x="0" y="-2" size="1.27" layer="27" align="center">&gt;VALUE</text></package></packages>
<symbols><symbol name="BLUEPILL">
<wire x1="-6.350" y1="-26.670" x2="6.350" y2="-26.670" width="0.254" layer="94"/>
<wire x1="6.350" y1="-26.670" x2="6.350" y2="26.670" width="0.254" layer="94"/>
<wire x1="6.350" y1="26.670" x2="-6.350" y2="26.670" width="0.254" layer="94"/>
<wire x1="-6.350" y1="26.670" x2="-6.350" y2="-26.670" width="0.254" layer="94"/>
<text x="-6.35" y="27.67" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-27.67" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="VBT" x="-8.89" y="24.13" length="short" direction="pas"/>
<pin name="C13" x="-8.89" y="21.59" length="short" direction="pas"/>
<pin name="C14" x="-8.89" y="19.05" length="short" direction="pas"/>
<pin name="C15" x="-8.89" y="16.51" length="short" direction="pas"/>
<pin name="A0" x="-8.89" y="13.97" length="short" direction="pas"/>
<pin name="A1" x="-8.89" y="11.43" length="short" direction="pas"/>
<pin name="A2" x="-8.89" y="8.89" length="short" direction="pas"/>
<pin name="A3" x="-8.89" y="6.35" length="short" direction="pas"/>
<pin name="A4" x="-8.89" y="3.81" length="short" direction="pas"/>
<pin name="A5" x="-8.89" y="1.27" length="short" direction="pas"/>
<pin name="A6" x="-8.89" y="-1.27" length="short" direction="pas"/>
<pin name="A7" x="-8.89" y="-3.81" length="short" direction="pas"/>
<pin name="B0" x="-8.89" y="-6.35" length="short" direction="pas"/>
<pin name="B1" x="-8.89" y="-8.89" length="short" direction="pas"/>
<pin name="B10" x="-8.89" y="-11.43" length="short" direction="pas"/>
<pin name="B11" x="-8.89" y="-13.97" length="short" direction="pas"/>
<pin name="RES" x="-8.89" y="-16.51" length="short" direction="pas"/>
<pin name="3.3​" x="-8.89" y="-19.05" length="short" direction="pas"/>
<pin name="GND​​" x="-8.89" y="-21.59" length="short" direction="pas"/>
<pin name="GND​" x="-8.89" y="-24.13" length="short" direction="pas"/>
<pin name="3.3" x="8.89" y="24.13" length="short" direction="pas" rot="R180"/>
<pin name="GND" x="8.89" y="21.59" length="short" direction="pas" rot="R180"/>
<pin name="5V" x="8.89" y="19.05" length="short" direction="pas" rot="R180"/>
<pin name="B9" x="8.89" y="16.51" length="short" direction="pas" rot="R180"/>
<pin name="B8" x="8.89" y="13.97" length="short" direction="pas" rot="R180"/>
<pin name="B7" x="8.89" y="11.43" length="short" direction="pas" rot="R180"/>
<pin name="B6" x="8.89" y="8.89" length="short" direction="pas" rot="R180"/>
<pin name="B5" x="8.89" y="6.35" length="short" direction="pas" rot="R180"/>
<pin name="B4" x="8.89" y="3.81" length="short" direction="pas" rot="R180"/>
<pin name="B3" x="8.89" y="1.27" length="short" direction="pas" rot="R180"/>
<pin name="A15" x="8.89" y="-1.27" length="short" direction="pas" rot="R180"/>
<pin name="A12" x="8.89" y="-3.81" length="short" direction="pas" rot="R180"/>
<pin name="A11" x="8.89" y="-6.35" length="short" direction="pas" rot="R180"/>
<pin name="A10" x="8.89" y="-8.89" length="short" direction="pas" rot="R180"/>
<pin name="A9" x="8.89" y="-11.43" length="short" direction="pas" rot="R180"/>
<pin name="A8" x="8.89" y="-13.97" length="short" direction="pas" rot="R180"/>
<pin name="B15" x="8.89" y="-16.51" length="short" direction="pas" rot="R180"/>
<pin name="B14" x="8.89" y="-19.05" length="short" direction="pas" rot="R180"/>
<pin name="B13" x="8.89" y="-21.59" length="short" direction="pas" rot="R180"/>
<pin name="B12" x="8.89" y="-24.13" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_RES">
<wire x1="-6.350" y1="-2.540" x2="6.350" y2="-2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="-2.540" x2="6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="2.540" x2="-6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="-6.350" y1="2.540" x2="-6.350" y2="-2.540" width="0.254" layer="94"/>
<text x="-6.35" y="3.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-3.54" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="1" x="-8.89" y="0.00" length="short" direction="pas"/>
<pin name="2" x="8.89" y="0.00" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_CAP">
<wire x1="-6.350" y1="-2.540" x2="6.350" y2="-2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="-2.540" x2="6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="2.540" x2="-6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="-6.350" y1="2.540" x2="-6.350" y2="-2.540" width="0.254" layer="94"/>
<text x="-6.35" y="3.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-3.54" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="+" x="-8.89" y="0.00" length="short" direction="pas"/>
<pin name="-" x="8.89" y="0.00" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_CAP_CER">
<wire x1="-6.350" y1="-2.540" x2="6.350" y2="-2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="-2.540" x2="6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="2.540" x2="-6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="-6.350" y1="2.540" x2="-6.350" y2="-2.540" width="0.254" layer="94"/>
<text x="-6.35" y="3.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-3.54" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="1" x="-8.89" y="0.00" length="short" direction="pas"/>
<pin name="2" x="8.89" y="0.00" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_DIODE">
<wire x1="-6.350" y1="-2.540" x2="6.350" y2="-2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="-2.540" x2="6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="2.540" x2="-6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="-6.350" y1="2.540" x2="-6.350" y2="-2.540" width="0.254" layer="94"/>
<text x="-6.35" y="3.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-3.54" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="A" x="-8.89" y="0.00" length="short" direction="pas"/>
<pin name="K" x="8.89" y="0.00" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_VARISTOR">
<wire x1="-6.350" y1="-2.540" x2="6.350" y2="-2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="-2.540" x2="6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="2.540" x2="-6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="-6.350" y1="2.540" x2="-6.350" y2="-2.540" width="0.254" layer="94"/>
<text x="-6.35" y="3.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-3.54" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="1" x="-8.89" y="0.00" length="short" direction="pas"/>
<pin name="2" x="8.89" y="0.00" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_FUSE">
<wire x1="-6.350" y1="-2.540" x2="6.350" y2="-2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="-2.540" x2="6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="6.350" y1="2.540" x2="-6.350" y2="2.540" width="0.254" layer="94"/>
<wire x1="-6.350" y1="2.540" x2="-6.350" y2="-2.540" width="0.254" layer="94"/>
<text x="-6.35" y="3.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-3.54" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="1" x="-8.89" y="0.00" length="short" direction="pas"/>
<pin name="2" x="8.89" y="0.00" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_NPN">
<wire x1="-6.350" y1="-3.810" x2="6.350" y2="-3.810" width="0.254" layer="94"/>
<wire x1="6.350" y1="-3.810" x2="6.350" y2="3.810" width="0.254" layer="94"/>
<wire x1="6.350" y1="3.810" x2="-6.350" y2="3.810" width="0.254" layer="94"/>
<wire x1="-6.350" y1="3.810" x2="-6.350" y2="-3.810" width="0.254" layer="94"/>
<text x="-6.35" y="4.81" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-4.81" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="B" x="-8.89" y="1.27" length="short" direction="pas"/>
<pin name="C" x="8.89" y="1.27" length="short" direction="pas" rot="R180"/>
<pin name="E" x="8.89" y="-1.27" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_HDR2">
<wire x1="-6.350" y1="-3.810" x2="6.350" y2="-3.810" width="0.254" layer="94"/>
<wire x1="6.350" y1="-3.810" x2="6.350" y2="3.810" width="0.254" layer="94"/>
<wire x1="6.350" y1="3.810" x2="-6.350" y2="3.810" width="0.254" layer="94"/>
<wire x1="-6.350" y1="3.810" x2="-6.350" y2="-3.810" width="0.254" layer="94"/>
<text x="-6.35" y="4.81" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-4.81" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="1" x="-8.89" y="1.27" length="short" direction="pas"/>
<pin name="2" x="-8.89" y="-1.27" length="short" direction="pas"/>
</symbol>
<symbol name="SYM_HDR3">
<wire x1="-6.350" y1="-5.080" x2="6.350" y2="-5.080" width="0.254" layer="94"/>
<wire x1="6.350" y1="-5.080" x2="6.350" y2="5.080" width="0.254" layer="94"/>
<wire x1="6.350" y1="5.080" x2="-6.350" y2="5.080" width="0.254" layer="94"/>
<wire x1="-6.350" y1="5.080" x2="-6.350" y2="-5.080" width="0.254" layer="94"/>
<text x="-6.35" y="6.08" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-6.08" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="1" x="-8.89" y="2.54" length="short" direction="pas"/>
<pin name="2" x="-8.89" y="0.00" length="short" direction="pas"/>
<pin name="3" x="-8.89" y="-2.54" length="short" direction="pas"/>
</symbol>
<symbol name="SYM_TERM3">
<wire x1="-6.350" y1="-5.080" x2="6.350" y2="-5.080" width="0.254" layer="94"/>
<wire x1="6.350" y1="-5.080" x2="6.350" y2="5.080" width="0.254" layer="94"/>
<wire x1="6.350" y1="5.080" x2="-6.350" y2="5.080" width="0.254" layer="94"/>
<wire x1="-6.350" y1="5.080" x2="-6.350" y2="-5.080" width="0.254" layer="94"/>
<text x="-6.35" y="6.08" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-6.08" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="1" x="-8.89" y="2.54" length="short" direction="pas"/>
<pin name="2" x="-8.89" y="0.00" length="short" direction="pas"/>
<pin name="3" x="-8.89" y="-2.54" length="short" direction="pas"/>
</symbol>
<symbol name="SYM_RELAY">
<wire x1="-7.620" y1="-5.080" x2="7.620" y2="-5.080" width="0.254" layer="94"/>
<wire x1="7.620" y1="-5.080" x2="7.620" y2="5.080" width="0.254" layer="94"/>
<wire x1="7.620" y1="5.080" x2="-7.620" y2="5.080" width="0.254" layer="94"/>
<wire x1="-7.620" y1="5.080" x2="-7.620" y2="-5.080" width="0.254" layer="94"/>
<text x="-7.62" y="6.08" size="1.778" layer="95">&gt;NAME</text>
<text x="-7.62" y="-6.08" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="COIL_P" x="-10.16" y="2.54" length="short" direction="pas"/>
<pin name="COIL_N" x="-10.16" y="0.00" length="short" direction="pas"/>
<pin name="COM" x="10.16" y="2.54" length="short" direction="pas" rot="R180"/>
<pin name="NO" x="10.16" y="0.00" length="short" direction="pas" rot="R180"/>
<pin name="NC" x="10.16" y="-2.54" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="SYM_ACDC5V">
<wire x1="-6.350" y1="-3.810" x2="6.350" y2="-3.810" width="0.254" layer="94"/>
<wire x1="6.350" y1="-3.810" x2="6.350" y2="3.810" width="0.254" layer="94"/>
<wire x1="6.350" y1="3.810" x2="-6.350" y2="3.810" width="0.254" layer="94"/>
<wire x1="-6.350" y1="3.810" x2="-6.350" y2="-3.810" width="0.254" layer="94"/>
<text x="-6.35" y="4.81" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-4.81" size="1.778" layer="96" align="top-left">&gt;VALUE</text>
<pin name="ACL" x="-8.89" y="1.27" length="short" direction="pas"/>
<pin name="ACN" x="-8.89" y="-1.27" length="short" direction="pas"/>
<pin name="VO_P" x="8.89" y="1.27" length="short" direction="pas" rot="R180"/>
<pin name="VO_N" x="8.89" y="-1.27" length="short" direction="pas" rot="R180"/>
</symbol></symbols>
<devicesets>
<deviceset name="BLUEPILL_GD32F103C8T6" prefix="U"><description>Blue Pill GD32F103C8T6 module (pin names carry this project's pin roles)</description>
<gates><gate name="G$1" symbol="BLUEPILL" x="0" y="0"/></gates>
<devices><device name="" package="BLUEPILL_40P"><connects>
<connect gate="G$1" pin="B12" pad="1"/>
<connect gate="G$1" pin="B13" pad="2"/>
<connect gate="G$1" pin="B14" pad="3"/>
<connect gate="G$1" pin="B15" pad="4"/>
<connect gate="G$1" pin="A8" pad="5"/>
<connect gate="G$1" pin="A9" pad="6"/>
<connect gate="G$1" pin="A10" pad="7"/>
<connect gate="G$1" pin="A11" pad="8"/>
<connect gate="G$1" pin="A12" pad="9"/>
<connect gate="G$1" pin="A15" pad="10"/>
<connect gate="G$1" pin="B3" pad="11"/>
<connect gate="G$1" pin="B4" pad="12"/>
<connect gate="G$1" pin="B5" pad="13"/>
<connect gate="G$1" pin="B6" pad="14"/>
<connect gate="G$1" pin="B7" pad="15"/>
<connect gate="G$1" pin="B8" pad="16"/>
<connect gate="G$1" pin="B9" pad="17"/>
<connect gate="G$1" pin="5V" pad="18"/>
<connect gate="G$1" pin="GND" pad="19"/>
<connect gate="G$1" pin="3.3" pad="20"/>
<connect gate="G$1" pin="GND​" pad="21"/>
<connect gate="G$1" pin="GND​​" pad="22"/>
<connect gate="G$1" pin="3.3​" pad="23"/>
<connect gate="G$1" pin="RES" pad="24"/>
<connect gate="G$1" pin="B11" pad="25"/>
<connect gate="G$1" pin="B10" pad="26"/>
<connect gate="G$1" pin="B1" pad="27"/>
<connect gate="G$1" pin="B0" pad="28"/>
<connect gate="G$1" pin="A7" pad="29"/>
<connect gate="G$1" pin="A6" pad="30"/>
<connect gate="G$1" pin="A5" pad="31"/>
<connect gate="G$1" pin="A4" pad="32"/>
<connect gate="G$1" pin="A3" pad="33"/>
<connect gate="G$1" pin="A2" pad="34"/>
<connect gate="G$1" pin="A1" pad="35"/>
<connect gate="G$1" pin="A0" pad="36"/>
<connect gate="G$1" pin="C15" pad="37"/>
<connect gate="G$1" pin="C14" pad="38"/>
<connect gate="G$1" pin="C13" pad="39"/>
<connect gate="G$1" pin="VBT" pad="40"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="RES" prefix="R"><description>Resistor</description>
<gates><gate name="G$1" symbol="SYM_RES" x="0" y="0"/></gates>
<devices><device name="" package="AXIAL_10.16"><connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="CAP" prefix="C"><description>Electrolytic capacitor</description>
<gates><gate name="G$1" symbol="SYM_CAP" x="0" y="0"/></gates>
<devices><device name="" package="RADIAL_2.5"><connects>
<connect gate="G$1" pin="+" pad="1"/>
<connect gate="G$1" pin="-" pad="2"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="CAP_CER" prefix="C"><description>Ceramic capacitor</description>
<gates><gate name="G$1" symbol="SYM_CAP_CER" x="0" y="0"/></gates>
<devices><device name="" package="DISC_5.0"><connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="DIODE" prefix="D"><description>Diode (K = banded end)</description>
<gates><gate name="G$1" symbol="SYM_DIODE" x="0" y="0"/></gates>
<devices><device name="" package="DO41_10.16"><connects>
<connect gate="G$1" pin="A" pad="2"/>
<connect gate="G$1" pin="K" pad="1"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="VARISTOR" prefix="RV"><description>Varistor</description>
<gates><gate name="G$1" symbol="SYM_VARISTOR" x="0" y="0"/></gates>
<devices><device name="" package="VARISTOR_7.5"><connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="FUSE" prefix="F"><description>Fuse in a holder</description>
<gates><gate name="G$1" symbol="SYM_FUSE" x="0" y="0"/></gates>
<devices><device name="" package="FUSE_5X20_22.5"><connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="NPN" prefix="Q"><description>NPN transistor TO-92</description>
<gates><gate name="G$1" symbol="SYM_NPN" x="0" y="0"/></gates>
<devices><device name="" package="TO92_2.54"><connects>
<connect gate="G$1" pin="B" pad="2"/>
<connect gate="G$1" pin="C" pad="3"/>
<connect gate="G$1" pin="E" pad="1"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="HDR2" prefix="J"><description>2-pin header</description>
<gates><gate name="G$1" symbol="SYM_HDR2" x="0" y="0"/></gates>
<devices><device name="" package="HDR_1X2"><connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="HDR3" prefix="J"><description>3-pin header</description>
<gates><gate name="G$1" symbol="SYM_HDR3" x="0" y="0"/></gates>
<devices><device name="" package="HDR_1X3"><connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="TERM3" prefix="J"><description>3-pole screw terminal</description>
<gates><gate name="G$1" symbol="SYM_TERM3" x="0" y="0"/></gates>
<devices><device name="" package="TERM_3P_5.08"><connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="RELAY" prefix="K"><description>SPDT relay 5 V coil, use COM and NO</description>
<gates><gate name="G$1" symbol="SYM_RELAY" x="0" y="0"/></gates>
<devices><device name="" package="RELAY_SRD_5PIN"><connects>
<connect gate="G$1" pin="COIL_P" pad="1"/>
<connect gate="G$1" pin="COIL_N" pad="2"/>
<connect gate="G$1" pin="COM" pad="3"/>
<connect gate="G$1" pin="NO" pad="4"/>
<connect gate="G$1" pin="NC" pad="5"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
<deviceset name="ACDC5V" prefix="PS"><description>AC-DC 5V 3W module (HLK-PM01 style)</description>
<gates><gate name="G$1" symbol="SYM_ACDC5V" x="0" y="0"/></gates>
<devices><device name="" package="ACDC_4PIN"><connects>
<connect gate="G$1" pin="ACL" pad="1"/>
<connect gate="G$1" pin="ACN" pad="2"/>
<connect gate="G$1" pin="VO_N" pad="3"/>
<connect gate="G$1" pin="VO_P" pad="4"/>
</connects><technologies><technology name=""/></technologies></device></devices></deviceset>
</devicesets></library></libraries>
<attributes/><variantdefs/><classes><class number="0" name="default" width="0" drill="0"/></classes>
<parts>
<part name="U1" library="AutoBath" deviceset="BLUEPILL_GD32F103C8T6" device="" value="GD32F103C8T6"/>
<part name="J_AC" library="AutoBath" deviceset="TERM3" device="" value="AC IN L N PE"/>
<part name="F1" library="AutoBath" deviceset="FUSE" device="" value="1A slow 250V"/>
<part name="RV1" library="AutoBath" deviceset="VARISTOR" device="" value="275VAC"/>
<part name="PS1" library="AutoBath" deviceset="ACDC5V" device="" value="5V 600mA"/>
<part name="F2" library="AutoBath" deviceset="FUSE" device="" value="2A slow 250V"/>
<part name="K1" library="AutoBath" deviceset="RELAY" device="" value="5V SPDT (light)"/>
<part name="K2" library="AutoBath" deviceset="RELAY" device="" value="5V SPDT (fan)"/>
<part name="J_LIGHT" library="AutoBath" deviceset="TERM3" device="" value="LIGHT L N PE"/>
<part name="J_FAN" library="AutoBath" deviceset="TERM3" device="" value="FAN L N PE"/>
<part name="Q1" library="AutoBath" deviceset="NPN" device="" value="S8050"/>
<part name="Q2" library="AutoBath" deviceset="NPN" device="" value="S8050"/>
<part name="R_B1" library="AutoBath" deviceset="RES" device="" value="1k"/>
<part name="R_B2" library="AutoBath" deviceset="RES" device="" value="1k"/>
<part name="R_PD1" library="AutoBath" deviceset="RES" device="" value="10k"/>
<part name="R_PD2" library="AutoBath" deviceset="RES" device="" value="10k"/>
<part name="D1" library="AutoBath" deviceset="DIODE" device="" value="1N4007"/>
<part name="D2" library="AutoBath" deviceset="DIODE" device="" value="1N4007"/>
<part name="D3" library="AutoBath" deviceset="DIODE" device="" value="1N5819"/>
<part name="C1" library="AutoBath" deviceset="CAP" device="" value="470uF 10V"/>
<part name="C2" library="AutoBath" deviceset="CAP_CER" device="" value="100nF"/>
<part name="J_PIR" library="AutoBath" deviceset="HDR3" device="" value="PIR 5V OUT GND"/>
<part name="R_PIR_A" library="AutoBath" deviceset="RES" device="" value="1k"/>
<part name="R_PIR_B" library="AutoBath" deviceset="RES" device="" value="20k DNP"/>
<part name="R_L1" library="AutoBath" deviceset="RES" device="" value="330R"/>
<part name="R_L2" library="AutoBath" deviceset="RES" device="" value="330R"/>
<part name="R_L3" library="AutoBath" deviceset="RES" device="" value="330R"/>
<part name="R_L4" library="AutoBath" deviceset="RES" device="" value="330R"/>
<part name="R_L5" library="AutoBath" deviceset="RES" device="" value="330R"/>
<part name="R_L6" library="AutoBath" deviceset="RES" device="" value="330R"/>
<part name="J_LED1" library="AutoBath" deviceset="HDR2" device="" value="LED PIR"/>
<part name="J_LED2" library="AutoBath" deviceset="HDR2" device="" value="LED LIGHT"/>
<part name="J_LED3" library="AutoBath" deviceset="HDR2" device="" value="LED FAN"/>
<part name="J_LED4" library="AutoBath" deviceset="HDR2" device="" value="LED FAULT"/>
<part name="J_LED5" library="AutoBath" deviceset="HDR2" device="" value="LED FAN ENABLED"/>
<part name="J_LED6" library="AutoBath" deviceset="HDR2" device="" value="LED HEARTBEAT"/>
<part name="J_BTN1" library="AutoBath" deviceset="HDR2" device="" value="BTN LIGHT"/>
<part name="J_BTN2" library="AutoBath" deviceset="HDR2" device="" value="BTN FAN"/>
<part name="J_LDR" library="AutoBath" deviceset="HDR2" device="" value="LDR"/>
<part name="R_LDR" library="AutoBath" deviceset="RES" device="" value="10k"/>
<part name="J_DHT" library="AutoBath" deviceset="HDR3" device="" value="DHT DNP"/>
<part name="R_DHT" library="AutoBath" deviceset="RES" device="" value="10k DNP"/>
<part name="J_LDRD" library="AutoBath" deviceset="HDR3" device="" value="LDR DIGITAL DNP"/>
<part name="J_UART" library="AutoBath" deviceset="HDR2" device="" value="UART TX GND"/>
</parts><sheets><sheet><description>AutoBath-Light-Fan controller</description><plain>
<text x="-30" y="75" size="3" layer="97">AutoBath-Light-Fan: PIR bathroom light + exhaust fan controller (GD32F103C8T6 Blue Pill)</text>
<text x="-30" y="70" size="2" layer="97">Nets joined by name labels. Mains parts need a clearance review before any fabrication. Footprints marked PLACEHOLDER are unverified.</text>
</plain><instances>
<instance part="U1" gate="G$1" x="0.00" y="0.00"/>
<instance part="J_AC" gate="G$1" x="110.00" y="55.00"/>
<instance part="F1" gate="G$1" x="205.00" y="55.00"/>
<instance part="RV1" gate="G$1" x="300.00" y="55.00"/>
<instance part="PS1" gate="G$1" x="395.00" y="55.00"/>
<instance part="F2" gate="G$1" x="110.00" y="25.00"/>
<instance part="K1" gate="G$1" x="205.00" y="25.00"/>
<instance part="K2" gate="G$1" x="300.00" y="25.00"/>
<instance part="J_LIGHT" gate="G$1" x="395.00" y="25.00"/>
<instance part="J_FAN" gate="G$1" x="110.00" y="-5.00"/>
<instance part="Q1" gate="G$1" x="205.00" y="-5.00"/>
<instance part="Q2" gate="G$1" x="300.00" y="-5.00"/>
<instance part="R_B1" gate="G$1" x="395.00" y="-5.00"/>
<instance part="R_B2" gate="G$1" x="110.00" y="-35.00"/>
<instance part="R_PD1" gate="G$1" x="205.00" y="-35.00"/>
<instance part="R_PD2" gate="G$1" x="300.00" y="-35.00"/>
<instance part="D1" gate="G$1" x="395.00" y="-35.00"/>
<instance part="D2" gate="G$1" x="110.00" y="-65.00"/>
<instance part="D3" gate="G$1" x="205.00" y="-65.00"/>
<instance part="C1" gate="G$1" x="300.00" y="-65.00"/>
<instance part="C2" gate="G$1" x="395.00" y="-65.00"/>
<instance part="J_PIR" gate="G$1" x="110.00" y="-95.00"/>
<instance part="R_PIR_A" gate="G$1" x="205.00" y="-95.00"/>
<instance part="R_PIR_B" gate="G$1" x="300.00" y="-95.00"/>
<instance part="R_L1" gate="G$1" x="395.00" y="-95.00"/>
<instance part="R_L2" gate="G$1" x="110.00" y="-125.00"/>
<instance part="R_L3" gate="G$1" x="205.00" y="-125.00"/>
<instance part="R_L4" gate="G$1" x="300.00" y="-125.00"/>
<instance part="R_L5" gate="G$1" x="395.00" y="-125.00"/>
<instance part="R_L6" gate="G$1" x="110.00" y="-155.00"/>
<instance part="J_LED1" gate="G$1" x="205.00" y="-155.00"/>
<instance part="J_LED2" gate="G$1" x="300.00" y="-155.00"/>
<instance part="J_LED3" gate="G$1" x="395.00" y="-155.00"/>
<instance part="J_LED4" gate="G$1" x="110.00" y="-185.00"/>
<instance part="J_LED5" gate="G$1" x="205.00" y="-185.00"/>
<instance part="J_LED6" gate="G$1" x="300.00" y="-185.00"/>
<instance part="J_BTN1" gate="G$1" x="395.00" y="-185.00"/>
<instance part="J_BTN2" gate="G$1" x="110.00" y="-215.00"/>
<instance part="J_LDR" gate="G$1" x="205.00" y="-215.00"/>
<instance part="R_LDR" gate="G$1" x="300.00" y="-215.00"/>
<instance part="J_DHT" gate="G$1" x="395.00" y="-215.00"/>
<instance part="R_DHT" gate="G$1" x="110.00" y="-245.00"/>
<instance part="J_LDRD" gate="G$1" x="205.00" y="-245.00"/>
<instance part="J_UART" gate="G$1" x="300.00" y="-245.00"/>
</instances><busses/><nets>
<net name="L_IN" class="0">
<segment><pinref part="J_AC" gate="G$1" pin="1"/>
<wire x1="101.11" y1="57.54" x2="91.91" y2="57.54" width="0.1524" layer="91"/>
<label x="92.91" y="57.54" size="1.27" layer="95"/></segment>
<segment><pinref part="F1" gate="G$1" pin="1"/>
<wire x1="196.11" y1="55.00" x2="186.91" y2="55.00" width="0.1524" layer="91"/>
<label x="187.91" y="55.00" size="1.27" layer="95"/></segment>
<segment><pinref part="F2" gate="G$1" pin="1"/>
<wire x1="101.11" y1="25.00" x2="91.91" y2="25.00" width="0.1524" layer="91"/>
<label x="92.91" y="25.00" size="1.27" layer="95"/></segment>
</net>
<net name="N" class="0">
<segment><pinref part="J_AC" gate="G$1" pin="2"/>
<wire x1="101.11" y1="55.00" x2="95.81" y2="55.00" width="0.1524" layer="91"/>
<label x="96.81" y="55.00" size="1.27" layer="95"/></segment>
<segment><pinref part="PS1" gate="G$1" pin="ACN"/>
<wire x1="386.11" y1="53.73" x2="380.81" y2="53.73" width="0.1524" layer="91"/>
<label x="381.81" y="53.73" size="1.27" layer="95"/></segment>
<segment><pinref part="RV1" gate="G$1" pin="2"/>
<wire x1="308.89" y1="55.00" x2="314.19" y2="55.00" width="0.1524" layer="91"/>
<label x="309.89" y="55.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LIGHT" gate="G$1" pin="2"/>
<wire x1="386.11" y1="25.00" x2="380.81" y2="25.00" width="0.1524" layer="91"/>
<label x="381.81" y="25.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_FAN" gate="G$1" pin="2"/>
<wire x1="101.11" y1="-5.00" x2="95.81" y2="-5.00" width="0.1524" layer="91"/>
<label x="96.81" y="-5.00" size="1.27" layer="95"/></segment>
</net>
<net name="PE" class="0">
<segment><pinref part="J_AC" gate="G$1" pin="3"/>
<wire x1="101.11" y1="52.46" x2="94.51" y2="52.46" width="0.1524" layer="91"/>
<label x="95.51" y="52.46" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LIGHT" gate="G$1" pin="3"/>
<wire x1="386.11" y1="22.46" x2="379.51" y2="22.46" width="0.1524" layer="91"/>
<label x="380.51" y="22.46" size="1.27" layer="95"/></segment>
<segment><pinref part="J_FAN" gate="G$1" pin="3"/>
<wire x1="101.11" y1="-7.54" x2="94.51" y2="-7.54" width="0.1524" layer="91"/>
<label x="95.51" y="-7.54" size="1.27" layer="95"/></segment>
</net>
<net name="L_FUSED_PS" class="0">
<segment><pinref part="F1" gate="G$1" pin="2"/>
<wire x1="213.89" y1="55.00" x2="230.89" y2="55.00" width="0.1524" layer="91"/>
<label x="214.89" y="55.00" size="1.27" layer="95"/></segment>
<segment><pinref part="PS1" gate="G$1" pin="ACL"/>
<wire x1="386.11" y1="56.27" x2="369.11" y2="56.27" width="0.1524" layer="91"/>
<label x="370.11" y="56.27" size="1.27" layer="95"/></segment>
<segment><pinref part="RV1" gate="G$1" pin="1"/>
<wire x1="291.11" y1="55.00" x2="274.11" y2="55.00" width="0.1524" layer="91"/>
<label x="275.11" y="55.00" size="1.27" layer="95"/></segment>
</net>
<net name="L_LOAD" class="0">
<segment><pinref part="F2" gate="G$1" pin="2"/>
<wire x1="118.89" y1="25.00" x2="130.69" y2="25.00" width="0.1524" layer="91"/>
<label x="119.89" y="25.00" size="1.27" layer="95"/></segment>
<segment><pinref part="K1" gate="G$1" pin="COM"/>
<wire x1="215.16" y1="27.54" x2="226.96" y2="27.54" width="0.1524" layer="91"/>
<label x="216.16" y="27.54" size="1.27" layer="95"/></segment>
<segment><pinref part="K2" gate="G$1" pin="COM"/>
<wire x1="310.16" y1="27.54" x2="321.96" y2="27.54" width="0.1524" layer="91"/>
<label x="311.16" y="27.54" size="1.27" layer="95"/></segment>
</net>
<net name="LIGHT_SWITCHED" class="0">
<segment><pinref part="K1" gate="G$1" pin="NO"/>
<wire x1="215.16" y1="25.00" x2="237.36" y2="25.00" width="0.1524" layer="91"/>
<label x="216.16" y="25.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LIGHT" gate="G$1" pin="1"/>
<wire x1="386.11" y1="27.54" x2="363.91" y2="27.54" width="0.1524" layer="91"/>
<label x="364.91" y="27.54" size="1.27" layer="95"/></segment>
</net>
<net name="FAN_SWITCHED" class="0">
<segment><pinref part="K2" gate="G$1" pin="NO"/>
<wire x1="310.16" y1="25.00" x2="329.76" y2="25.00" width="0.1524" layer="91"/>
<label x="311.16" y="25.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_FAN" gate="G$1" pin="1"/>
<wire x1="101.11" y1="-2.46" x2="81.51" y2="-2.46" width="0.1524" layer="91"/>
<label x="82.51" y="-2.46" size="1.27" layer="95"/></segment>
</net>
<net name="+5V" class="0">
<segment><pinref part="PS1" gate="G$1" pin="VO_P"/>
<wire x1="403.89" y1="56.27" x2="411.79" y2="56.27" width="0.1524" layer="91"/>
<label x="404.89" y="56.27" size="1.27" layer="95"/></segment>
<segment><pinref part="D3" gate="G$1" pin="A"/>
<wire x1="196.11" y1="-65.00" x2="188.21" y2="-65.00" width="0.1524" layer="91"/>
<label x="189.21" y="-65.00" size="1.27" layer="95"/></segment>
<segment><pinref part="C1" gate="G$1" pin="+"/>
<wire x1="291.11" y1="-65.00" x2="283.21" y2="-65.00" width="0.1524" layer="91"/>
<label x="284.21" y="-65.00" size="1.27" layer="95"/></segment>
<segment><pinref part="C2" gate="G$1" pin="1"/>
<wire x1="386.11" y1="-65.00" x2="378.21" y2="-65.00" width="0.1524" layer="91"/>
<label x="379.21" y="-65.00" size="1.27" layer="95"/></segment>
<segment><pinref part="K1" gate="G$1" pin="COIL_P"/>
<wire x1="194.84" y1="27.54" x2="186.94" y2="27.54" width="0.1524" layer="91"/>
<label x="187.94" y="27.54" size="1.27" layer="95"/></segment>
<segment><pinref part="K2" gate="G$1" pin="COIL_P"/>
<wire x1="289.84" y1="27.54" x2="281.94" y2="27.54" width="0.1524" layer="91"/>
<label x="282.94" y="27.54" size="1.27" layer="95"/></segment>
<segment><pinref part="D1" gate="G$1" pin="K"/>
<wire x1="403.89" y1="-35.00" x2="411.79" y2="-35.00" width="0.1524" layer="91"/>
<label x="404.89" y="-35.00" size="1.27" layer="95"/></segment>
<segment><pinref part="D2" gate="G$1" pin="K"/>
<wire x1="118.89" y1="-65.00" x2="126.79" y2="-65.00" width="0.1524" layer="91"/>
<label x="119.89" y="-65.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_PIR" gate="G$1" pin="1"/>
<wire x1="101.11" y1="-92.46" x2="93.21" y2="-92.46" width="0.1524" layer="91"/>
<label x="94.21" y="-92.46" size="1.27" layer="95"/></segment>
</net>
<net name="5V_MCU" class="0">
<segment><pinref part="D3" gate="G$1" pin="K"/>
<wire x1="213.89" y1="-65.00" x2="225.69" y2="-65.00" width="0.1524" layer="91"/>
<label x="214.89" y="-65.00" size="1.27" layer="95"/></segment>
<segment><pinref part="U1" gate="G$1" pin="5V"/>
<wire x1="8.89" y1="19.05" x2="20.69" y2="19.05" width="0.1524" layer="91"/>
<label x="9.89" y="19.05" size="1.27" layer="95"/></segment>
</net>
<net name="GND" class="0">
<segment><pinref part="PS1" gate="G$1" pin="VO_N"/>
<wire x1="403.89" y1="53.73" x2="411.79" y2="53.73" width="0.1524" layer="91"/>
<label x="404.89" y="53.73" size="1.27" layer="95"/></segment>
<segment><pinref part="C1" gate="G$1" pin="-"/>
<wire x1="308.89" y1="-65.00" x2="316.79" y2="-65.00" width="0.1524" layer="91"/>
<label x="309.89" y="-65.00" size="1.27" layer="95"/></segment>
<segment><pinref part="C2" gate="G$1" pin="2"/>
<wire x1="403.89" y1="-65.00" x2="411.79" y2="-65.00" width="0.1524" layer="91"/>
<label x="404.89" y="-65.00" size="1.27" layer="95"/></segment>
<segment><pinref part="Q1" gate="G$1" pin="E"/>
<wire x1="213.89" y1="-6.27" x2="221.79" y2="-6.27" width="0.1524" layer="91"/>
<label x="214.89" y="-6.27" size="1.27" layer="95"/></segment>
<segment><pinref part="Q2" gate="G$1" pin="E"/>
<wire x1="308.89" y1="-6.27" x2="316.79" y2="-6.27" width="0.1524" layer="91"/>
<label x="309.89" y="-6.27" size="1.27" layer="95"/></segment>
<segment><pinref part="R_PD1" gate="G$1" pin="2"/>
<wire x1="213.89" y1="-35.00" x2="221.79" y2="-35.00" width="0.1524" layer="91"/>
<label x="214.89" y="-35.00" size="1.27" layer="95"/></segment>
<segment><pinref part="R_PD2" gate="G$1" pin="2"/>
<wire x1="308.89" y1="-35.00" x2="316.79" y2="-35.00" width="0.1524" layer="91"/>
<label x="309.89" y="-35.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_PIR" gate="G$1" pin="3"/>
<wire x1="101.11" y1="-97.54" x2="93.21" y2="-97.54" width="0.1524" layer="91"/>
<label x="94.21" y="-97.54" size="1.27" layer="95"/></segment>
<segment><pinref part="R_PIR_B" gate="G$1" pin="2"/>
<wire x1="308.89" y1="-95.00" x2="316.79" y2="-95.00" width="0.1524" layer="91"/>
<label x="309.89" y="-95.00" size="1.27" layer="95"/></segment>
<segment><pinref part="R_LDR" gate="G$1" pin="2"/>
<wire x1="308.89" y1="-215.00" x2="316.79" y2="-215.00" width="0.1524" layer="91"/>
<label x="309.89" y="-215.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_BTN1" gate="G$1" pin="2"/>
<wire x1="386.11" y1="-186.27" x2="378.21" y2="-186.27" width="0.1524" layer="91"/>
<label x="379.21" y="-186.27" size="1.27" layer="95"/></segment>
<segment><pinref part="J_BTN2" gate="G$1" pin="2"/>
<wire x1="101.11" y1="-216.27" x2="93.21" y2="-216.27" width="0.1524" layer="91"/>
<label x="94.21" y="-216.27" size="1.27" layer="95"/></segment>
<segment><pinref part="J_DHT" gate="G$1" pin="3"/>
<wire x1="386.11" y1="-217.54" x2="378.21" y2="-217.54" width="0.1524" layer="91"/>
<label x="379.21" y="-217.54" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LDRD" gate="G$1" pin="3"/>
<wire x1="196.11" y1="-247.54" x2="188.21" y2="-247.54" width="0.1524" layer="91"/>
<label x="189.21" y="-247.54" size="1.27" layer="95"/></segment>
<segment><pinref part="J_UART" gate="G$1" pin="2"/>
<wire x1="291.11" y1="-246.27" x2="283.21" y2="-246.27" width="0.1524" layer="91"/>
<label x="284.21" y="-246.27" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED1" gate="G$1" pin="2"/>
<wire x1="196.11" y1="-156.27" x2="188.21" y2="-156.27" width="0.1524" layer="91"/>
<label x="189.21" y="-156.27" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED2" gate="G$1" pin="2"/>
<wire x1="291.11" y1="-156.27" x2="283.21" y2="-156.27" width="0.1524" layer="91"/>
<label x="284.21" y="-156.27" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED3" gate="G$1" pin="2"/>
<wire x1="386.11" y1="-156.27" x2="378.21" y2="-156.27" width="0.1524" layer="91"/>
<label x="379.21" y="-156.27" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED4" gate="G$1" pin="2"/>
<wire x1="101.11" y1="-186.27" x2="93.21" y2="-186.27" width="0.1524" layer="91"/>
<label x="94.21" y="-186.27" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED5" gate="G$1" pin="2"/>
<wire x1="196.11" y1="-186.27" x2="188.21" y2="-186.27" width="0.1524" layer="91"/>
<label x="189.21" y="-186.27" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED6" gate="G$1" pin="2"/>
<wire x1="291.11" y1="-186.27" x2="283.21" y2="-186.27" width="0.1524" layer="91"/>
<label x="284.21" y="-186.27" size="1.27" layer="95"/></segment>
<segment><pinref part="U1" gate="G$1" pin="GND"/>
<wire x1="8.89" y1="21.59" x2="16.79" y2="21.59" width="0.1524" layer="91"/>
<label x="9.89" y="21.59" size="1.27" layer="95"/></segment>
<segment><pinref part="U1" gate="G$1" pin="GND​"/>
<wire x1="-8.89" y1="-24.13" x2="-16.79" y2="-24.13" width="0.1524" layer="91"/>
<label x="-15.79" y="-24.13" size="1.27" layer="95"/></segment>
<segment><pinref part="U1" gate="G$1" pin="GND​​"/>
<wire x1="-8.89" y1="-21.59" x2="-16.79" y2="-21.59" width="0.1524" layer="91"/>
<label x="-15.79" y="-21.59" size="1.27" layer="95"/></segment>
</net>
<net name="3V3" class="0">
<segment><pinref part="U1" gate="G$1" pin="3.3"/>
<wire x1="8.89" y1="24.13" x2="16.79" y2="24.13" width="0.1524" layer="91"/>
<label x="9.89" y="24.13" size="1.27" layer="95"/></segment>
<segment><pinref part="U1" gate="G$1" pin="3.3​"/>
<wire x1="-8.89" y1="-19.05" x2="-16.79" y2="-19.05" width="0.1524" layer="91"/>
<label x="-15.79" y="-19.05" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LDR" gate="G$1" pin="1"/>
<wire x1="196.11" y1="-213.73" x2="188.21" y2="-213.73" width="0.1524" layer="91"/>
<label x="189.21" y="-213.73" size="1.27" layer="95"/></segment>
<segment><pinref part="J_DHT" gate="G$1" pin="1"/>
<wire x1="386.11" y1="-212.46" x2="378.21" y2="-212.46" width="0.1524" layer="91"/>
<label x="379.21" y="-212.46" size="1.27" layer="95"/></segment>
<segment><pinref part="R_DHT" gate="G$1" pin="1"/>
<wire x1="101.11" y1="-245.00" x2="93.21" y2="-245.00" width="0.1524" layer="91"/>
<label x="94.21" y="-245.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LDRD" gate="G$1" pin="1"/>
<wire x1="196.11" y1="-242.46" x2="188.21" y2="-242.46" width="0.1524" layer="91"/>
<label x="189.21" y="-242.46" size="1.27" layer="95"/></segment>
</net>
<net name="RELAY1_DRV" class="0">
<segment><pinref part="U1" gate="G$1" pin="A1"/>
<wire x1="-8.89" y1="11.43" x2="-25.89" y2="11.43" width="0.1524" layer="91"/>
<label x="-24.89" y="11.43" size="1.27" layer="95"/></segment>
<segment><pinref part="R_B1" gate="G$1" pin="1"/>
<wire x1="386.11" y1="-5.00" x2="369.11" y2="-5.00" width="0.1524" layer="91"/>
<label x="370.11" y="-5.00" size="1.27" layer="95"/></segment>
</net>
<net name="RELAY2_DRV" class="0">
<segment><pinref part="U1" gate="G$1" pin="A2"/>
<wire x1="-8.89" y1="8.89" x2="-25.89" y2="8.89" width="0.1524" layer="91"/>
<label x="-24.89" y="8.89" size="1.27" layer="95"/></segment>
<segment><pinref part="R_B2" gate="G$1" pin="1"/>
<wire x1="101.11" y1="-35.00" x2="84.11" y2="-35.00" width="0.1524" layer="91"/>
<label x="85.11" y="-35.00" size="1.27" layer="95"/></segment>
</net>
<net name="Q1_BASE" class="0">
<segment><pinref part="R_B1" gate="G$1" pin="2"/>
<wire x1="403.89" y1="-5.00" x2="416.99" y2="-5.00" width="0.1524" layer="91"/>
<label x="404.89" y="-5.00" size="1.27" layer="95"/></segment>
<segment><pinref part="Q1" gate="G$1" pin="B"/>
<wire x1="196.11" y1="-3.73" x2="183.01" y2="-3.73" width="0.1524" layer="91"/>
<label x="184.01" y="-3.73" size="1.27" layer="95"/></segment>
<segment><pinref part="R_PD1" gate="G$1" pin="1"/>
<wire x1="196.11" y1="-35.00" x2="183.01" y2="-35.00" width="0.1524" layer="91"/>
<label x="184.01" y="-35.00" size="1.27" layer="95"/></segment>
</net>
<net name="Q2_BASE" class="0">
<segment><pinref part="R_B2" gate="G$1" pin="2"/>
<wire x1="118.89" y1="-35.00" x2="131.99" y2="-35.00" width="0.1524" layer="91"/>
<label x="119.89" y="-35.00" size="1.27" layer="95"/></segment>
<segment><pinref part="Q2" gate="G$1" pin="B"/>
<wire x1="291.11" y1="-3.73" x2="278.01" y2="-3.73" width="0.1524" layer="91"/>
<label x="279.01" y="-3.73" size="1.27" layer="95"/></segment>
<segment><pinref part="R_PD2" gate="G$1" pin="1"/>
<wire x1="291.11" y1="-35.00" x2="278.01" y2="-35.00" width="0.1524" layer="91"/>
<label x="279.01" y="-35.00" size="1.27" layer="95"/></segment>
</net>
<net name="K1_COIL_LOW" class="0">
<segment><pinref part="K1" gate="G$1" pin="COIL_N"/>
<wire x1="194.84" y1="25.00" x2="176.54" y2="25.00" width="0.1524" layer="91"/>
<label x="177.54" y="25.00" size="1.27" layer="95"/></segment>
<segment><pinref part="Q1" gate="G$1" pin="C"/>
<wire x1="213.89" y1="-3.73" x2="232.19" y2="-3.73" width="0.1524" layer="91"/>
<label x="214.89" y="-3.73" size="1.27" layer="95"/></segment>
<segment><pinref part="D1" gate="G$1" pin="A"/>
<wire x1="386.11" y1="-35.00" x2="367.81" y2="-35.00" width="0.1524" layer="91"/>
<label x="368.81" y="-35.00" size="1.27" layer="95"/></segment>
</net>
<net name="K2_COIL_LOW" class="0">
<segment><pinref part="K2" gate="G$1" pin="COIL_N"/>
<wire x1="289.84" y1="25.00" x2="271.54" y2="25.00" width="0.1524" layer="91"/>
<label x="272.54" y="25.00" size="1.27" layer="95"/></segment>
<segment><pinref part="Q2" gate="G$1" pin="C"/>
<wire x1="308.89" y1="-3.73" x2="327.19" y2="-3.73" width="0.1524" layer="91"/>
<label x="309.89" y="-3.73" size="1.27" layer="95"/></segment>
<segment><pinref part="D2" gate="G$1" pin="A"/>
<wire x1="101.11" y1="-65.00" x2="82.81" y2="-65.00" width="0.1524" layer="91"/>
<label x="83.81" y="-65.00" size="1.27" layer="95"/></segment>
</net>
<net name="PIR_RAW" class="0">
<segment><pinref part="J_PIR" gate="G$1" pin="2"/>
<wire x1="101.11" y1="-95.00" x2="88.01" y2="-95.00" width="0.1524" layer="91"/>
<label x="89.01" y="-95.00" size="1.27" layer="95"/></segment>
<segment><pinref part="R_PIR_A" gate="G$1" pin="1"/>
<wire x1="196.11" y1="-95.00" x2="183.01" y2="-95.00" width="0.1524" layer="91"/>
<label x="184.01" y="-95.00" size="1.27" layer="95"/></segment>
</net>
<net name="PIR_OUT" class="0">
<segment><pinref part="R_PIR_A" gate="G$1" pin="2"/>
<wire x1="213.89" y1="-95.00" x2="226.99" y2="-95.00" width="0.1524" layer="91"/>
<label x="214.89" y="-95.00" size="1.27" layer="95"/></segment>
<segment><pinref part="R_PIR_B" gate="G$1" pin="1"/>
<wire x1="291.11" y1="-95.00" x2="278.01" y2="-95.00" width="0.1524" layer="91"/>
<label x="279.01" y="-95.00" size="1.27" layer="95"/></segment>
<segment><pinref part="U1" gate="G$1" pin="A0"/>
<wire x1="-8.89" y1="13.97" x2="-21.99" y2="13.97" width="0.1524" layer="91"/>
<label x="-20.99" y="13.97" size="1.27" layer="95"/></segment>
</net>
<net name="LED1_DRV" class="0">
<segment><pinref part="U1" gate="G$1" pin="A3"/>
<wire x1="-8.89" y1="6.35" x2="-23.29" y2="6.35" width="0.1524" layer="91"/>
<label x="-22.29" y="6.35" size="1.27" layer="95"/></segment>
<segment><pinref part="R_L1" gate="G$1" pin="1"/>
<wire x1="386.11" y1="-95.00" x2="371.71" y2="-95.00" width="0.1524" layer="91"/>
<label x="372.71" y="-95.00" size="1.27" layer="95"/></segment>
</net>
<net name="LED1_ANODE" class="0">
<segment><pinref part="R_L1" gate="G$1" pin="2"/>
<wire x1="403.89" y1="-95.00" x2="420.89" y2="-95.00" width="0.1524" layer="91"/>
<label x="404.89" y="-95.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED1" gate="G$1" pin="1"/>
<wire x1="196.11" y1="-153.73" x2="179.11" y2="-153.73" width="0.1524" layer="91"/>
<label x="180.11" y="-153.73" size="1.27" layer="95"/></segment>
</net>
<net name="LED2_DRV" class="0">
<segment><pinref part="U1" gate="G$1" pin="A4"/>
<wire x1="-8.89" y1="3.81" x2="-23.29" y2="3.81" width="0.1524" layer="91"/>
<label x="-22.29" y="3.81" size="1.27" layer="95"/></segment>
<segment><pinref part="R_L2" gate="G$1" pin="1"/>
<wire x1="101.11" y1="-125.00" x2="86.71" y2="-125.00" width="0.1524" layer="91"/>
<label x="87.71" y="-125.00" size="1.27" layer="95"/></segment>
</net>
<net name="LED2_ANODE" class="0">
<segment><pinref part="R_L2" gate="G$1" pin="2"/>
<wire x1="118.89" y1="-125.00" x2="135.89" y2="-125.00" width="0.1524" layer="91"/>
<label x="119.89" y="-125.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED2" gate="G$1" pin="1"/>
<wire x1="291.11" y1="-153.73" x2="274.11" y2="-153.73" width="0.1524" layer="91"/>
<label x="275.11" y="-153.73" size="1.27" layer="95"/></segment>
</net>
<net name="LED3_DRV" class="0">
<segment><pinref part="U1" gate="G$1" pin="A5"/>
<wire x1="-8.89" y1="1.27" x2="-23.29" y2="1.27" width="0.1524" layer="91"/>
<label x="-22.29" y="1.27" size="1.27" layer="95"/></segment>
<segment><pinref part="R_L3" gate="G$1" pin="1"/>
<wire x1="196.11" y1="-125.00" x2="181.71" y2="-125.00" width="0.1524" layer="91"/>
<label x="182.71" y="-125.00" size="1.27" layer="95"/></segment>
</net>
<net name="LED3_ANODE" class="0">
<segment><pinref part="R_L3" gate="G$1" pin="2"/>
<wire x1="213.89" y1="-125.00" x2="230.89" y2="-125.00" width="0.1524" layer="91"/>
<label x="214.89" y="-125.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED3" gate="G$1" pin="1"/>
<wire x1="386.11" y1="-153.73" x2="369.11" y2="-153.73" width="0.1524" layer="91"/>
<label x="370.11" y="-153.73" size="1.27" layer="95"/></segment>
</net>
<net name="LED4_DRV" class="0">
<segment><pinref part="U1" gate="G$1" pin="A6"/>
<wire x1="-8.89" y1="-1.27" x2="-23.29" y2="-1.27" width="0.1524" layer="91"/>
<label x="-22.29" y="-1.27" size="1.27" layer="95"/></segment>
<segment><pinref part="R_L4" gate="G$1" pin="1"/>
<wire x1="291.11" y1="-125.00" x2="276.71" y2="-125.00" width="0.1524" layer="91"/>
<label x="277.71" y="-125.00" size="1.27" layer="95"/></segment>
</net>
<net name="LED4_ANODE" class="0">
<segment><pinref part="R_L4" gate="G$1" pin="2"/>
<wire x1="308.89" y1="-125.00" x2="325.89" y2="-125.00" width="0.1524" layer="91"/>
<label x="309.89" y="-125.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED4" gate="G$1" pin="1"/>
<wire x1="101.11" y1="-183.73" x2="84.11" y2="-183.73" width="0.1524" layer="91"/>
<label x="85.11" y="-183.73" size="1.27" layer="95"/></segment>
</net>
<net name="LED5_DRV" class="0">
<segment><pinref part="U1" gate="G$1" pin="B11"/>
<wire x1="-8.89" y1="-13.97" x2="-23.29" y2="-13.97" width="0.1524" layer="91"/>
<label x="-22.29" y="-13.97" size="1.27" layer="95"/></segment>
<segment><pinref part="R_L5" gate="G$1" pin="1"/>
<wire x1="386.11" y1="-125.00" x2="371.71" y2="-125.00" width="0.1524" layer="91"/>
<label x="372.71" y="-125.00" size="1.27" layer="95"/></segment>
</net>
<net name="LED5_ANODE" class="0">
<segment><pinref part="R_L5" gate="G$1" pin="2"/>
<wire x1="403.89" y1="-125.00" x2="420.89" y2="-125.00" width="0.1524" layer="91"/>
<label x="404.89" y="-125.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED5" gate="G$1" pin="1"/>
<wire x1="196.11" y1="-183.73" x2="179.11" y2="-183.73" width="0.1524" layer="91"/>
<label x="180.11" y="-183.73" size="1.27" layer="95"/></segment>
</net>
<net name="LED6_DRV" class="0">
<segment><pinref part="U1" gate="G$1" pin="B13"/>
<wire x1="8.89" y1="-21.59" x2="23.29" y2="-21.59" width="0.1524" layer="91"/>
<label x="9.89" y="-21.59" size="1.27" layer="95"/></segment>
<segment><pinref part="R_L6" gate="G$1" pin="1"/>
<wire x1="101.11" y1="-155.00" x2="86.71" y2="-155.00" width="0.1524" layer="91"/>
<label x="87.71" y="-155.00" size="1.27" layer="95"/></segment>
</net>
<net name="LED6_ANODE" class="0">
<segment><pinref part="R_L6" gate="G$1" pin="2"/>
<wire x1="118.89" y1="-155.00" x2="135.89" y2="-155.00" width="0.1524" layer="91"/>
<label x="119.89" y="-155.00" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LED6" gate="G$1" pin="1"/>
<wire x1="291.11" y1="-183.73" x2="274.11" y2="-183.73" width="0.1524" layer="91"/>
<label x="275.11" y="-183.73" size="1.27" layer="95"/></segment>
</net>
<net name="BTN_LIGHT" class="0">
<segment><pinref part="U1" gate="G$1" pin="B0"/>
<wire x1="-8.89" y1="-6.35" x2="-24.59" y2="-6.35" width="0.1524" layer="91"/>
<label x="-23.59" y="-6.35" size="1.27" layer="95"/></segment>
<segment><pinref part="J_BTN1" gate="G$1" pin="1"/>
<wire x1="386.11" y1="-183.73" x2="370.41" y2="-183.73" width="0.1524" layer="91"/>
<label x="371.41" y="-183.73" size="1.27" layer="95"/></segment>
</net>
<net name="BTN_FAN" class="0">
<segment><pinref part="U1" gate="G$1" pin="B1"/>
<wire x1="-8.89" y1="-8.89" x2="-21.99" y2="-8.89" width="0.1524" layer="91"/>
<label x="-20.99" y="-8.89" size="1.27" layer="95"/></segment>
<segment><pinref part="J_BTN2" gate="G$1" pin="1"/>
<wire x1="101.11" y1="-213.73" x2="88.01" y2="-213.73" width="0.1524" layer="91"/>
<label x="89.01" y="-213.73" size="1.27" layer="95"/></segment>
</net>
<net name="LDR_ADC" class="0">
<segment><pinref part="U1" gate="G$1" pin="A7"/>
<wire x1="-8.89" y1="-3.81" x2="-21.99" y2="-3.81" width="0.1524" layer="91"/>
<label x="-20.99" y="-3.81" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LDR" gate="G$1" pin="2"/>
<wire x1="196.11" y1="-216.27" x2="183.01" y2="-216.27" width="0.1524" layer="91"/>
<label x="184.01" y="-216.27" size="1.27" layer="95"/></segment>
<segment><pinref part="R_LDR" gate="G$1" pin="1"/>
<wire x1="291.11" y1="-215.00" x2="278.01" y2="-215.00" width="0.1524" layer="91"/>
<label x="279.01" y="-215.00" size="1.27" layer="95"/></segment>
</net>
<net name="DHT_DATA" class="0">
<segment><pinref part="U1" gate="G$1" pin="B12"/>
<wire x1="8.89" y1="-24.13" x2="23.29" y2="-24.13" width="0.1524" layer="91"/>
<label x="9.89" y="-24.13" size="1.27" layer="95"/></segment>
<segment><pinref part="J_DHT" gate="G$1" pin="2"/>
<wire x1="386.11" y1="-215.00" x2="371.71" y2="-215.00" width="0.1524" layer="91"/>
<label x="372.71" y="-215.00" size="1.27" layer="95"/></segment>
<segment><pinref part="R_DHT" gate="G$1" pin="2"/>
<wire x1="118.89" y1="-245.00" x2="133.29" y2="-245.00" width="0.1524" layer="91"/>
<label x="119.89" y="-245.00" size="1.27" layer="95"/></segment>
</net>
<net name="LDR_DIGITAL" class="0">
<segment><pinref part="U1" gate="G$1" pin="B10"/>
<wire x1="-8.89" y1="-11.43" x2="-27.19" y2="-11.43" width="0.1524" layer="91"/>
<label x="-26.19" y="-11.43" size="1.27" layer="95"/></segment>
<segment><pinref part="J_LDRD" gate="G$1" pin="2"/>
<wire x1="196.11" y1="-245.00" x2="177.81" y2="-245.00" width="0.1524" layer="91"/>
<label x="178.81" y="-245.00" size="1.27" layer="95"/></segment>
</net>
<net name="UART_TX" class="0">
<segment><pinref part="U1" gate="G$1" pin="A9"/>
<wire x1="8.89" y1="-11.43" x2="21.99" y2="-11.43" width="0.1524" layer="91"/>
<label x="9.89" y="-11.43" size="1.27" layer="95"/></segment>
<segment><pinref part="J_UART" gate="G$1" pin="1"/>
<wire x1="291.11" y1="-243.73" x2="278.01" y2="-243.73" width="0.1524" layer="91"/>
<label x="279.01" y="-243.73" size="1.27" layer="95"/></segment>
</net>
</nets></sheet></sheets></schematic></drawing></eagle>
