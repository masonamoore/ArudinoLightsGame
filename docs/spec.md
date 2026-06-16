# Light Recorder Game Spec

This is the working build spec for the Arduino light game. It documents the v1 behavior already implemented and the remaining decisions to confirm while the breadboard is being assembled.

## Implemented V1

- Board: Elegoo Mega 2560 controller board.
- Input: 4x4 membrane keypad.
- Output: four LEDs and an optional passive buzzer.
- Keys `1`, `2`, `3`, and `4` each flash one matching LED.
- Key `5` starts recording into the selected save slot.
- Key `6` stops recording and saves the current sequence.
- Keys `A`, `B`, `C`, and `D` select and play saved slots.
- The buzzer is optional for v1.
- Keys `0`, `7`, `8`, `9`, `*`, and `#` are intentionally unused for v1.
- Save slots store up to 64 notes each.
- Songs are represented as a fixed-size linked list backed by an array, avoiding dynamic heap allocation on the Arduino.
- Songs use RAM storage for v1, so saved slots reset when the Arduino loses power.
- A browser simulator mirrors the keypad and LED behavior.
- Wiring is documented in both Markdown and SVG.

## Current Pin Decisions

| Function | Pins |
| --- | --- |
| LED 1-4 | Mega pins `38`, `40`, `42`, `44` |
| Passive buzzer | Mega pin `46` |
| Keypad rows | Mega pins `22`, `24`, `26`, `28` |
| Keypad columns | Mega pins `30`, `32`, `34`, `36` |
| Charge rail | Mega `5V` to breadboard red `+` rail |
| Ground rail | Mega `GND` to breadboard blue `-` rail |

## Confirmed Decisions

### 1. Should songs survive power loss?

Decision: keep RAM storage for v1.

| Option | Pros | Cons |
| --- | --- | --- |
| RAM storage | Simple, fast, low risk while testing keypad and LEDs | Songs disappear when the Arduino resets or loses power |
| EEPROM storage | Songs survive unplugging the Arduino | More code complexity and EEPROM has limited write cycles |

### 2. Should `A`-`D` play immediately or only select a slot?

Decision: select and play immediately when the slot has notes.

| Option | Pros | Cons |
| --- | --- | --- |
| Select and play | Fewer buttons needed, fast to test saved patterns | You cannot select a filled slot without playing it |
| Select only | Cleaner mental model for recording into old slots | Needs another play key, likely `#` or `*` |

### 3. Is the buzzer required?

Decision: keep the buzzer optional for v1.

| Option | Pros | Cons |
| --- | --- | --- |
| Optional buzzer | Game works even if the kit buzzer is missing or annoying | Slightly less feedback |
| Required buzzer | More musical, easier to hear recording/playback | More wiring and one more part to debug |

### 4. What should unused keys do?

Decision: leave `0`, `7`, `8`, `9`, `*`, and `#` unused for v1.

| Option | Pros | Cons |
| --- | --- | --- |
| Leave unused | Less accidental behavior during first wiring test | Some keypad buttons do nothing |
| Add controls later | Could support clear, replay, tempo, or erase | More rules to learn before the core game works |

## Hardware Bring-Up Checklist

1. Upload the sketch.
2. Open Serial Monitor at `9600` baud.
3. Press `1`, `2`, `3`, and `4`; confirm the four LEDs flash in order.
4. Press `5`, then `1`, `2`, `3`, then `6`; confirm Serial Monitor says the notes were saved.
5. Press `A`; confirm the saved pattern replays.
6. If keypad buttons are scrambled, adjust `rowPins` and `colPins` in `LightRecorderGame.ino`.

## Hardware Test Confirmations

Answer these after the first breadboard test:

1. Do the keypad labels match the pressed keys in Serial Monitor?
2. Do LED 1-4 light in the intended physical order?
3. Does the optional buzzer make a tone when LEDs flash?
