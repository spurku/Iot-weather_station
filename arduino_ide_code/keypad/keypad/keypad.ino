#include <Keypad.h>

const byte ROWS = 4; // four rows connected
const byte COLS = 1; // only one column connected

char keys[ROWS][COLS] = {
  {'A'},
  {'B'},
  {'C'},
  {'D'}
};

byte rowPins[ROWS] = {A1, A2, A3, A4}; // Pins 1-4 (Row 1 to Row 4)
byte colPins[COLS] = {A5};            // Pin 5 (Column 1)

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
  }
}
