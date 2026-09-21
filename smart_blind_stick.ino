/* ================================================================
   SMART BLIND STICK — Complete Final Version
   ----------------------------------------------------------------
   Features:
   - 3 Ultrasonic sensors (front, left, right)
   - Water sensor (puddle detection)
   - 2 Vibration motors
   - LOUD active buzzer with tone()
   - SOS emergency feature (when all 3 sensors < 15cm)
   ================================================================ */

// ===== PIN DEFINITIONS =====
const int TRIG_FRONT = 2, ECHO_FRONT = 3;
const int TRIG_LEFT  = 4, ECHO_LEFT  = 5;
const int TRIG_RIGHT = 6, ECHO_RIGHT = 7;

const int BUZZER_PIN    = 11;
const int VIBRATION_1   = 12;
const int VIBRATION_2   = 13;
const int WATER_PIN     = A0;

// ===== THRESHOLDS =====
const int DANGER_DIST   = 30;
const int WARNING_DIST  = 60;
const int SOS_DIST      = 15;
const int WATER_TRIGGER = 300;
const long SOS_COOLDOWN = 30000;

// ===== STATE =====
unsigned long lastBeepTime = 0;
bool beepState             = false;
unsigned long lastSosTime  = 0;

// ================================================================
// SETUP
// ================================================================
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_FRONT, OUTPUT); pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT,  OUTPUT); pinMode(ECHO_LEFT,  INPUT);
  pinMode(TRIG_RIGHT, OUTPUT); pinMode(ECHO_RIGHT, INPUT);

  pinMode(BUZZER_PIN,  OUTPUT);
  pinMode(VIBRATION_1, OUTPUT);
  pinMode(VIBRATION_2, OUTPUT);

  Serial.println("=== Smart Blind Stick — Ready ===");
}

// ================================================================
// SENSOR HELPERS
// ================================================================
long measureDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void vibrateOn() {
  digitalWrite(VIBRATION_1, HIGH);
  digitalWrite(VIBRATION_2, HIGH);
}

void vibrateOff() {
  digitalWrite(VIBRATION_1, LOW);
  digitalWrite(VIBRATION_2, LOW);
}

void loudBeep(int interval, int frequency) {
  unsigned long now = millis();
  if (now - lastBeepTime >= interval) {
    lastBeepTime = now;
    beepState = !beepState;
    if (beepState) tone(BUZZER_PIN, frequency);
    else           noTone(BUZZER_PIN);
  }
}

void allAlertsOff() {
  noTone(BUZZER_PIN);
  vibrateOff();
  beepState = false;
}

// ================================================================
// SOS EMERGENCY PATTERN — Morse Code: ... --- ...
// ================================================================
void triggerSOS() {
  if (millis() - lastSosTime < SOS_COOLDOWN) {
    Serial.println("(SOS cooldown active)");
    return;
  }
  lastSosTime = millis();

  Serial.println("\n SOS EMERGENCY ");
  Serial.println("User may be trapped or fallen!");

  vibrateOn();

  for (int repeat = 0; repeat < 2; repeat++) {
    // S = ... (3 short)
    for (int i = 0; i < 3; i++) {
      tone(BUZZER_PIN, 2500); delay(200);
      noTone(BUZZER_PIN);     delay(200);
    }
    delay(400);
    // O = --- (3 long)
    for (int i = 0; i < 3; i++) {
      tone(BUZZER_PIN, 2500); delay(500);
      noTone(BUZZER_PIN);     delay(200);
    }
    delay(400);
    // S = ... (3 short)
    for (int i = 0; i < 3; i++) {
      tone(BUZZER_PIN, 2500); delay(200);
      noTone(BUZZER_PIN);     delay(200);
    }
    delay(1000);
  }

  vibrateOff();
  Serial.println("SOS pattern complete\n");
}

// ================================================================
// MAIN LOOP
// ================================================================
void loop() {
  long distFront    = measureDistance(TRIG_FRONT, ECHO_FRONT);
  long distLeft     = measureDistance(TRIG_LEFT,  ECHO_LEFT);
  long distRight    = measureDistance(TRIG_RIGHT, ECHO_RIGHT);
  int  waterValue   = analogRead(WATER_PIN);
  bool waterDetected = (waterValue > WATER_TRIGGER);

  // Print to Serial Monitor
  Serial.print("F:"); Serial.print(distFront); Serial.print(" | ");
  Serial.print("L:"); Serial.print(distLeft);  Serial.print(" | ");
  Serial.print("R:"); Serial.print(distRight); Serial.print(" | ");
  Serial.print("Water:"); Serial.print(waterValue);
  Serial.println(waterDetected ? " WET!" : " dry");

  // SOS check — all 3 sensors very close = user trapped/fallen
  bool sosCondition = (distFront < SOS_DIST &&
                       distLeft  < SOS_DIST &&
                       distRight < SOS_DIST);
  if (sosCondition) {
    triggerSOS();
    return;
  }

  // Normal graduated alerts
  long minDist = min(distFront, min(distLeft, distRight));

  if (waterDetected) {
    loudBeep(60,  3000);
    vibrateOn();
  }
  else if (minDist <= DANGER_DIST) {
    loudBeep(80,  2500);
    vibrateOn();
  }
  else if (minDist <= WARNING_DIST) {
    loudBeep(250, 1800);
    vibrateOn();
  }
  else {
    allAlertsOff();
  }

  delay(50);
}
