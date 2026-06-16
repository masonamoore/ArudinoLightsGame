# Arduino Light Recorder Game

An Arduino Mega 2560 keypad game that lets you play four LEDs, record a light pattern, and save/play up to four songs from keypad slots `A` through `D`.

## Game Controls

| Key | Action |
| --- | --- |
| `1` | Flash LED 1 |
| `2` | Flash LED 2 |
| `3` | Flash LED 3 |
| `4` | Flash LED 4 |
| `5` | Start recording into the currently selected slot |
| `6` | Stop recording and save the slot |
| `A` | Select/play slot A |
| `B` | Select/play slot B |
| `C` | Select/play slot C |
| `D` | Select/play slot D |

The Arduino sketch stores each saved pattern as a fixed-size linked list inside a note pool. That keeps the sequence easy to traverse while avoiding heap allocation on the board.

## Hardware

- Arduino Mega 2560
- 4x4 membrane keypad
- 4 LEDs
- 4 220 ohm resistors
- Passive buzzer, optional but supported
- Breadboard and jumper wires

Wire the board from [docs/wiring.md](docs/wiring.md).

## Run The Arduino Code

Install the Arduino CLI and the keypad library:

```sh
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli lib install Keypad
```

Compile the sketch for an Arduino Mega:

```sh
arduino-cli compile --fqbn arduino:avr:mega LightRecorderGame
```

Upload it to the board. Replace `/dev/cu.usbmodemXXXX` with the port from `arduino-cli board list`.

```sh
arduino-cli board list
arduino-cli upload -p /dev/cu.usbmodemXXXX --fqbn arduino:avr:mega LightRecorderGame
```

Open the Serial Monitor at 9600 baud:

```sh
arduino-cli monitor -p /dev/cu.usbmodemXXXX --config baudrate=9600
```

## Try The UI Simulator

Open [simulator/index.html](simulator/index.html) in a browser. It mirrors the keypad controls, shows what each key does, and lights the matching LED on button press.

## Current Assumptions

- Board: Arduino Mega 2560.
- Keypad: standard 4x4 membrane keypad with rows on pins `22`, `24`, `26`, `28` and columns on pins `30`, `32`, `34`, `36`.
- LEDs: pins `2`, `3`, `4`, and `5`.
- Buzzer: pin `9`.
- Songs are held in RAM, so they reset when the Arduino loses power.
