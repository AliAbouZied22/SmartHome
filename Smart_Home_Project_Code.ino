#define BLYNK_TEMPLATE_ID "TMPL2FvpK0yPk"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "5hzqr_C0MUXV9zRzXed5-3OCLCNXPUPT"

#include <DHT.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <Keypad.h>

char ssid[] = "Your Wifi SSID";
char pass[] = "Your Wifi Pass";

#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo mainDoorServo;
Servo bathroomServo;
Servo garageServo;

const int motorPin1 = 14;
const int motorPin2 = 27;

const int mainLed = 25;
const int buzzerPin = 2;

const int trigPin = 23;
const int echoPin = 35;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 19, 18, 5, 4 };
byte colPins[COLS] = { 32, 22, 21, 15 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const char* password = "2227";
String inputPassword = "";

BlynkTimer timer;

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

void loop() {
  Blynk.run();
  timer.run();
  handleKeypad();
}

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

void buzz(int duration) {
  tone(buzzerPin, 2000);
  delay(duration);
  noTone(buzzerPin);
}

void closeAllDoors() {
  mainDoorServo.write(0);
  bathroomServo.write(0);
  garageServo.write(0);
}

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

BLYNK_WRITE(V1) {
  int state = param.asInt();
  bathroomServo.write(state ? 80 : 0);
}

BLYNK_WRITE(V4) {
  digitalWrite(mainLed, param.asInt());
}

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
