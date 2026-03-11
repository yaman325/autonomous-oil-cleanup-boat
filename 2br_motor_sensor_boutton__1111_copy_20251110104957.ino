#include <Wire.h>

#define LIDARLite_ADDRESS 0x62
#define RegisterMeasure    0x00
#define MeasureValue       0x04
#define RegisterHighLowB   0x8f
#define BUZZER_PIN 32

// ===== إعدادات المحركين (ESCs) =====
const int escRight = 13;   // المحرك الأيمن
const int escLeft  = 12;   // المحرك الأيسر

// ===== الزر =====
const int switchPin = 36;  // الزر ON/OFF متصل بـ GPIO14

const int minPulse = 1000;
const int midPulse = 1500;
const int maxPulse = 2000;
const int period   = 20000;

int throttleRight = 0;
int throttleLeft  = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(22, 21);

  pinMode(escRight, OUTPUT);
  pinMode(escLeft, OUTPUT);
  pinMode(switchPin, INPUT); // استخدم INPUT_PULLDOWN لو كان متاح
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(escRight, LOW);
  digitalWrite(escLeft, LOW);
  digitalWrite(BUZZER_PIN, HIGH);

  Serial.println("تهيئة LIDAR-Lite و ESC للمحركين مع زر تشغيل...");

  sendNeutral(escRight);
  sendNeutral(escLeft);
  delay(3000);
}

void loop() {
  bool isOn = digitalRead(switchPin);

  if (!isOn) {
    // الزر في وضع OFF → توقف تام
    sendPWMCycle(escLeft, 0);
    sendPWMCycle(escRight, 0);
    Serial.println("النظام متوقف (OFF).");
    delay(200);
    return; // لا تكمل البرنامج
  }

  // الزر في وضع ON → نفّذ البرنامج
  takeRangeReading();
  delay(20);
  int distance = readDistance();

  Serial.print("المسافة: ");
  Serial.print(distance);
  Serial.println(" سم");

  if (distance > 0) {
    if (distance <= 20) {
      Serial.println("عائق قريب! دوران لتفاديه...");
      throttleLeft  = 0;
      throttleRight = 40;
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);    // مدة الصوت بالملّي ثانية
    } else {
      throttleLeft  = 70;
      throttleRight = 70;
      digitalWrite(BUZZER_PIN, HIGH);

    }
  } else {
    throttleLeft  = 0;
    throttleRight = 0;
    digitalWrite(BUZZER_PIN, HIGH);

  }

  sendPWMCycle(escLeft, throttleLeft);
  sendPWMCycle(escRight, throttleRight);

  delay(50);
}

// ====== دوال الحساس ======
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
  } else {
    return -1;
  }
}

// ====== دوال ESC ======
void sendNeutral(int escPin) {
  for (int i = 0; i < 200; i++) {
    sendPulse(escPin, midPulse);
  }
}

void sendPWMCycle(int escPin, int percent) {
  int pulseWidth;

  if (percent == 0) {
    pulseWidth = midPulse;
  } else if (percent > 0) {
    pulseWidth = map(percent, 0, 100, midPulse, maxPulse);
  } else {
    pulseWidth = map(percent, -100, 0, minPulse, midPulse);
  }

  sendPulse(escPin, pulseWidth);
}

void sendPulse(int escPin, int pulseWidth) {
  digitalWrite(escPin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(escPin, LOW);
  delayMicroseconds(period - pulseWidth);
}
