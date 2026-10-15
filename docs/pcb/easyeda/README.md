# EasyEDA Pro device export

`BluePill_GD32F103C8T6_device.elibz2` is the Blue Pill device exported from EasyEDA Pro (Library, Device, Personal, right click, Save as (Local), `.elibz2` V3 format). It holds the **device, the 40-pin symbol and the footprint `BLUEPILL_40P`** (pads 1 to 40, rows 15.24 mm apart).

The 3D model is only referenced by ID in this file, it is **not inside it**. To get the 3D view:
1. Import the file: File, Import, EasyEDA (Professional), pick the `.elibz2` (or use Library, Add, import).
2. Add a 3D model: New 3D Model, pick `../3d/BluePill_GD32F103C8T6_WITH_header_obj.zip` (or the WITHOUT one), Save.
3. Open footprint `BLUEPILL_40P`, 3D Model, pick it from the Personal list, Save.

Exported 2026-10-15 from V3.2.206. Not tested by importing into a second account yet. Measurements are from a photo, see [`../3d/README.md`](../3d/README.md).

Licence: CC BY 4.0, see [`../LICENSE.md`](../LICENSE.md).
