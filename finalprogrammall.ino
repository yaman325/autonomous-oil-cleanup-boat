#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>

// ===== Program 1: DC Motor + Touch + Water Level =====
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;
int touchPin = 15;
const int touchThreshold = 30;
int dutyCycle = 230;

bool motorState1 = false;
bool touched = false;
bool lastWaterState = false;

// Water level sensor pin
int waterPin = 4;

// ===== Program 2: ESCs + LIDAR + Buzzer + Button =====
#define LIDARLite_ADDRESS 0x62
#define RegisterMeasure   0x00
#define MeasureValue      0x04
#define RegisterStatus    0x01
#define RegisterDistance  0x8f

#define BUZZER_PIN 25
#define ALERT_DISTANCE 10

Servo esc1;
Servo esc2;
int escPin1 = 32;
int escPin2 = 33;

int buttonPin = 2;
int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

enum SystemState {STOPPED, RUNNING, STOPPING};
SystemState systemState = STOPPED;

#define NUM_READINGS 5
int readings[NUM_READINGS];
int readIndex = 0;
int total = 0;
int averageDistance = 0;

int esc1Speed = 1000;
int esc2Speed = 1000;
const int speedStep = 40;
const int maxSpeed = 1600;

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Motor 1
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(enable1Pin, 0);

  pinMode(touchPin, INPUT);
  pinMode(waterPin, INPUT_PULLUP); // حساس الطفو

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // ESCs
  esc1.attach(escPin1, 1000, 2000);
  esc2.attach(escPin2, 1000, 2000);
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);

  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize LIDAR readings
  for (int i = 0; i < NUM_READINGS; i++) readings[i] = 0;

  Serial.println("=== System Ready ===");
}

// ===== Loop =====
void loop() {
  // ===== Program 1: Touch + Water Level =====
  bool waterFull = (digitalRead(waterPin) == HIGH); // HIGH = ممتلئ
  if (waterFull != lastWaterState) {
    Serial.println(waterFull ? "💧 الخزان ممتلئ" : "💧 الخزان فاضي");
    lastWaterState = waterFull;
  }

  // إذا ممتلئ → إيقاف المحرك فورًا
  if (waterFull) motorState1 = false;

  int touchValue = touchRead(touchPin);
  if (touchValue < touchThreshold && !touched) {
    motorState1 = !motorState1;
    touched = true;
    Serial.print(">> Touch: MOTOR1 ");
    Serial.println(motorState1 ? "ON" : "OFF");
    delay(300);
  }
  if (touchValue >= touchThreshold) touched = false;

  if (motorState1) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    analogWrite(enable1Pin, dutyCycle);
  } else {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(enable1Pin, 0);
  }

  // ===== Program 2: Button + ESCs + LIDAR + Buzzer =====
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        if (systemState == STOPPED || systemState == STOPPING) {
          systemState = RUNNING;
          Serial.println("زر ESCs: بدء التشغيل التدريجي");
        } else if (systemState == RUNNING) {
          systemState = STOPPING;
          Serial.println("زر ESCs: بدء الإيقاف التدريجي");
        }
      }
    }
  }
  lastButtonState = reading;

  int distance = getStableDistance();
  total = total - readings[readIndex];
  readings[readIndex] = distance;
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % NUM_READINGS;
  averageDistance = total / NUM_READINGS;

  bool buzzerActive = false;
  if (averageDistance > 0 && averageDistance <= ALERT_DISTANCE) {
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerActive = true;
    esc1Speed = 1000;
  } else digitalWrite(BUZZER_PIN, LOW);

  bool esc1Active = false;
  bool esc2Active = false;

  if (systemState == RUNNING) {
    if (esc2Speed < maxSpeed) esc2Speed += speedStep;
    if (averageDistance > 0 && averageDistance <= ALERT_DISTANCE) {
      esc1Speed = 1000;
      esc2Active = true;
    } else {
      if (esc1Speed < esc2Speed) esc1Speed += speedStep;
      if (esc1Speed > esc2Speed) esc1Speed = esc2Speed;
      esc1Active = true;
      esc2Active = true;
    }
  } else if (systemState == STOPPING) {
    if (esc1Speed > 1000) esc1Speed -= speedStep;
    if (esc2Speed > 1000) esc2Speed -= speedStep;
    if (esc1Speed < 1000) esc1Speed = 1000;
    if (esc2Speed < 1000) esc2Speed = 1000;
    esc1Active = esc1Speed > 1000;
    esc2Active = esc2Speed > 1000;
    if (esc1Speed == 1000 && esc2Speed == 1000) systemState = STOPPED;
  }

  esc1.writeMicroseconds(esc1Speed);
  esc2.writeMicroseconds(esc2Speed);

  // ===== Serial Output =====
  Serial.print("[ESC1: "); printSpeedBar(esc1Speed);
  Serial.print(" | ESC2: "); printSpeedBar(esc2Speed);
  Serial.print(" | Distance: "); Serial.print(averageDistance);
  Serial.print("cm | Buzzer: "); Serial.print(buzzerActive ? "ON" : "OFF");
  Serial.print(" | State: ");
  switch (systemState) {
    case RUNNING: Serial.print("تشغيل تدريجي"); break;
    case STOPPING: Serial.print("إيقاف تدريجي"); break;
    case STOPPED: Serial.print("متوقف"); break;
  }
  Serial.println(" ]");

  delay(25);
}

// ===== Functions =====
void printSpeedBar(int speed) {
  int length = map(speed, 1000, maxSpeed, 0, 20);
  for (int i = 0; i < length; i++) Serial.print("█");
  for (int i = length; i < 20; i++) Serial.print("-");
}

int getStableDistance() {
  for (int attempt = 0; attempt < 3; attempt++) {
    int d = singleDistanceRead();
    if (d > 0 && d < 4000) return d;
    delay(5);
  }
  return -1;
}

int singleDistanceRead() {
  Wire.beginTransmission(LIDARLite_ADDRESS);
  Wire.write(RegisterMeasure);
  Wire.write(MeasureValue);
  if (Wire.endTransmission() != 0) return -1;

  unsigned long startTime = millis();
  while (true) {
    Wire.beginTransmission(LIDARLite_ADDRESS);
    Wire.write(RegisterStatus);
    Wire.endTransmission(false);
    Wire.requestFrom(LIDARLite_ADDRESS, 1);

    if (Wire.available()) {
      byte status = Wire.read();
      if (!(status & 0x01)) break;
    }

    if (millis() - startTime > 50) return -1;
    delay(1);
  }

  Wire.beginTransmission(LIDARLite_ADDRESS);
  Wire.write(RegisterDistance);
  Wire.endTransmission(false);
  Wire.requestFrom(LIDARLite_ADDRESS, 2);

  if (Wire.available() == 2) {
    int highByte = Wire.read();
    int lowByte  = Wire.read();
    return (highByte << 8) | lowByte;
  }

  return -1;
}
