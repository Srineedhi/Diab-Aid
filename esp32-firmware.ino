/**
 * FootGuard Pro — ESP32/ESP8266 Firmware (Supabase edition)
 * -------------------------------------------------------------
 * Reads real Force Sensitive Resistors (FSR: heel/midfoot/forefoot/toe) +
 * DS18B20 temperature sensor from your sandal prototype, and pushes each
 * reading directly into your Supabase "sensor_data" table via its REST API
 * (PostgREST) — no separate backend server needed, the ESP32 talks to
 * Supabase directly over HTTPS.
 *
 * PROJECT (already filled in to match your team's supabase-config.js):
 *   URL: https://rzkqmilgfriogresucxb.supabase.co
 *
 * TABLE EXPECTED (create with the SQL in supabase-config.js if you haven't):
 *   sensor_data (id, patient_id, heel, midfoot, forefoot, toe, temperature, status, created_at)
 *
 * LIBRARIES NEEDED (Arduino IDE -> Library Manager):
 *   - OneWire
 *   - DallasTemperature
 *   - ArduinoJson
 *   (ESP8266WiFi / ESP8266HTTPClient are built in for ESP8266 boards;
 *    for ESP32 use WiFi.h + HTTPClient.h instead — same logic either way)
 *
 * HARDWARE ASSUMED (adjust pins to your actual wiring):
 *   - 4x FSR (heel/midfoot/forefoot/toe) — ESP8266 only has one analog pin (A0),
 *     so for 4 independent readings use an external ADC (e.g. ADS1115) or an ESP32
 *     (which has multiple ADC-capable pins). This file assumes an ADS1115-style
 *     4-channel read via readFSR(channel) — swap in your actual sensor read calls.
 *   - 1x DS18B20 temperature sensor on OneWire pin
 *   - 3x vibration motors + 1x buzzer + 1x LED for on-foot feedback
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

// ---- WiFi config ----
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// ---- Supabase config (matches supabase-config.js) ----
#define SUPABASE_URL "https://rzkqmilgfriogresucxb.supabase.co"
#define SUPABASE_ANON_KEY "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InJ6a3FtaWxnZnJpb2dyZXN1Y3hiIiwicm9sZSI6ImFub24iLCJpYXQiOjE3ODQzNzQ4NjMsImV4cCI6MjA5OTk1MDg2M30.Q0Dbjtyc5c-246w2fnoCHuTLcVxl4ce6uUCu9YAszRc"
#define SENSOR_TABLE "sensor_data"
#define PATIENT_ID "P-1042" // this device belongs to this patient record

// ---- Pins (adjust to your actual wiring) ----
const int ONE_WIRE_PIN = D4;
const int VIB_MOTOR_1 = D1;
const int VIB_MOTOR_2 = D2;
const int VIB_MOTOR_3 = D3;
const int BUZZER_PIN = D5;
const int LED_PIN = D6;

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature tempSensor(&oneWire);

// ---- Calibration state (per your research doc, section 3.2.3) ----
float baselineHeel=0, baselineMidfoot=0, baselineForefoot=0, baselineToe=0, baselineTemp=0;
float pressureThreshold = 0; // 1.5 x baseline average pressure

void setup() {
  Serial.begin(115200);
  pinMode(VIB_MOTOR_1, OUTPUT); pinMode(VIB_MOTOR_2, OUTPUT); pinMode(VIB_MOTOR_3, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT); pinMode(LED_PIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(400); Serial.print("."); }
  Serial.println("\nConnected: " + WiFi.localIP().toString());

  tempSensor.begin();
  calibrateBaseline();
}

// Replace these four with your actual per-zone FSR reads (e.g. via an ADS1115 4-channel ADC).
// Placeholder here reads the same analog pin and offsets it, purely so the file compiles standalone.
float readFSR(int zoneIndex) {
  int raw = analogRead(A0);
  float kPa = map(raw, 0, 1023, 0, 60);
  return max(0.0f, kPa + (zoneIndex * 1.5f)); // TODO: replace with real per-channel reads
}

void calibrateBaseline() {
  Serial.println("Calibrating baseline for 30 seconds — keep the sandal still...");
  float hSum=0, mSum=0, fSum=0, tSum2=0, tempSum=0;
  int samples = 0;
  unsigned long start = millis();
  while (millis() - start < 30000) {
    hSum += readFSR(0); mSum += readFSR(1); fSum += readFSR(2); tSum2 += readFSR(3);
    tempSensor.requestTemperatures();
    tempSum += tempSensor.getTempCByIndex(0);
    samples++;
    delay(500);
  }
  baselineHeel = hSum/samples; baselineMidfoot = mSum/samples;
  baselineForefoot = fSum/samples; baselineToe = tSum2/samples;
  baselineTemp = tempSum/samples;
  float avgBaseline = (baselineHeel+baselineMidfoot+baselineForefoot+baselineToe)/4.0;
  pressureThreshold = 1.5 * avgBaseline; // P_threshold = 1.5 x P_baseline, per your calibration formula
  Serial.printf("Baseline set: heel=%.1f midfoot=%.1f forefoot=%.1f toe=%.1f temp=%.1f threshold=%.1f\n",
    baselineHeel, baselineMidfoot, baselineForefoot, baselineToe, baselineTemp, pressureThreshold);
}

String classifyStatus(float avgPressure, float temp) {
  float pressureDelta = max(0.0f, avgPressure - (baselineHeel+baselineMidfoot+baselineForefoot+baselineToe)/4.0f);
  float tempDelta = max(0.0f, temp - baselineTemp);
  float pressureScore = min(100.0f, (pressureDelta/25.0f)*100);
  float tempScore = min(100.0f, (tempDelta/3.0f)*100);
  float score = pressureScore*0.55f + tempScore*0.45f; // simplified on-device version of the dashboard's weighting
  if (score >= 75) return "critical";
  if (score >= 50) return "high";
  if (score >= 25) return "moderate";
  return "low";
}

void pushToSupabase(float heel, float midfoot, float forefoot, float toe, float temperature, String status) {
  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClientSecure client;
  client.setInsecure(); // fine for a hackathon demo; use a proper root CA cert for production
  HTTPClient https;

  String url = String(SUPABASE_URL) + "/rest/v1/" + SENSOR_TABLE;
  if (https.begin(client, url)) {
    https.addHeader("Content-Type", "application/json");
    https.addHeader("apikey", SUPABASE_ANON_KEY);
    https.addHeader("Authorization", String("Bearer ") + SUPABASE_ANON_KEY);
    https.addHeader("Prefer", "return=minimal"); // don't bother sending the inserted row back, saves bandwidth

    StaticJsonDocument<256> doc;
    doc["patient_id"] = PATIENT_ID;
    doc["heel"] = heel;
    doc["midfoot"] = midfoot;
    doc["forefoot"] = forefoot;
    doc["toe"] = toe;
    doc["temperature"] = temperature;
    doc["status"] = status;
    String body;
    serializeJson(doc, body);

    int httpCode = https.POST(body);
    if (httpCode > 0) {
      Serial.printf("Supabase insert -> HTTP %d\n", httpCode);
    } else {
      Serial.printf("Supabase insert failed: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  }
}

void loop() {
  float heel = readFSR(0), midfoot = readFSR(1), forefoot = readFSR(2), toe = readFSR(3);
  tempSensor.requestTemperatures();
  float temperature = tempSensor.getTempCByIndex(0);
  float avgPressure = (heel+midfoot+forefoot+toe)/4.0;

  String status = classifyStatus(avgPressure, temperature);

  // Local feedback on the device itself — immediate, no network round-trip needed
  bool overThreshold = avgPressure > pressureThreshold;
  digitalWrite(LED_PIN, status != "low" ? HIGH : LOW);
  digitalWrite(BUZZER_PIN, status == "critical" ? HIGH : LOW);
  digitalWrite(VIB_MOTOR_1, overThreshold ? HIGH : LOW);
  digitalWrite(VIB_MOTOR_2, (status=="high"||status=="critical") ? HIGH : LOW);
  digitalWrite(VIB_MOTOR_3, status == "critical" ? HIGH : LOW);

  pushToSupabase(heel, midfoot, forefoot, toe, temperature, status);

  Serial.printf("heel=%.1f midfoot=%.1f forefoot=%.1f toe=%.1f temp=%.1f status=%s\n",
    heel, midfoot, forefoot, toe, temperature, status.c_str());

  delay(2500); // matches the dashboard's polling/realtime cadence
}
