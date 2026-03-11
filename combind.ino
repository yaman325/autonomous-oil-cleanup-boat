#define BLYNK_PRINT Serial
// ===== بيانات Blynk IoT =====
#define BLYNK_TEMPLATE_ID   "TMPL6DUO_YLL1"
#define BLYNK_TEMPLATE_NAME "ocean_monsterv.2.0"
#define BLYNK_AUTH_TOKEN    "KBS_Yam3mk2211"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>



// ===== WiFi =====
char ssid[] = "yam";
char pass[] = "12345678";

// ===== LiDAR =====
#define LIDARLite_ADDRESS 0x62
#define RegisterMeasure    0x00
#define MeasureValue       0x04
#define RegisterHighLowB   0x8f

// ===== Pins =====
#define BUZZER_PIN 32
const int escRight = 13;
const int escLeft  = 12;

#define IN1 26
#define IN2 27
#define ENA 14
#define floatPin 4
#define BUTTON_PIN 15
#define POWER_BUTTON 36

// ===== ESC values =====
const int minPulse = 1000;
const int midPulse = 1500;
const int maxPulse = 2000;
const int period   = 20000;

// ===== States =====
bool systemState = false;
bool motorState  = false;
int baseSpeed    = 70;

bool lastPowerButton = HIGH;
unsigned long lastPowerDebounce = 0;

// ===== Blynk Virtual Pins =====
BLYNK_WRITE(V0) { systemState = param.asInt(); }  // زر تشغيل/ايقاف النظام
BLYNK_WRITE(V1) { baseSpeed   = param.asInt(); }  // Slider سرعة المحركين
BLYNK_WRITE(V2) { motorState  = param.asInt(); }  // زر تشغيل/ايقاف المضخة

void setup() {
  Serial.begin(115200);
  Wire.begin(22, 21);

  pinMode(escRight, OUTPUT);
  pinMode(escLeft, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(floatPin, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POWER_BUTTON, INPUT_PULLUP);

  digitalWrite(BUZZER_PIN, HIGH);
  stopMotor();

  sendNeutral(escRight);
  sendNeutral(escLeft);
  delay(3000);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Blynk Connected");
}

void loop() {
  Blynk.run();

  // ===== زر التشغيل الفيزيائي =====
  bool readingPower = digitalRead(POWER_BUTTON);
  if (readingPower == LOW && lastPowerButton == HIGH &&
      millis() - lastPowerDebounce > 200) {
    systemState = !systemState;
    lastPowerDebounce = millis();
  }
  lastPowerButton = readingPower;

  // ===== التحكم بالمحركين =====
  if (!systemState) {
    sendPWMCycle(escLeft, 0);
    sendPWMCycle(escRight, 0);
  } else {
    takeRangeReading();
    delay(20);
    int distance = readDistance();

    int leftSpeed  = baseSpeed;
    int rightSpeed = baseSpeed;

    if (distance > 0 && distance <= 20) {
      leftSpeed = 0;
      rightSpeed = baseSpeed / 2;
      digitalWrite(BUZZER_PIN, LOW);
    } else {
      digitalWrite(BUZZER_PIN, HIGH);
    }

    sendPWMCycle(escLeft, leftSpeed);
    sendPWMCycle(escRight, rightSpeed);
  }

  // ===== مضخة الماء =====
  bool full = digitalRead(floatPin);

  if (full == HIGH) {
    motorState = false; // حماية الخزان
  }

  if (motorState) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 255);
  } else {
    stopMotor();
  }

  // ===== إرسال حالة الخزان إلى Blynk =====
  Blynk.virtualWrite(V3, full == HIGH ? 255 : 0);

  delay(50);
}

// ===== LiDAR =====
void takeRangeReading() {
  Wire.beginTransmission(LIDARLite_ADDRESS);
  Wire.write(RegisterMeasure);
  Wire.write(MeasureValue);
  Wire.endTransmission();
}

int readDistance() {
  Wire.beginTransmission(LIDARLite_ADDRESS);
  Wire.write(RegisterHighLowB);
  Wire.endTransmission(false);
  Wire.requestFrom(LIDARLite_ADDRESS, 2);

  if (Wire.available() == 2) {
    int highByte = Wire.read();
    int lowByte = Wire.read();
    return (highByte << 8) + lowByte;
  }
  return -1;
}

// ===== ESC =====
void sendNeutral(int escPin) {
  for (int i = 0; i < 200; i++) {
    sendPulse(escPin, midPulse);
  }
}

void sendPWMCycle(int escPin, int percent) {
  int pulseWidth;
  if (percent == 0) pulseWidth = midPulse;
  else pulseWidth = map(percent, 0, 100, midPulse, maxPulse);
  sendPulse(escPin, pulseWidth);
}

void sendPulse(int escPin, int pulseWidth) {
  digitalWrite(escPin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(escPin, LOW);
  delayMicroseconds(period - pulseWidth);
}

// ===== DC Motor =====
void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}
