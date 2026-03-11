#define IN1 26
#define IN2 27
#define ENA 14
#define floatPin 4     // حساس مستوى الماء (HIGH = ممتلئ)
#define BUTTON_PIN 15   // الزر (طرف إلى D15 وطرف إلى GND)

bool motorState = false;    // حالة المحرك (تشغيل/إيقاف)
bool lastButtonState = HIGH; // لأننا نستخدم INPUT_PULLUP

int speedValue = 255; // سرعة المحرك (0–255)

void setup() {
  pinMode(floatPin, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  Serial.begin(115200);
  stopMotor();
  Serial.println("🚀 النظام جاهز للتشغيل...");
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);
  bool full = digitalRead(floatPin);

  // عند الضغط على الزر (من HIGH إلى LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    motorState = !motorState; // تبديل الحالة
    delay(250); // منع الاهتزاز
  }
  lastButtonState = buttonState;

  // إذا امتلأ الخزان، أوقف المحرك تلقائيًا
  if (full == HIGH) {
    motorState = false;
    Serial.println("💧 الخزان ممتلئ - المحرك توقف تلقائيًا!");
  }

  // تنفيذ الحالة الحالية
  if (motorState) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speedValue);
    Serial.println("✅ المحرك يعمل...");
  } else {
    stopMotor();
    Serial.println("⛔ المحرك متوقف.");
  }

  delay(200);
}

void runMotor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedValue); // التحكم بالسرعة
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}