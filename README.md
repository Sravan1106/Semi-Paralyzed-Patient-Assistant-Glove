# Semi-Paralyzed-Patient-Assistant-Glove
A firmware developed for semi-paralyzed patients to help them communicate their day-to-day needs using simple finger gestures. It uses **flex sensors** embedded in a glove to detect different finger bend combinations, which are then mapped to **predefined voice messages** and **SMS alerts** sent via a GSM module.

## 📌 Features

- 🔧 Detects **8 unique hand gestures** using 3 flex sensors.
- 🗣 Plays **pre-recorded voice messages** using APR33A3 voice module.
- 📱 Sends **SMS notifications** to caretakers via GSM (SIM800A).
- 📺 Displays status messages on a 16x2 **I2C LCD**.
- 🤖 Built on **Arduino Mega 2560** using hardware Serial1 (no software serial needed).

---

## 📷 System Overview

| Gesture (Finger Bends) | LCD Message     | SMS Alert              |
|------------------------|------------------|-------------------------|
| 000 (None)             | All OK           | Patient is fine.        |
| 001 (R)                | Need Water       | Water needed.           |
| 010 (M)                | Need Food        | Food needed.            |
| 011 (M + R)            | Need Help Walk   | Help to walk.           |
| 100 (I)                | Feeling Cold     | Feeling cold.           |
| 101 (I + R)            | Need Blanket     | Need a blanket.         |
| 110 (I + M)            | Bathroom         | Bathroom request.       |
| 111 (All)              | EMERGENCY!       | EMERGENCY! Help needed! |

---

## 🔩 Hardware Required

- Arduino Mega 2560
- 3x Flex Sensors (Index, Middle, Ring)
- SIM800A GSM Module
- APR33A3 Voice Playback Module
- Speaker (for voice output)
- 16x2 LCD with I2C Interface
- Jumper wires, power supply, etc.

---
## 🔧 Setup Instructions

1. **Connect Flex Sensors** to analog pins A0 (Index), A1 (Middle), A2 (Ring).
2. **GSM SIM800A** connected to:
   - TX → Arduino Mega pin 19 (RX1)
   - RX → Arduino Mega pin 18 (TX1)
3. **APR33A3 Voice Module**: Connect 8 digital pins (7–13 and 6) to voice trigger inputs.
4. **LCD**: Connect via I2C (usually SDA = 20, SCL = 21 on Mega).
5. **Upload the Code** from `src/glove_assist.ino`.
6. Replace `+91XXXXXXXXXX` in the code with the **caretaker's phone number**.

---

## 🛠 Development Environment

- Platform: **Arduino Mega 2560**
- IDE: Arduino IDE or **VS Code** with Arduino extension
- Libraries used:
  - `LiquidCrystal_I2C`
  - Built-in `Serial` and `Serial1`

---

## 🧪 Calibration Tips

- Adjust the flex sensor threshold values in the code (`> 600`) to match real bending resistance.
- Use `Serial.print()` for testing sensor values live if needed.

---
## 👩‍💻 Author

**SRAVAN GINNARAPU P D**

---
---

## 📄 Code Explanation

Here’s a breakdown of how the code works:

### 🔹 Flex Sensor Input
```cpp
int valI = analogRead(flexIndex);
int valM = analogRead(flexMiddle);
int valR = analogRead(flexRing);

Reads analog values from the flex sensors.

Sensors change resistance when bent — mapped to voltage.```


🔹 Gesture Detection
cpp
Copy
Edit
bool bentI = valI > 600;
bool bentM = valM > 600;
bool bentR = valR > 600;
int gesture = (bentI << 2) | (bentM << 1) | bentR;
Converts finger bends into a binary number (e.g., 101 = 5).

Enables 8 gesture combinations.

🔹 LCD Display
cpp
Copy
Edit
lcd.setCursor(0, 0);
lcd.print(lcdMessages[gesture]);
Displays a readable message on the LCD for the gesture.

🔹 Voice Output
cpp
Copy
Edit
digitalWrite(voicePins[gesture], HIGH);
delay(500);
digitalWrite(voicePins[gesture], LOW);
Triggers one of the 8 pre-recorded voice outputs using the APR33A3 module.

🔹 SMS Notification
cpp
Copy
Edit
Serial1.println("AT+CMGF=1"); // Text mode
Serial1.println("AT+CMGS=\"+91XXXXXXXXXX\"");
Serial1.print(message);
Serial1.write(26); // CTRL+Z to send
Sends an SMS to the caretaker using the SIM800A GSM module.

Uses Serial1 since Arduino Mega has multiple hardware serial ports.

🔹 Anti-Repetition Logic
cpp
Copy
Edit
if (gesture != lastGesture) {
   // only act if gesture has changed
}
Avoids sending repeated messages for the same gesture.


