/*
  Project: Semi-Paralyzed Patient Assistance Glove
  Description: Detects 8 finger-bending gestures using flex sensors and triggers
               voice playback and SMS alerts to a caretaker.
  Board: Arduino Mega 2560
  GSM via Serial1 (TX1 = pin 18, RX1 = pin 19)
  Author: ALONA TREASA P D
*/

#include <LiquidCrystal_I2C.h>

// === Flex sensor analog pins ===
const int flexIndex  = A0;  // Index finger
const int flexMiddle = A1;  // Middle finger
const int flexRing   = A2;  // Ring finger

// === Voice trigger pins for APR33A3 ===
const int voicePins[8] = {7, 8, 9, 10, 11, 12, 13, 6}; // Assign 1 pin per gesture

// === LCD I2C configuration ===
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Check I2C address (0x27 or 0x3F)

// === Gesture messages ===
const char* lcdMessages[8] = {
  "All OK", "Need Water", "Need Food", "Need Help Walk",
  "Feeling Cold", "Need Blanket", "Bathroom", "EMERGENCY!"
};

const char* smsMessages[8] = {
  "Patient is fine.",
  "Water needed.",
  "Food needed.",
  "Help to walk.",
  "Feeling cold.",
  "Need a blanket.",
  "Bathroom request.",
  "EMERGENCY! Immediate help!"
};

// === Gesture tracking ===
int lastGesture = -1;  // Prevents repeated alerts for same gesture

void setup() {
  Serial.begin(9600);        // For debugging
  Serial1.begin(9600);       // For GSM SIM800A

  // Voice pins as output
  for (int i = 0; i < 8; i++) {
    pinMode(voicePins[i], OUTPUT);
    digitalWrite(voicePins[i], LOW);
  }

  // LCD init
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Glove System Init");
  delay(2000);
  lcd.clear();
}

void loop() {
  int valI = analogRead(flexIndex);
  int valM = analogRead(flexMiddle);
  int valR = analogRead(flexRing);

  // Adjust threshold as per calibration
  bool bentI = valI > 600;
  bool bentM = valM > 600;
  bool bentR = valR > 600;

  // Encode gesture into a binary index (0 to 7)
  int gesture = (bentI << 2) | (bentM << 1) | bentR;

  // Only respond to a new gesture
  if (gesture != lastGesture) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(lcdMessages[gesture]);

    triggerVoice(gesture);
    sendSMS(smsMessages[gesture]);

    lastGesture = gesture;
  }

  delay(1500); // Delay for debounce and stability
}

void triggerVoice(int gestureIndex) {
  digitalWrite(voicePins[gestureIndex], HIGH);
  delay(500);
  digitalWrite(voicePins[gestureIndex], LOW);
}

void sendSMS(const char* message) {
  Serial1.println("AT+CMGF=1");  // Text mode
  delay(1000);
  Serial1.println("AT+CMGS=\"+91XXXXXXXXXX\"");  // Replace with real number
  delay(1000);
  Serial1.print(message);
  Serial1.write(26); // ASCII code of CTRL+Z
  delay(5000);
}
