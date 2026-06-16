#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const byte LED_COUNT = 4;
const byte SLOT_COUNT = 4;
const byte MAX_NOTES_PER_SLOT = 64;

const byte ledPins[LED_COUNT] = {38, 40, 42, 44};
const int noteFrequencies[LED_COUNT] = {262, 330, 392, 523};
const byte buzzerPin = 46;

const unsigned int noteDurationMs = 220;
const unsigned int playbackPauseMs = 40;

struct NoteNode {
  byte ledIndex;
  unsigned int waitBeforeMs;
  unsigned int durationMs;
  int nextIndex;
};

NoteNode notePool[SLOT_COUNT][MAX_NOTES_PER_SLOT];
int songHead[SLOT_COUNT];
int songTail[SLOT_COUNT];
byte songSize[SLOT_COUNT];

byte selectedSlot = 0;
bool isRecording = false;
unsigned long lastRecordEventMs = 0;

void setup() {
  Serial.begin(9600);

  for (byte i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  pinMode(buzzerPin, OUTPUT);
  clearAllSongs();
  printWelcome();
}

void loop() {
  char key = keypad.getKey();
  if (!key) {
    return;
  }

  handleKey(key);
}

void handleKey(char key) {
  byte ledIndex;
  byte slotIndex;

  if (keyToLedIndex(key, ledIndex)) {
    if (isRecording) {
      recordNote(ledIndex);
    }

    playLight(ledIndex, noteDurationMs);
    return;
  }

  if (key == '5') {
    startRecording();
    return;
  }

  if (key == '6') {
    stopRecording();
    return;
  }

  if (keyToSlotIndex(key, slotIndex)) {
    selectSlot(slotIndex);
    return;
  }

  Serial.print(F("Unused key: "));
  Serial.println(key);
}

bool keyToLedIndex(char key, byte &ledIndex) {
  if (key >= '1' && key <= '4') {
    ledIndex = key - '1';
    return true;
  }
  return false;
}

bool keyToSlotIndex(char key, byte &slotIndex) {
  if (key >= 'A' && key <= 'D') {
    slotIndex = key - 'A';
    return true;
  }
  return false;
}

void startRecording() {
  clearSong(selectedSlot);
  isRecording = true;
  lastRecordEventMs = millis();

  Serial.print(F("Recording started in slot "));
  Serial.println(slotName(selectedSlot));
}

void stopRecording() {
  if (!isRecording) {
    Serial.println(F("Not recording. Press 5 to start."));
    return;
  }

  isRecording = false;
  Serial.print(F("Recording stopped. Saved "));
  Serial.print(songSize[selectedSlot]);
  Serial.print(F(" notes in slot "));
  Serial.println(slotName(selectedSlot));
}

void selectSlot(byte slotIndex) {
  if (isRecording) {
    Serial.println(F("Stop recording with 6 before changing slots."));
    return;
  }

  selectedSlot = slotIndex;
  Serial.print(F("Selected slot "));
  Serial.print(slotName(selectedSlot));

  if (songSize[selectedSlot] == 0) {
    Serial.println(F(". Slot is empty."));
    return;
  }

  Serial.println(F(". Playing saved song."));
  playSong(selectedSlot);
}

void recordNote(byte ledIndex) {
  if (songSize[selectedSlot] >= MAX_NOTES_PER_SLOT) {
    Serial.println(F("Recording full. Press 6 to save."));
    return;
  }

  unsigned long now = millis();
  unsigned int waitBefore = constrain(now - lastRecordEventMs, 0, 60000);
  lastRecordEventMs = now;

  byte newIndex = songSize[selectedSlot];
  notePool[selectedSlot][newIndex].ledIndex = ledIndex;
  notePool[selectedSlot][newIndex].waitBeforeMs = waitBefore;
  notePool[selectedSlot][newIndex].durationMs = noteDurationMs;
  notePool[selectedSlot][newIndex].nextIndex = -1;

  if (songHead[selectedSlot] == -1) {
    songHead[selectedSlot] = newIndex;
  } else {
    notePool[selectedSlot][songTail[selectedSlot]].nextIndex = newIndex;
  }

  songTail[selectedSlot] = newIndex;
  songSize[selectedSlot]++;

  Serial.print(F("Recorded light "));
  Serial.print(ledIndex + 1);
  Serial.print(F(" in slot "));
  Serial.println(slotName(selectedSlot));
}

void playSong(byte slotIndex) {
  int current = songHead[slotIndex];

  while (current != -1) {
    NoteNode note = notePool[slotIndex][current];
    delay(note.waitBeforeMs);
    playLight(note.ledIndex, note.durationMs);
    delay(playbackPauseMs);
    current = note.nextIndex;
  }
}

void playLight(byte ledIndex, unsigned int durationMs) {
  digitalWrite(ledPins[ledIndex], HIGH);
  tone(buzzerPin, noteFrequencies[ledIndex], durationMs);
  delay(durationMs);
  digitalWrite(ledPins[ledIndex], LOW);
  noTone(buzzerPin);
}

void clearAllSongs() {
  for (byte slot = 0; slot < SLOT_COUNT; slot++) {
    clearSong(slot);
  }
}

void clearSong(byte slotIndex) {
  songHead[slotIndex] = -1;
  songTail[slotIndex] = -1;
  songSize[slotIndex] = 0;

  for (byte i = 0; i < MAX_NOTES_PER_SLOT; i++) {
    notePool[slotIndex][i].nextIndex = -1;
  }
}

char slotName(byte slotIndex) {
  return 'A' + slotIndex;
}

void printWelcome() {
  Serial.println(F("Arduino Light Recorder Game"));
  Serial.println(F("1-4: play lights"));
  Serial.println(F("5: start recording selected slot"));
  Serial.println(F("6: stop recording"));
  Serial.println(F("A-D: select/play saved slots"));
  Serial.println(F("Default slot: A"));
}
