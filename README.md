# PicoBoyOS

**PicoBoyOS** is the firmware/software that powers **PicoBoy** - an **original device inspired by the Pip-Boy** from *Fallout*, built on the Raspberry Pi Pico.

PicoBoy combines a custom UI with a fully custom mechanical design: a **3D-printed enclosure** modeled in Fusion 360 (currently **v48**). v48 is the point where I’m testing a more “real product” layout: starting on a **custom PCB for power delivery** and making room for planned subsystems like a **custom Geiger counter**.

<img width="870" height="723" alt="image" src="https://github.com/user-attachments/assets/e5732d0d-5179-44d7-ac0d-f709ee3065ce" />


> Fan-made, Pip-Boy inspired. Not affiliated with Bethesda.

---

## What’s Working (Right Now)

- **Inventory-style UI**
  - Scroll through a list of items
  - Simple, readable presentation inspired by the Pip-Boy “vibe”
- **Rotary encoder navigation (current primary input)**
  - Rotate to navigate
  - Click to select
- **4.0" TFT display output (SPI)**
  - Touch hardware exists on the module, but **touch input is not implemented yet**

---

## Hardware Snapshot

### Display

Current display module:

- **Bewinner 4.0" TFT LCD Touch Screen Module**
- **480×320**, **SPI (14-pin)** interface
- **ILI9488** driver chip
- Includes **microSD card slot** and **stylus**
- Touch controller present, but not used (yet)

### Controls

Current controls are **fully powered by a rotary encoder**:

- **Rotate:** scroll / navigate
- **Click:** select / confirm

Planned:

- **Touchscreen navigation**
  - Will simplify selection + scrolling
  - Not wired or implemented in code yet

### Enclosure / Mechanical

- Designed in **Fusion 360**
- Iteration-heavy build process (currently **v48**)
- v48 focus:
  - Prototyping a **custom power delivery PCB**
  - Planning space/structure for add-ons (ex: **Geiger counter**)

---

## Software / Tooling

- Started in the **Arduino IDE**
- Migrated to **PlatformIO** to manage the project more cleanly

---

## Getting Started (Software)

### Clone

```bash
git clone https://github.com/mattordev/PicoBoyOS.git
cd PicoBoyOS
```

### Build & Upload (PlatformIO)

1. Install **VS Code** + the **PlatformIO** extension
2. Open this repo in VS Code
3. Build:
   ```bash
   pio run
   ```
4. Upload:
   ```bash
   pio run -t upload
   ```

---

## UI Navigation

**Current (rotary encoder):**
- Rotate encoder to move through items
- Press encoder button to select

**Planned (touch):**
- Tap-to-select
- Touch scrolling
- (Pending wiring + firmware implementation)

---

## Sharing / Reproducing the Build

A full **BOM + build guide** is planned, because I’d love to get PicoBoy into other peoples’ hands, but it’s **not ready yet** and I need to tweak a lot before this happens

If you’re following along early, expect changes to:
- enclosure revisions
- wiring/pin assignments
- display/driver setup
- PCB layout as it evolves

---

## Roadmap

- [ ] Touch input support (tap/scroll/select)
- [ ] Inventory improvements (categories, details view, sorting)
- [ ] Save/persistence for UI state + settings
- [ ] UI polish (assets, transitions, sound)
- [ ] Hardware expansion:
  - [ ] Power delivery PCB bring-up
  - [ ] Geiger counter module integration

---

## Dream Features

These are longer-term “wouldn’t it be sick if…” ideas. No promises, but this is the direction I *want* PicoBoyOS to grow.

- **World map**
  - A scrollable/zoomable map screen (local tiles or simplified vector-style)
  - Markers / points of interest
- **Stats screen**
  - Character-style overview page (health/energy/etc.)
  - “Vault terminal” style readouts
- **Quests / objectives**
  - Simple quest log with tracking
- **Radio / audio page**
  - UI for audio playback (if/when hardware supports it)
- **Data pages**
  - Device info, sensors, logs, “Pip-Boy-ish” diagnostics
- **Geiger counter integration**
  - Live CPM reading + history graph + alerts
- **MicroSD content**
  - Load assets/data/items from SD card (themes, inventories, maps, audio)
- **Themes / skins**
  - Green phosphor look, amber, custom palettes, fonts, UI chrome
- **Animations + sound**
  - Boot sequence, UI ticks/clicks, transitions
- **Companion / dev tools**
  - Config editor for inventories/content
  - USB serial or BLE sync (future)

---

## Contributing

If you’d like to contribute, PRs/issues are welcome - especially in areas like:
- UI screens & navigation structure
- input abstraction (encoder now, touch later)
- PlatformIO cleanup and documentation
- performance improvements for the ILI9488 SPI display

---

## License

This project is licensed under the **GNU Affero General Public License v3.0 (AGPL-3.0)**. See [`LICENSE`](LICENSE) for details.

---

## Credits

- Project/design/dev: **@mattordev**
- Inspiration: *Fallout* Pip-Boy aesthetic (fan-made)
