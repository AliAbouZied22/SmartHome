# SmartHome


This repository contains Arduino code for an ESP32-based home automation project that integrates:

* **DHT11 Temperature Sensor** (reads and sends temperature to Blynk)
* **Ultrasonic Sensor (HC-SR04)** (automatically opens/closes a garage door)
* **Servo Motors** (controls main door, bathroom door, and garage door)
* **DC Motor** (controls garage door open/close via Blynk buttons)
* **Keypad & Buzzer** (allows local PIN-based unlocking of the main door)
* **Blynk Integration** (remote control of lights, doors, and live temperature monitoring)

Below is a simple breakdown of the code, followed by instructions on how to deploy it to your own ESP32 and set up a Blynk template.

---

## Table of Contents

1. [Features](#features)
2. [Hardware Requirements](#hardware-requirements)
3. [Software Requirements](#software-requirements)
4. [Wiring & Pin Assignment](#wiring--pin-assignment)
5. [Blynk Template Setup](#blynk-template-setup)
6. [Installing & Configuring the Code](#installing--configuring-the-code)
7. [Code Breakdown](#code-breakdown)
8. [Usage](#usage)
9. [Troubleshooting](#troubleshooting)

---

## Features

* **Temperature Monitoring**

  * Reads temperature from a DHT11 sensor every 2 seconds and sends it to Blynk (Virtual Pin V6).
* **Garage Door Automation**

  * Uses an HC-SR04 ultrasonic sensor to detect if a car or object is within 10 cm; if so, the garage servo opens for 3 seconds then closes automatically.
  * Also allows manual garage door control via two Blynk buttons (Virtual Pins V5 and V7).
* **Servo-Controlled Doors**

  * **Main Door**: Operated locally via a 4×4 keypad with a 4-digit PIN (default “2227”). If the PIN is correct, the main door servo opens briefly, beeps, then recloses.
  * **Bathroom Door**: Controlled remotely via a Blynk switch (Virtual Pin V1).
  * **Garage Door**: In addition to ultrasonic auto mode, the garage door can be triggered by Blynk buttons on V5/V7 to drive a DC motor for a brief interval.
* **DC Motor Control**

  * The DC motor (wired to pins 14 and 27) can be turned on in one direction or the other for 0.5 seconds to open/close a heavy garage door, triggered by Blynk buttons (V5/V7).
* **LED & Buzzer**

  * A main LED (pin 25) can be toggled on/off via a Blynk button (V4).
  * A buzzer (pin 2) beeps for feedback on keypad entry (short beep on success, long beep on failure).

---

## Hardware Requirements

* **Microcontroller**: ESP32 development board (e.g., Wemos Lolin32, NodeMCU-32S, etc.)
* **Sensors & Actuators**:

  * DHT11 Temperature & Humidity Sensor
  * HC-SR04 Ultrasonic Distance Sensor
  * Three SG90 (or similar) micro servos (main door, bathroom door, garage door)
  * One DC Motor with driver (e.g., L298N) or MOSFET circuit to drive two motor pins
  * 4×4 Matrix Keypad
  * Passive Buzzer
  * LED (any color)
* **Connections**:

  * Jumper wires, breadboard (or soldered connections), 5 V power supply (or USB power for ESP32 plus external supply for servos/motor).

---

## Software Requirements

1. **Arduino IDE (version 1.8.19 or later)**

   * Board Support:

     * Install “esp32” by Espressif in Boards Manager.
2. **Arduino Libraries** (via Sketch → Include Library → Manage Libraries):

   * **DHT sensor library** by Adafruit (for DHT11)
   * **Adafruit Unified Sensor** (dependency of DHT library)
   * **Blynk** (BlynkSimpleEsp32)
   * **ESP32Servo** (for controlling servos on ESP32)
   * **Keypad** (by Mark Stanley & Alexander Brevig, for the 4×4 keypad)

---

## Wiring & Pin Assignment

Below is the pin mapping as defined in `main.ino`:

| Component                     | ESP32 Pin      | Notes                                 |
| ----------------------------- | -------------- | ------------------------------------- |
| **DHT11 Data**                | 26             | `#define DHTPIN 26`                   |
| **Ultrasonic Trig (HC-SR04)** | 23             | `#define trigPin 23`                  |
| **Ultrasonic Echo (HC-SR04)** | 35             | `#define echoPin 35`                  |
| **Main Door Servo**           | 13             | `mainDoorServo.attach(13)`            |
| **Bathroom Door Servo**       | 12             | `bathroomServo.attach(12)`            |
| **Garage Door Servo**         | 33             | `garageServo.attach(33)`              |
| **DC Motor Pin 1**            | 14             | `motorPin1 = 14` (one direction)      |
| **DC Motor Pin 2**            | 27             | `motorPin2 = 27` (opposite direction) |
| **Main LED**                  | 25             | `mainLed = 25` (Blynk V4)             |
| **Buzzer**                    | 2              | `buzzerPin = 2`                       |
| **Keypad Rows**               | 19, 18, 5, 4   | Defined in `rowPins[]`                |
| **Keypad Columns**            | 32, 22, 21, 15 | Defined in `colPins[]`                |

> **Important**: Servos often draw more current than the ESP32’s 3.3 V pin can supply. It is recommended to power your servos from a **separate 5 V regulated source** and share a common ground with the ESP32.

---

## Blynk Template Setup

1. **Install Blynk App**

   * Download and install **Blynk IoT** (iOS or Android) on your smartphone.

2. **Create a New Template**

   * Open the Blynk IoT app (or visit the Blynk Console at [https://blynk.cloud](https://blynk.cloud)).
   * Click **“Create New Template”** (or “+ Template”).
   * Enter a **Template Name** (e.g., “Quickstart Template”) and optional description.
   * After creation, you will see:

     ```
     Template ID:   TMPL2FvpK0yPk  
     Template Name: Quickstart Template  
     Auth Token:    5hzqr_C0MUXV9zRzXed5-3OCLCNXPUPT
     ```
   * Copy these three values and paste them into your code, replacing the placeholders:

     ```cpp
     #define BLYNK_TEMPLATE_ID   "TMPL2FvpK0yPk"
     #define BLYNK_TEMPLATE_NAME "Quickstart Template"
     #define BLYNK_AUTH_TOKEN    "5hzqr_C0MUXV9zRzXed5-3OCLCNXPUPT"
     ```

3. **Add Widgets in the Blynk App**

   * Open the template’s **Dashboard** and click **“+ Add Widget”**.
   * Add the following widgets (set each to **Switch** or **Button** mode as needed) and assign virtual pins:

     * **Value Display** → Virtual Pin **V6** (to show temperature)
     * **Button** (Switch Mode) → Virtual Pin **V4** (to toggle main LED)
     * **Button** (Switch Mode) → Virtual Pin **V5** (“Garage Open” via DC motor forward)
     * **Button** (Switch Mode) → Virtual Pin **V7** (“Garage Close” via DC motor reverse)
     * **Button** (Switch Mode) → Virtual Pin **V1** (to open/close bathroom servo)
   * Optionally label the widgets clearly (e.g., “Toggle LED (V4)”, “Garage Open (V5)”, “Garage Close (V7)”, “Bathroom Door (V1)”).

4. **Retrieve Device Auth Token**

   * In the Blynk Console’s **Devices** tab under your template, click **“Create Device”** (e.g., “ESP32-HomeAuto”).
   * Copy the **Auth Token** for that device (you will paste it into the code in place of `BLYNK_AUTH_TOKEN`).

---

## Installing & Configuring the Code

1. **Clone or Download This Repository**

   ```bash
   git clone https://github.com/<your-username>/<repository-name>.git
   cd <repository-name>
   ```

2. **Open in Arduino IDE**

   * Launch Arduino IDE.
   * Go to **File → Open…** and navigate to the project folder; open `main.ino`.

3. **Install Required Libraries**

   * In Arduino IDE, go to **Sketch → Include Library → Manage Libraries…**
   * Search for and install:

     * **DHT sensor library** by Adafruit
     * **Adafruit Unified Sensor** (dependency)
     * **Blynk**
     * **ESP32Servo**
     * **Keypad**

4. **Edit Wi-Fi & Blynk Credentials**
   At the top of `main.ino`, locate:

   ```cpp
   #define BLYNK_TEMPLATE_ID   "TMPL2FvpK0yPk"
   #define BLYNK_TEMPLATE_NAME "Quickstart Template"
   #define BLYNK_AUTH_TOKEN    "5hzqr_C0MUXV9zRzXed5-3OCLCNXPUPT"

   char ssid[] = "Your_WiFi_SSID";
   char pass[] = "Your_WiFi_Password";
   ```

   * Replace `"Your_WiFi_SSID"` and `"Your_WiFi_Password"` with your network credentials.
   * Replace the three Blynk `#define` lines with **your** Template ID, Template Name, and Auth Token from the Blynk Console.

5. **Set the Keypad PIN (Optional)**

   * By default, the code uses:

     ```cpp
     const char* password = "2227";
     ```
   * To change the 4-digit PIN, update this line (e.g., `"1234"`).

6. **Compile, Upload & Monitor**

   * Select the correct ESP32 board and COM port:

     * **Tools → Board → \[Your ESP32 Model]**
     * **Tools → Port → \[COMx]**
   * Click **Verify/Compile**.
   * Once compiled, click **Upload**.
   * Open the Serial Monitor (**Tools → Serial Monitor**, baud rate **115200**) to watch debug messages (temperature readings, keypad inputs, etc.).

---

## Code Breakdown

Below is a high-level overview of the major sections and functions in `main.ino`.

### 1. Library Inclusions & Defines

```cpp
#include <DHT.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <Keypad.h>
```

* **DHT.h**: Interacts with the DHT11 temperature sensor.
* **BlynkSimpleEsp32.h**: Connects ESP32 to the Blynk IoT platform.
* **ESP32Servo.h**: Drives servo motors on ESP32 pins.
* **Keypad.h**: Reads input from a 4×4 matrix keypad.

```cpp
#define BLYNK_TEMPLATE_ID   "TMPL2FvpK0yPk"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN    "5hzqr_C0MUXV9zRzXed5-3OCLCNXPUPT"
```

* Mandatory for Blynk; replace with your own values.

```cpp
char ssid[] = "Your Wifi SSID";  
char pass[] = "Your Wifi Pass";
```

* Wi-Fi SSID & password; change before uploading.

```cpp
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
```

* Initializes DHT11 on GPIO 26.

```cpp
Servo mainDoorServo;
Servo bathroomServo;
Servo garageServo;
```

* Creates three `Servo` objects for the doors.

```cpp
const int motorPin1 = 14;
const int motorPin2 = 27;
```

* GPIO pins to control the DC motor in forward/reverse.

```cpp
const int mainLed = 25;
const int buzzerPin = 2;
const int trigPin = 23;
const int echoPin = 35;
```

* LED pin for remote toggle, buzzer pin, and ultrasonic sensor pins.

```cpp
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
  { '1','2','3','A' },
  { '4','5','6','B' },
  { '7','8','9','C' },
  { '*','0','#','D' }
};
byte rowPins[ROWS] = { 19, 18, 5, 4 };
byte colPins[COLS] = { 32, 22, 21, 15 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const char* password = "2227";  // Default PIN for main door
String inputPassword = "";
```

* Defines a 4×4 keypad mapping.
* Default 4-digit PIN is `"2227"`.
* `inputPassword` accumulates keystrokes until `#` is pressed.

### 2. Global Blynk Timer

```cpp
BlynkTimer timer;
```

* Schedules periodic tasks without blocking.

### 3. `setup()` Function

```cpp
void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(2000);

  mainDoorServo.attach(13);
  bathroomServo.attach(12);
  garageServo.attach(33);
  closeAllDoors();

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  stopMotor();

  pinMode(mainLed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(mainLed, LOW);
  digitalWrite(buzzerPin, LOW);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, readAndSendTemperature);
  timer.setInterval(500L, handleGarageDoor);

  mainDoorServo.write(100);
}
```

* **Serial.begin(115200)**: For USB debugging.
* **dht.begin()**: Starts DHT sensor.
* **Servo.attach(pin)**: Attaches servos to their GPIO pins.
* **closeAllDoors()**: Sets all servos to position 0 (closed).
* **pinMode + stopMotor()**: Initializes DC motor pins.
* **LED & buzzer**: Configured as outputs, initially off.
* **Ultrasonic pins**: Trigger as OUTPUT, echo as INPUT.
* **Blynk.begin(...)**: Connects to Wi-Fi and Blynk.
* **timer.setInterval(...)**:

  * Every 2 sec → `readAndSendTemperature()`
  * Every 0.5 sec → `handleGarageDoor()`
* **mainDoorServo.write(100)**: Ensures the main door servo is closed at startup.

### 4. `loop()` Function

```cpp
void loop() {
  Blynk.run();
  timer.run();
  handleKeypad();
}
```

* Runs Blynk’s background process, scheduled timers, and polls the keypad each iteration.

### 5. Keypad Handling

```cpp
void handleKeypad() {
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      if (inputPassword == password) {
        Serial.println("Password correct! Opening main door...");
        mainDoorServo.write(0);
        buzz(100);
        delay(3000);
        mainDoorServo.write(100);
      } else {
        Serial.println("Wrong password!");
        buzz(500);
      }
      inputPassword = "";
    } else if (key == '*') {
      inputPassword = "";
    } else {
      inputPassword += key;
      Serial.print("Input: ");
      Serial.println(inputPassword);
    }
  }
}
```

* Reads the pressed key.
* If `#` is pressed:

  * Compares `inputPassword` to `password`.
  * If correct → open main door servo (angle 0), beep 100 ms, delay 3 sec, close (angle 100).
  * If incorrect → beep 500 ms.
  * Resets `inputPassword`.
* If `*` is pressed: clears the current PIN entry.
* Otherwise, appends the key to `inputPassword` and prints it to Serial.

### 6. Buzzer Helper

```cpp
void buzz(int duration) {
  tone(buzzerPin, 2000);
  delay(duration);
  noTone(buzzerPin);
}
```

* Plays a 2 kHz tone on `buzzerPin` for `duration` ms, then silences it.

### 7. Close All Doors Helper

```cpp
void closeAllDoors() {
  mainDoorServo.write(0);
  bathroomServo.write(0);
  garageServo.write(0);
}
```

* Sets all servos to angle 0 (closed); adjust if your mechanical setup differs.

### 8. DC Motor Control via Blynk

```cpp
BLYNK_WRITE(V5) {
  int val = param.asInt();
  if (val == 1) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    delay(500);
    stopMotor();
  }
}

BLYNK_WRITE(V7) {
  int val = param.asInt();
  if (val == 1) {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    delay(500);
    stopMotor();
  }
}

void stopMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}
```

* **V5**: When Blynk button on V5 is pressed (value = 1) → spin the DC motor “forward” for 500 ms, then stop.
* **V7**: When Blynk button on V7 is pressed → spin the DC motor “reverse” for 500 ms, then stop.

### 9. Bathroom Door Servo via Blynk

```cpp
BLYNK_WRITE(V1) {
  int state = param.asInt();
  bathroomServo.write(state ? 80 : 0);
}
```

* **V1**: If switch is ON (param = 1) → set bathroom servo to 80° (open); if OFF → set to 0° (closed). Adjust angles if needed.

### 10. Main LED via Blynk

```cpp
BLYNK_WRITE(V4) {
  digitalWrite(mainLed, param.asInt());
}
```

* **V4**: Toggles `mainLed` HIGH or LOW based on Blynk switch.

### 11. Ultrasonic‐Controlled Garage Door (Auto Mode)

```cpp
void handleGarageDoor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  float distance = duration * 0.034 / 2;

  if (distance > 0 && distance < 10) {
    garageServo.write(80);
    delay(3000);
    garageServo.write(0);
  } else {
    garageServo.write(0);
  }
}
```

* Every 500 ms (via `timer`), pulses the HC-SR04:

  1. Send 10 µs HIGH pulse on `trigPin`.
  2. Measure incoming HIGH on `echoPin`.
  3. Calculate distance: `distance = (duration × 0.034 cm/µs) / 2`.
* If distance is between 0 and 10 cm → set garage servo to 80° (open), wait 3 sec, then close (0°).
* Otherwise → ensure the garage servo stays at 0°.

### 12. Temperature Read & Blynk Update

```cpp
void readAndSendTemperature() {
  float tempC = dht.readTemperature();
  if (isnan(tempC)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.println(" °C");
  Blynk.virtualWrite(V6, tempC);
}
```

* Every 2 sec, read temperature from DHT11.
* If the reading is NaN → print an error to Serial.
* Otherwise → print temperature to Serial and send it to Blynk on Virtual Pin V6.

---

## Usage

1. **Power Up the ESP32**

   * Plug the ESP32 into USB (or use an external 5 V supply for servos).
   * Wait \~10 seconds for Wi-Fi and Blynk connection.

2. **Monitor Serial Output**

   * Open the Serial Monitor at **115200 baud**. You should see:

     ```
     [Timestamp] Ready (IP: 192.168.x.x)
     Temp: 23.4 °C
     Temp: 23.3 °C
     ...
     ```
   * When you press keys on the keypad, it prints entered digits and whether the PIN is correct/incorrect.

3. **Control via Blynk App**

   * In your Blynk dashboard (Dashboard → <Your Device>), you should see:

     * **Value Display** widget showing temperature (V6).
     * **Button** for **V4** to toggle the main LED (ON/OFF).
     * **Buttons** for **V5** and **V7** to open/close the garage via DC motor.
     * **Switch** for **V1** to open/close the bathroom door servo.

4. **Local Door Unlock (Keypad)**

   * On the keypad, type the 4-digit PIN (default “2227”), then press **#**.
   * If correct → main door servo opens for \~3 sec (and beeps), then closes. If incorrect → buzzer beeps longer.

5. **Garage Automation**

   * If an object comes within 10 cm of the ultrasonic sensor, the garage door servo will open (80°) for 3 sec, then close.
   * You can override via Blynk buttons on V5/V7 to drive the DC motor forward or reverse for 0.5 sec.

---

## Troubleshooting

* **Servo Jitter or Inaccurate Positions**

  * Ensure servos have a stable 5 V supply. The ESP32’s 3.3 V pin typically cannot provide enough current.
  * Confirm that the 5 V supply’s ground is shared with the ESP32’s GND.

* **Blynk Not Connecting**

  * Verify Wi-Fi credentials (`ssid`, `pass`) are correct.
  * Ensure the ESP32 is within range of your router.
  * Check the Serial Monitor for Blynk status messages (it will print connection errors).

* **DHT11 Reading “nan”**

  * Check wiring: DHT11 data → GPIO 26, VCC → 3.3 V or 5 V (depending on module), GND → GND.
  * Ensure the data line has a 10 kΩ pull-up resistor (often built into breakout modules).

* **Keypad Doesn’t Register**

  * Double-check row and column pins match `rowPins[]` and `colPins[]`.
  * Ensure the keypad’s membrane connectors are firmly attached.

* **Ultrasonic Sensor Inaccurate**

  * Confirm `trigPin` is GPIO 23 and `echoPin` is GPIO 35.
  * Place sensor on a stable surface facing a reflective object straight on; oblique angles can yield false readings.
  * Adjust the distance threshold in `handleGarageDoor()` if 10 cm is too sensitive or not sensitive enough for your setup.

---

## Customization

* **Change Keypad PIN**

  * Edit the line:

    ```cpp
    const char* password = "2227";
    ```
  * Replace `"2227"` with any 4-digit string you prefer.

* **Adjust Servo Angles**

  * If your servos mount differently, modify calls to `servo.write(0)`, `servo.write(80)`, or `servo.write(100)` to match your mechanical open/closed positions.

* **Modify Distance Threshold**

  * In `handleGarageDoor()`, change:

    ```cpp
    if (distance > 0 && distance < 10) { ... }
    ```
  * For example, use `< 15` to trigger the garage door at a farther distance.

* **Add More Sensors/Actuators**

  * Replicate the pattern of `pinMode` and `BLYNK_WRITE(...)` to include additional relays, LEDs, or sensors on other free GPIO pins.

---

## License

This project is released under the [MIT License](LICENSE). Feel free to use and modify as you wish, and submit PRs or issues for improvements!

---

→ **Enjoy your ESP32 home automation system!**
Feel free to open issues or fork this repo for your own custom expansions.
