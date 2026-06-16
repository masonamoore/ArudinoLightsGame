const keypadLayout = [
  "1", "2", "3", "A",
  "4", "5", "6", "B",
  "7", "8", "9", "C",
  "*", "0", "#", "D"
];

const maxNotesPerSlot = 64;
const noteDurationMs = 220;
const slots = {
  A: [],
  B: [],
  C: [],
  D: []
};

let selectedSlot = "A";
let recording = false;
let lastRecordAt = 0;

const keypad = document.querySelector("#keypad");
const status = document.querySelector("#status");
const lights = [...document.querySelectorAll(".led")];
const songSlots = document.querySelector("#songSlots");

function renderKeypad() {
  keypad.innerHTML = "";
  keypadLayout.forEach((label) => {
    const button = document.createElement("button");
    button.type = "button";
    button.className = "key";
    button.textContent = label;
    button.setAttribute("aria-label", `Key ${label}`);

    if (label === "5" || label === "6") button.classList.add("record");
    if (["A", "B", "C", "D"].includes(label)) button.classList.add("slot");

    button.addEventListener("click", () => pressKey(label, button));
    keypad.appendChild(button);
  });
}

function renderSlots() {
  songSlots.innerHTML = "";
  Object.entries(slots).forEach(([slot, notes]) => {
    const meter = document.createElement("div");
    meter.className = `slot-meter${slot === selectedSlot ? " selected" : ""}`;
    meter.innerHTML = `<strong>Slot ${slot}</strong><span>${notes.length} / ${maxNotesPerSlot} notes</span>`;
    songSlots.appendChild(meter);
  });
}

function pressKey(label, button) {
  button.classList.add("active");
  setTimeout(() => button.classList.remove("active"), 120);

  if (["1", "2", "3", "4"].includes(label)) {
    const ledIndex = Number(label) - 1;
    flashLight(ledIndex);

    if (recording && slots[selectedSlot].length < maxNotesPerSlot) {
      const now = performance.now();
      slots[selectedSlot].push({
        ledIndex,
        waitBeforeMs: Math.max(0, Math.round(now - lastRecordAt))
      });
      lastRecordAt = now;
      setStatus(`Recording slot ${selectedSlot}: ${slots[selectedSlot].length} notes`, true);
      renderSlots();
    }
    return;
  }

  if (label === "5") {
    slots[selectedSlot] = [];
    recording = true;
    lastRecordAt = performance.now();
    setStatus(`Recording slot ${selectedSlot}`, true);
    renderSlots();
    return;
  }

  if (label === "6") {
    recording = false;
    setStatus(`Saved slot ${selectedSlot}: ${slots[selectedSlot].length} notes`, false);
    renderSlots();
    return;
  }

  if (["A", "B", "C", "D"].includes(label)) {
    if (recording) {
      setStatus("Press 6 before changing slots", true);
      return;
    }

    selectedSlot = label;
    renderSlots();
    if (slots[selectedSlot].length > 0) {
      setStatus(`Playing slot ${selectedSlot}`, false);
      playSlot(selectedSlot);
    } else {
      setStatus(`Slot ${selectedSlot} ready`, false);
    }
  }
}

function setStatus(message, isRecording) {
  status.textContent = message;
  status.classList.toggle("recording", isRecording);
}

function flashLight(index) {
  lights[index].classList.add("on");
  setTimeout(() => lights[index].classList.remove("on"), noteDurationMs);
}

async function playSlot(slot) {
  for (const note of slots[slot]) {
    await sleep(note.waitBeforeMs);
    flashLight(note.ledIndex);
    await sleep(noteDurationMs + 40);
  }
  setStatus(`Slot ${slot} ready`, false);
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

renderKeypad();
renderSlots();
