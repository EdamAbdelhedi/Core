# MUX Files Overview

This document explains the 3 main source files used by the `MUX` firmware:

- `src/main.cpp`
- `src/mux_control.h`
- `src/spin_data_objects.h`

It also shows how they work together for the PWM test bench.

---

## Global Role

The `MUX` firmware controls the external multiplexers used to route one `CUT` signal to one oscilloscope channel.

The firmware must do 3 things:

1. store the software state of each route
2. apply that state to the GPIO pins
3. expose that state through ThingSet so the Python test scripts can control it

---

## File Roles

### `src/main.cpp`

This is the hardware control program.

It contains:

- the physical GPIO mapping for each MUX channel
- the `mux_routes` state table
- the `mux_apply()` function
- the startup routine that configures pins and applies default states

What this program really does:

- it knows which MCU pins are connected to the external mux hardware
- it converts a logical route like "Mux2 on Ch3 selects input 14" into GPIO levels
- it writes the select bits `S0..S3`
- it disables the other mux outputs on the same channel before enabling the selected one
- it applies the in-memory route state to the real hardware

What this program does not do:

- it does not parse ThingSet commands
- it does not know anything about serial communication
- it does not decide which route should be changed from the outside

So `main.cpp` is the execution layer.

If a route must become electrically active on the board, this file is the one that makes it happen.

In short:

- `main.cpp` knows the hardware
- `main.cpp` writes to GPIO
- `main.cpp` is the place where the routing really happens

### `src/mux_control.h`

This is the shared interface file between the programs.

It declares:

- `MuxRouteState`
- `mux_routes`
- `mux_apply()`

What this file really does:

- it defines the common data model used by the firmware
- it gives the same names and types to both `main.cpp` and `spin_data_objects.h`
- it prevents each file from inventing its own local version of the route state

Why it matters:

- `main.cpp` needs to know what a route looks like
- `spin_data_objects.h` also needs to know what a route looks like
- both must talk about the exact same `mux_routes` object

Without this file, the code would become tightly coupled and harder to maintain.

So `mux_control.h` is not a program that runs by itself.
It is a bridge file used at compile time so the two other source files can work together cleanly.

In short:

- `mux_control.h` is the contract
- it lets several files talk about the same routing state

### `src/spin_data_objects.h`

This is the ThingSet interface program layer.

It exposes the firmware to the outside world with paths like:

- `Mux/Mux1/Ch1/wChannel`
- `Mux/Mux1/Ch1/wEnable`
- `Mux/Mux1/Ch1/xApply`
- `Mux/Mux1/Ch1/rChannel`
- `Mux/Mux1/Ch1/rEnable`

What this program really does:

- it creates the public control tree seen from the PC side
- it connects each ThingSet object to one route entry in `mux_routes`
- it receives write operations such as `wChannel`, `wEnable`, and `xApply`
- it decides when a write should trigger a hardware update
- it calls `mux_apply()` after `xApply`

This means it is the translation layer between:

- external commands coming from Python scripts or a terminal
- internal firmware state used by the board

It does not directly control GPIO.

Instead, it:

- receives write requests from ThingSet
- updates `mux_routes`
- calls `mux_apply()` when `xApply` is requested

What it does not do:

- it does not know which physical STM32 pin is `PA0`, `PC7`, etc.
- it does not decide how select bits are encoded on `S0..S3`
- it does not drive the mux hardware directly

So `spin_data_objects.h` is the API layer of the firmware.

In short:

- `spin_data_objects.h` is the communication layer
- it translates ThingSet commands into firmware actions

---

## File Roles As Programs

Another way to see the 3 files is to treat them as 3 roles inside one firmware:

```text
main.cpp
    = hardware engine

mux_control.h
    = shared language / common contract

spin_data_objects.h
    = remote control interface
```

Or in sentence form:

- `main.cpp` executes the routing
- `mux_control.h` defines the shared objects needed for that routing
- `spin_data_objects.h` exposes that routing to the outside world

---

## Example Of Cooperation

Suppose the test script wants:

- channel `Ch2`
- mux `Mux3`
- input `8`
- enabled = `true`

Then each file plays a different role:

### `spin_data_objects.h`

It receives:

- `wChannel = 8`
- `wEnable = true`
- `xApply = true`

Then it updates the matching route state and calls `mux_apply()`.

### `mux_control.h`

It provides the shared route object definition so both files agree on:

- where the state is stored
- what `selected_channel` means
- what `enabled` means

### `main.cpp`

It takes that state and physically applies it:

- writes the 4-bit channel value to `S0..S3`
- disables other mux outputs for `Ch2`
- enables `Mux3` for `Ch2`

That is the full cooperation model of the firmware.

---

## Relationship Between Files

## Simple View

```text
Python test script
        |
        v
ThingSet path write
        |
        v
spin_data_objects.h
        |
        v
mux_control.h
        |
        v
main.cpp
        |
        v
GPIO pins -> external multiplexers -> oscilloscope routing
```

## Dependency View

```text
+----------------------+
| spin_data_objects.h  |
| ThingSet interface   |
+----------+-----------+
           |
           | uses declarations from
           v
+----------------------+
|    mux_control.h     |
| shared definitions   |
+----------+-----------+
           ^
           |
           | implemented by
           |
+----------+-----------+
|      main.cpp        |
| hardware behavior    |
+----------------------+
```

---

## Detailed Execution Flow

When a test script wants to route a signal, the flow is:

1. the Python script writes ThingSet values like `wChannel`, `wEnable`, and `xApply`
2. `spin_data_objects.h` receives the write callback
3. it updates the corresponding entry in `mux_routes`
4. it calls `mux_apply(mux_index, channel_index)`
5. `main.cpp` reads the route state
6. `main.cpp` updates select pins `S0..S3`
7. `main.cpp` disables the other mux outputs for that channel
8. `main.cpp` enables the selected mux output

### Flow Diagram

```text
Write /Mux/Mux2/Ch3/wChannel = 14
Write /Mux/Mux2/Ch3/wEnable  = true
Write /Mux/Mux2/Ch3/xApply   = true
             |
             v
spin_data_objects.h callback
             |
             v
mux_routes[1][2] updated
             |
             v
mux_apply(1, 2)
             |
             v
main.cpp configures:
- CH3 select pins
- disables all CH3 mux outputs
- enables MUX2 on CH3
```

---

## What Each File Knows

### `main.cpp` knows

- which GPIO pin is connected to `S0`, `S1`, `S2`, `S3`
- which GPIO pin enables `MUX1`, `MUX2`, `MUX3`
- how to convert a selected channel number into 4 select bits
- how to enforce "only one mux enabled at a time"

### `mux_control.h` knows

- what a route state looks like
- that a shared route table exists
- that `mux_apply()` exists

### `spin_data_objects.h` knows

- how the ThingSet tree is organized
- which ThingSet object maps to which route entry
- when to trigger `mux_apply()`

---

## Data Model

The firmware stores one route state per:

- mux: `Mux1`, `Mux2`, `Mux3`
- channel: `Ch1`, `Ch2`, `Ch3`, `Ch4`

So the shared state table is:

```text
mux_routes[mux_index][channel_index]
```

Where:

- `mux_index = 0` means `Mux1`
- `mux_index = 1` means `Mux2`
- `mux_index = 2` means `Mux3`
- `channel_index = 0` means `Ch1`
- `channel_index = 1` means `Ch2`
- `channel_index = 2` means `Ch3`
- `channel_index = 3` means `Ch4`

Each entry contains:

- `selected_channel`
- `enabled`

### State Table Schema

```text
                Ch1            Ch2            Ch3            Ch4
Mux1    mux_routes[0][0] mux_routes[0][1] mux_routes[0][2] mux_routes[0][3]
Mux2    mux_routes[1][0] mux_routes[1][1] mux_routes[1][2] mux_routes[1][3]
Mux3    mux_routes[2][0] mux_routes[2][1] mux_routes[2][2] mux_routes[2][3]
```

---

## Why This Split Is Useful

This separation makes the code easier to maintain:

- `main.cpp` can change if hardware wiring changes
- `spin_data_objects.h` can change if the ThingSet API changes
- `mux_control.h` stays small and stable as the shared interface

That means:

- hardware logic stays isolated
- communication logic stays isolated
- shared definitions stay simple

---

## Practical Reading Order

If you want to understand the MUX firmware quickly, read in this order:

1. `src/mux_control.h`
2. `src/main.cpp`
3. `src/spin_data_objects.h`

Reason:

1. `mux_control.h` tells you the shared objects
2. `main.cpp` shows what the firmware really does
3. `spin_data_objects.h` shows how external commands reach that logic

---

## Summary

```text
mux_control.h
    defines the shared state and function declarations

main.cpp
    implements the hardware routing behavior

spin_data_objects.h
    exposes that behavior through ThingSet
```

And together:

```text
ThingSet command
   -> updates route state
   -> calls mux_apply()
   -> drives GPIO
   -> routes the CUT signal to the oscilloscope
```
