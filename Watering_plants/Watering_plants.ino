#define BLYNK_TEMPLATE_ID "TMPL3EFrJ2rXa"
#define BLYNK_TEMPLATE_NAME "Plant Watering"
#define BLYNK_AUTH_TOKEN "ag8f1ZKQK987psCPjhXuTxpt2VIOnD7h"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "sanjay";
char pass[] = "12345678";

// Pins
#define MOISTURE_PIN 34
#define RELAY1 25
#define RELAY2 26
#define RELAY3 27

// Moisture thresholds
int threshold1 = 50;
int threshold2 = 70;
int threshold3 = 90;

// Variables
int moistureValue = 0;
int moisturePercent = 0;
bool manualMode = false;   // Auto = false, Manual = true
bool manual1 = false;
bool manual2 = false;
bool manual3 = false;

BlynkTimer timer;

// Virtual pins
#define VPIN_MODE V0      // Auto/Manual switch
#define VPIN_PUMP1 V1     // Manual button Plant 1
#define VPIN_PUMP2 V2     // Manual button Plant 2
#define VPIN_PUMP3 V3     // Manual button Plant 3
#define VPIN_MOISTURE V5  // Moisture gauge

// === Function to smooth moisture readings ===
int getSmoothMoisture(int samples = 10) {
  long total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(MOISTURE_PIN);
    delay(5);
  }
  return total / samples;
}

// === Auto/Manual switch handler ===
BLYNK_WRITE(VPIN_MODE) {
  manualMode = param.asInt();
  if (!manualMode) {
    // Reset manual buttons and turn off pumps
    manual1 = manual2 = manual3 = false;
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY3, HIGH);
    Blynk.virtualWrite(VPIN_PUMP1, 0);
    Blynk.virtualWrite(VPIN_PUMP2, 0);
    Blynk.virtualWrite(VPIN_PUMP3, 0);
  }
}

// === Manual pump button handlers ===
BLYNK_WRITE(VPIN_PUMP1) { if (manualMode) manual1 = param.asInt(); }
BLYNK_WRITE(VPIN_PUMP2) { if (manualMode) manual2 = param.asInt(); }
BLYNK_WRITE(VPIN_PUMP3) { if (manualMode) manual3 = param.asInt(); }

// === Main soil checking logic ===
void checkSoil() {
  // Smooth moisture reading
  moistureValue = getSmoothMoisture();
  moisturePercent = map(moistureValue, 4095, 2000, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // Update Blynk gauge
  Blynk.virtualWrite(VPIN_MOISTURE, moisturePercent);

  // ---- Plant 1 ----
  if (manualMode) {
    digitalWrite(RELAY1, manual1 ? LOW : HIGH);
  } else {
    bool newState = (moisturePercent < threshold1);
    digitalWrite(RELAY1, newState ? LOW : HIGH);
    Blynk.virtualWrite(VPIN_PUMP1, newState ? 1 : 0); // Sync button
  }

  // ---- Plant 2 ----
  if (manualMode) {
    digitalWrite(RELAY2, manual2 ? LOW : HIGH);
  } else {
    bool newState = (moisturePercent < threshold2);
    digitalWrite(RELAY2, newState ? LOW : HIGH);
    Blynk.virtualWrite(VPIN_PUMP2, newState ? 1 : 0); // Sync button
  }

  // ---- Plant 3 ----
  if (manualMode) {
    digitalWrite(RELAY3, manual3 ? LOW : HIGH);
  } else {
    bool newState = (moisturePercent < threshold3);
    digitalWrite(RELAY3, newState ? LOW : HIGH);
    Blynk.virtualWrite(VPIN_PUMP3, newState ? 1 : 0); // Sync button
  }

  // --- Debug monitor ---
  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.print("% | Mode: ");
  Serial.print(manualMode ? "Manual" : "Auto");
  Serial.print(" | Pump1: ");
  Serial.print(digitalRead(RELAY1) ? "OFF" : "ON");
  Serial.print(" | Pump2: ");
  Serial.print(digitalRead(RELAY2) ? "OFF" : "ON");
  Serial.print(" | Pump3: ");
  Serial.println(digitalRead(RELAY3) ? "OFF" : "ON");
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, checkSoil); // Every 2 sec
}

void loop() {
  Blynk.run();
  timer.run();
}
