/*
 * IoT-Based Gas Leakage Detection and Alert System
 * Board  : NodeMCU (ESP8266)
 * Sensor : MQ-2 (analog -> A0)
 * Output : LED (D5), Buzzer (D6), SSD1306 OLED 128x64 (I2C, 0x3C)
 * Cloud  : Blynk IoT (V0 = gas value, event "gas_alert" = notification)
 *
 * Before uploading:
 *   1. Copy secrets.h.example to secrets.h (same folder as this sketch)
 *   2. Fill in your Wi-Fi name, password and Blynk Auth Token
 */

// --- Blynk template details (from Blynk Console) ---
#define BLYNK_TEMPLATE_ID   "TMPL3pD7LvUor"
#define BLYNK_TEMPLATE_NAME "GAS LEAKAGE ALERT"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Secure credentials (WIFI_SSID, WIFI_PASS, BLYNK_AUTH_TOKEN) ---
#include "secrets.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

// --- Pin configuration ---
int smokeA0   = A0;   // MQ-2 analog pin
int ledPin    = D5;   // Red LED
int buzzerPin = D6;   // Buzzer

// --- SENSOR CALIBRATION ---
// 1. Run the code and let it read clean air for ~5 minutes.
// 2. Note the highest "Gas Sensor Value" in the Serial Monitor (e.g. 120).
// 3. Set the threshold 50-100 points above that (e.g. 220).
// 300 is only a placeholder value.
int sensorThres = 300;

// --- OLED display setup ---
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Blynk timer ---
BlynkTimer timer;

// --- State management (prevents notification spam) ---
// false = Safe, true = Alert active
bool isAlertActive = false;

// Called every second: read sensor, update OLED/LED/buzzer, notify Blynk
void sendSensor() {
  int data = analogRead(smokeA0);
  Blynk.virtualWrite(V0, data);

  Serial.print("Gas Sensor Value: ");
  Serial.println(data);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("Gas Value: ");
  display.println(data);

  if (data > sensorThres) {
    display.setCursor(0, 30);
    display.setTextSize(2);
    display.print("ALERT!");
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("GAS LEAK DETECTED");

    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);

    if (!isAlertActive) {
      Serial.println("State Changed: ALERT - Sending Notification");
      Blynk.logEvent("gas_alert", "Gas Leakage Detected!");
      isAlertActive = true;
    }
  } else {
    display.setCursor(0, 30);
    display.setTextSize(2);
    display.print("SAFE");
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("System Normal");

    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);

    if (isAlertActive) {
      Serial.println("State Changed: SAFE");
    }
    isAlertActive = false;
  }
  display.display();
}

void setup() {
  Serial.begin(9600);
  Serial.println("\nStarting Gas Leakage Detector...");

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED initialization failed!"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Connecting to WiFi...");
  display.println(ssid);
  display.display();

  Blynk.begin(auth, ssid, pass);

  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Connected!");
  display.setCursor(0, 30);
  display.println("Sensor Warming Up...");
  display.display();

  delay(30000L);  // MQ-2 warm-up: 30 seconds

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("Gas Leak System Ready");
  display.display();
  delay(2000);

  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
