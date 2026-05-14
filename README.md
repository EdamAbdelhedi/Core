# CUT Firmware Project

This folder contains the `CUT` firmware project for the SPIN board under test.

In the current setup, `CUT` is used to generate PWM signals that are later routed by the `MUX` board to the oscilloscope.

## Main files

- `src/main.cpp`: main firmware entry point
- `src/spin_data_objects.h`: ThingSet objects exposed by the firmware
- `src/cut_pwm_control.h`: helper used by the simplified CUT PWM selector
- `platformio.ini`: build and upload configuration

## Current PWM control

`CUT` exposes the PWM interfaces used by the current hardware tests:

- `Cut/PwmOut/wMux`
- `Cut/PwmOut/wPin`
- `Cut/PwmOut/wMux2`
- `Cut/PwmOut/wPin2`
- `Cut/PwmOut/wFreq_Hz`
- `Cut/PwmOut/wDuty`
- `Cut/PwmOut/wMod`
- `Cut/PwmOut/wEnable`
- `Cut/PwmOut/xApply`

Readbacks:

- `Cut/PwmOut/rMux`
- `Cut/PwmOut/rPin`
- `Cut/PwmOut/rMux2`
- `Cut/PwmOut/rPin2`
- `Cut/PwmOut/rOutput`
- `Cut/PwmOut/rOutput2`
- `Cut/PwmOut/rStatus`

For burst-mode tests, `CUT` also keeps a reduced `Spin/Pwm` interface with only:

- `Spin/Pwm/wTU`
- `Spin/Pwm/wAction`
- `Spin/Pwm/wBurstCmp`
- `Spin/Pwm/wBurstPer`
- `Spin/Pwm/xExec`

## Current synchronized mapping with MUX

For the present hardware setup, the simple selector is aligned with the MUX channel numbering and now uses the pair `wMux + wPin`:

- `wMux = 1`, `wPin = 8` -> `PA8` -> `PWM A main` -> `MUX1 channel 8`
- `wMux = 1`, `wPin = 9` -> `PA9` -> `PWM A complementary` -> `MUX1 channel 9`
- `wMux = 1`, `wPin = 10` -> `PA10` -> `PWM B main` -> `MUX1 channel 10`
- `wMux = 2`, `wPin = 12` -> `PB12` -> `PWM C main` -> `MUX2 channel 12`
- `wMux = 2`, `wPin = 13` -> `PB13` -> `PWM C complementary` -> `MUX2 channel 13`
- `wMux = 2`, `wPin = 14` -> `PB14` -> `PWM D main` -> `MUX2 channel 14`
- `wMux = 2`, `wPin = 15` -> `PB15` -> `PWM D complementary` -> `MUX2 channel 15`
- `wMux = 3`, `wPin = 6` -> `PC6` -> `PWM F main` -> `MUX3 channel 6`
- `wMux = 3`, `wPin = 7` -> `PC7` -> `PWM F complementary` -> `MUX3 channel 7`
- `wMux = 3`, `wPin = 8` -> `PC8` -> `PWM E main` -> `MUX3 channel 8`
- `wMux = 3`, `wPin = 9` -> `PC9` -> `PWM E complementary` -> `MUX3 channel 9`

## Development

Use VS Code with PlatformIO to build and upload this project.

If you want to control `CUT` and `MUX` together from one PC-side command, use:

- `tests/set_cut_mux_pwm.py`
