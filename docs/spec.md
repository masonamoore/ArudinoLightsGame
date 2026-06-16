# Light Recorder Game Spec

This is the working build spec for the Arduino light game. It documents the v1 behavior already implemented and the remaining decisions to confirm while the breadboard is being assembled.

## Implemented V1

- Board: Arduino Mega 2560.
- Input: 4x4 membrane keypad.
- Output: four LEDs and an optional passive buzzer.
- Keys `1`, `2`, `3`, and `4` each flash one matching LED.
- Key `5` starts recording into the selected save slot.
- Key `6` stops recording and saves the current sequence.
- Keys `A`, `B`, `C`, and `D` select and play saved slots.
- Save slots store up to 64 notes each.
- Songs are represented as a fixed-size linked list backed by an array, avoiding dynamic heap allocation on the Arduino.
- Songs use RAM storage for v1, so saved slots reset when the Arduino loses power.
- A browser simulator mirrors the keypad and LED behavior.
- Wiring is documented in both Markdown and SVG.

## Current Pin Decisions

| Function | Pins |
| --- | --- |
| LED 1-4 | `D2`, `D3`, `D4`, `D5` |
| Passive buzzer | `D9` |
| Keypad rows | `D22`, `D24`, `D26`, `D28` |
| Keypad columns | `D30`, `D32`, `D34`, `D36` |
| Ground | Arduino `GND` to breadboard ground rail |

## Confirmed Decisions

### 1. Should songs survive power loss?

Decision: keep RAM storage for v1.

| Option | Pros | Cons |
| --- | --- | --- |
| RAM storage | Simple, fast, low risk while testing keypad and LEDs | Songs disappear when the Arduino resets or loses power |
| EEPROM storage | Songs survive unplugging the Arduino | More code complexity and EEPROM has limited write cycles |

## Open Decisions

### 1. Should `A`-`D` play immediately or only select a slot?

Recommendation for first build: play immediately when the slot has notes.

| Option | Pros | Cons |
| --- | --- | --- |
| Select and play | Fewer buttons needed, fast to test saved patterns | You cannot select a filled slot without playing it |
| Select only | Cleaner mental model for recording into old slots | Needs another play key, likely `#` or `*` |

### 2. Is the buzzer required?

Recommendation for first build: keep it optional.

| Option | Pros | Cons |
| --- | --- | --- |
| Optional buzzer | Game works even if the kit buzzer is missing or annoying | Slightly less feedback |
| Required buzzer | More musical, easier to hear recording/playback | More wiring and one more part to debug |

### 3. What should unused keys do?

Recommendation for first build: leave `0`, `7`, `8`, `9`, `*`, and `#` unused.

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

## Next Confirmations

Answer these after the first breadboard test:

1. Do the keypad labels match the pressed keys in Serial Monitor?
2. Do LED 1-4 light in the intended physical order?
3. Should `A`-`D` play immediately or only select the slot?
4. Should the buzzer stay optional after the first hardware test?
