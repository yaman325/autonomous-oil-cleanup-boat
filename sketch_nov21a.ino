#include <Wire.h>
#include <TinyGPS++.h>   // <<< مكتبة TinyGPSPlus المطلوبة

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

#define IN1 26
#define IN2 27
#define ENA 14
#define floatPin 4     // حساس مستوى الماء (HIGH = ممتلئ)
#define BUTTON_PIN 15   // الزر (طرف إلى D15 وطرف إلى GND)

bool motorState = false;    // حالة المحرك (تشغيل/إيقاف)
bool lastButtonState = HIGH; // لأننا نستخدم INPUT_PULLUP

int speedValue = 255; // سرعة المحرك (0–255)

// ====== إضافات GPS ======
TinyGPSPlus gps;          // كائن تحليل بيانات GPS
// سنستخدم Serial2 على ESP32: RX2 = GPIO16، TX2 = GPIO17
HardwareSerial GPSSerial(2);
unsigned long lastGpsPrint = 0;
const unsigned long gpsPrintInterval = 1000; // طباعة كل 1 ثانية تقريبًا

void setup() {
  Serial.begin(115200);
  Wire.begin(22, 21);

  pinMode(escRight, OUTPUT);
  pinMode(escLeft, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(escRight, LOW);
  digitalWrite(escLeft, LOW);
  digitalWrite(BUZZER_PIN, HIGH);

  pinMode(floatPin, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  stopMotor();

  Serial.println("تهيئة كاملة...");

  sendNeutral(escRight);
  sendNeutral(escLeft);
  delay(3000);

  // تهيئة Serial2 لوصلة الـGPS
  GPSSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("GPS Serial2 initialized on RX=16, TX=17 (baud 9600).");
}

void loop() {
  bool isOn = digitalRead(switchPin);

  if (!isOn) {
    sendPWMCycle(escLeft, 0);
    sendPWMCycle(escRight, 0);
    Serial.println("النظام متوقف (OFF).");
  } else {
    // تحديد قيم المحركات بشكل ثابت لأننا حذفنا الليدار
    throttleLeft  = 70;
    throttleRight = 70;
    digitalWrite(BUZZER_PIN, HIGH);

    sendPWMCycle(escLeft, throttleLeft);
    sendPWMCycle(escRight, throttleRight);
  }

  bool buttonState = digitalRead(BUTTON_PIN);
  bool full = digitalRead(floatPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    motorState = !motorState;
    delay(250);
  }
  lastButtonState = buttonState;

  if (full == HIGH) {
    motorState = false;
    Serial.println("💧 الخزان ممتلئ - المحرك توقف تلقائيًا!");
  }

  if (motorState) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speedValue);
    Serial.println("✅ المحرك يعمل...");
  } else {
    stopMotor();
    Serial.println("⛔ المحرك متوقف.");
  }

  // === قراءة ومعالجة بيانات GPS بشكل غير حاجز (non-blocking) ===
  readGPS();

  delay(100);
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

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

// ====== دوال GPS ======
void readGPS() {
  while (GPSSerial.available() > 0) {
    char c = (char)GPSSerial.read();
    gps.encode(c);
  }

  unsigned long now = millis();
  if (now - lastGpsPrint >= gpsPrintInterval) {
    lastGpsPrint = now;

    if (gps.location.isValid()) {
      Serial.print("GPS: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(", ");
      Serial.print(gps.location.lng(), 6);
      Serial.print(" | Sats: ");
      Serial.print(gps.satellites.value());
      Serial.print(" | HDOP: ");
      if (gps.hdop.isValid()) Serial.print(gps.hdop.hdop());
      else Serial.print("n/a");
      Serial.print(" | Speed(kph): ");
      if (gps.speed.isValid()) Serial.print(gps.speed.kmph());
      else Serial.print("n/a");
      Serial.print(" | Time: ");
      if (gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print('0');
        Serial.print(gps.time.hour());
        Serial.print(':');
        if (gps.time.minute() < 10) Serial.print('0');
        Serial.print(gps.time.minute());
        Serial.print(':');
        if (gps.time.second() < 10) Serial.print('0');
        Serial.print(gps.time.second());
      } else {
        Serial.print("n/a");
      }
      Serial.println();
    } else {
      Serial.println("GPS: لا يوجد قفل على الموقع (no fix) حتى الآن.");
    }
  }
}
