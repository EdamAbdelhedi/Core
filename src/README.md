# MUX Firmware

This folder contains the firmware used to control the external multiplexers that route CUT signals to the oscilloscope inputs.

## Role of the MUX firmware

The firmware drives three multiplexers for each routing group:

- `MUX1`
- `MUX2`
- `MUX3`

The routing groups are:

- `CH1`
- `CH2`
- `CH3`
- `CH4`

For each routing group (`CH1` to `CH4`), there are therefore three multiplexers:

- `MUX1`
- `MUX2`
- `MUX3`

For each routing group, the firmware:

- sets the shared select lines `S0..S3` to choose one channel from `0` to `15`
- enables only one multiplexer output at a time
- keeps the other multiplexer outputs disabled to avoid conflicts

The enable pins are active-low:

- `LOW` means enabled
- `HIGH` means disabled

At startup, the firmware configures all GPIOs, disables all mux outputs, and applies the default routing state.

## Control model

The routing state is handled with two values for each `MUXx / CHy` pair:

- `selected_channel`: the input channel to route
- `enabled`: whether this mux output is active

An apply function then pushes the software state to the GPIO pins.

Example:

- set `mux1_ch2_selected_channel`
- set `mux1_ch2_enabled`
- call `mux1_ch2_apply()`

## Files in this folder

### `main.cpp`

Main firmware implementation:

- defines the GPIO mapping for all mux select and enable pins
- configures the pins as outputs
- applies mux routing changes
- disables unused outputs
- starts the background status task

### `mux_control.h`

Public control interface for the mux state:

- declares the shared `mux_routes` table
- declares the generic `mux_apply()` function

This header is shared with the ThingSet object definitions.

### `spin_data_objects.h`

ThingSet integration for remote monitoring and control.

It exposes a tree of data objects such as:

- `Mux/Mux1/Ch1`
- `Mux/Mux2/Ch3`
- `Mux/Mux3/Ch4`

For each mux channel, it provides:

- `wChannel` to write the selected input
- `wEnable` to write the enable state
- `xApply` to request hardware update
- `rChannel` to read back the selected input
- `rEnable` to read back the enable state

## Build and runtime configuration files

### `app.conf`

Zephyr application configuration:

- enables ThingSet support
- enables CAN communication
- enables shell support
- reduces logging to save memory and flash

### `app.ini`

Project configuration file listing external dependencies used by the application.

### `app.overlay`

Device-tree overlay used to:

- configure USB CDC ACM interfaces for console and shell
- bind ThingSet communication to `fdcan2`

## Notes

- Channel values are masked to `4` bits, so the valid range is `0..15`.
- For one routing group, the three multiplexers share the same select lines, so only one mux should be enabled at a time.
