#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "DHT.h"

// -------------------- USER CONFIG --------------------
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

String THINGSPEAK_API_KEY = "YOUR_THINGSPEAK_WRITE_KEY";

// DHT config
#define DHTPIN D4         // GPIO2 (NodeMCU D4)
#define DHTTYPE DHT22     // change to DHT11 if you have DHT11

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C    // common OLED I2C address

// Soil config (LM393 analog output to A0)
// Calibration: adjust these after checking your own readings in wet/dry soil.
int SOIL_WET = 350;   // reading when probe is in water / very wet soil
int SOIL_DRY = 850;   // reading when probe is in dry soil
// -----------------------------------------------------

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL_MS = 20000; // 20 seconds

int clampInt(int x, int lo, int hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

int soilPercentFromRaw(int raw) {
  // Convert raw to percentage (100% = wet, 0% = dry)
  raw = clampInt(raw, SOIL_WET, SOIL_DRY);
  long pct = map(raw, SOIL_DRY, SOIL_WET, 0, 100);
  return (int)clampInt((int)pct, 0, 100);
}

void showOnOLED(int soilRaw, int soilPct, float t, float h, bool wifiOk) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Smart Agri Monitor");

  display.setCursor(0, 14);
  display.print("Soil: ");
  display.print(soilPct);
  display.print("% (");
  display.print(soilRaw);
  display.print(")");

  display.setCursor(0, 28);
  display.print("Temp: ");
  if (isnan(t)) display.print("NA");
  else {
    display.print(t, 1);
    display.print(" C");
  }

  display.setCursor(0, 42);
  display.print("Hum : ");
  if (isnan(h)) display.print("NA");
  else {
    display.print(h, 1);
    display.print(" %");
  }

  display.setCursor(0, 56);
  display.print("WiFi: ");
  display.print(wifiOk ? "OK" : "OFF");

  display.display();
}

bool sendToThingSpeak(int soilPct, float t, float h) {
  if (WiFi.status() != WL_CONNECTED) return false;

  WiFiClient client;
  HTTPClient http;

  String url = "http://api.thingspeak.com/update?api_key=" + THINGSPEAK_API_KEY +
               "&field1=" + String(soilPct) +
               "&field2=" + (isnan(t) ? String("") : String(t, 1)) +
               "&field3=" + (isnan(h) ? String("") : String(h, 1));

  http.begin(client, url);
  int httpCode = http.GET();
  String payload = http.getString();
  http.end();

  // ThingSpeak returns entry ID as payload if successful (e.g., "12")
  Serial.print("ThingSpeak HTTP: ");
  Serial.print(httpCode);
  Serial.print(" | Response: ");
  Serial.println(payload);

  return (httpCode == 200 && payload.length() > 0);
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi not connected (timeout).");
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED init failed. Check wiring/address.");
    while (true) delay(1000);
  }
  display.clearDisplay();
  display.display();

  dht.begin();
  connectWiFi();
}

void loop() {
  // Reconnect WiFi if dropped
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  int soilRaw = analogRead(A0);
  int soilPct = soilPercentFromRaw(soilRaw);

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  bool wifiOk = (WiFi.status() == WL_CONNECTED);
  showOnOLED(soilRaw, soilPct, t, h, wifiOk);

  Serial.print("SoilRaw=");
  Serial.print(soilRaw);
  Serial.print(" Soil%=");
  Serial.print(soilPct);
  Serial.print(" Temp=");
  Serial.print(t);
  Serial.print("C Hum=");
  Serial.print(h);
  Serial.println("%");

  // Send to ThingSpeak on interval
  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL_MS) {
    lastSend = now;
    bool ok = sendToThingSpeak(soilPct, t, h);
    Serial.println(ok ? "Upload OK" : "Upload FAILED");
  }

  delay(500);
}