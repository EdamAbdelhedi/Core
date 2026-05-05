# MUX Firmware Project

This folder is the `MUX` firmware project for a second SPIN card used to route CUT signals to the oscilloscope through external multiplexers.

Use VS Code with PlatformIO to build and upload this project.

## Main files

- `src/main.cpp`: GPIO-based multiplexer routing firmware for the MUX card
- `src/spin_data_objects.h`: ThingSet data objects exposed by the firmware
- `platformio.ini`: board, shield, libraries, and upload configuration

## Current ThingSet control

`MUX` exposes the routing tree used by the PWM tests:

- `Mux/Mux1/Ch1` to `Mux/Mux1/Ch4`
- `Mux/Mux2/Ch1` to `Mux/Mux2/Ch4`
- `Mux/Mux3/Ch1` to `Mux/Mux3/Ch4`

Each route exposes:

- `wChannel`
- `wEnable`
- `xApply`
- `rChannel`
- `rEnable`

`MUX` does not automatically know which signal `CUT` selected. It is configured either:

- manually from a terminal such as Tera Term, or
- from the PC-side helper `tests/set_cut_mux_pwm.py`

Important hardware behavior:

- `MUX1`, `MUX2`, and `MUX3` share the same `S0..S3` address lines
- only one of the three muxes should be enabled at a time
- the firmware enforces this by disabling all muxes first, then enabling only the selected one

## Current synchronized mapping with CUT

For the present setup, the practical synchronized routes are:

- `Mux1/channel 8` -> `CUT PA8`
- `Mux1/channel 9` -> `CUT PA9`
- `Mux1/channel 10` -> `CUT PA10`
- `Mux2/channel 12` -> `CUT PB12`
- `Mux2/channel 13` -> `CUT PB13`
- `Mux2/channel 14` -> `CUT PB14`
- `Mux2/channel 15` -> `CUT PB15`
- `Mux3/channel 6` -> `CUT PC6`
- `Mux3/channel 7` -> `CUT PC7`
- `Mux3/channel 8` -> `CUT PC8`
- `Mux3/channel 9` -> `CUT PC9`

## What to edit first

To program the SPIN MUX card, start by editing `src/main.cpp`.

Current control-pin setup:

- shared address lines: `S0 -> PA0`, `S1 -> PA1`, `S2 -> PA2`, `S3 -> PA3`
- enable pins: `MUX1 -> PC0`, `MUX2 -> PC1`, `MUX3 -> PC2`

Update those pin numbers if your hardware changes.

If you need to change project configuration, update `platformio.ini` and the files in `src/` such as `app.ini`, `app.conf`, and `app.overlay`.
