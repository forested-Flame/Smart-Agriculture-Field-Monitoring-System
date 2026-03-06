🌱 Smart Agriculture Field Monitoring System (IoT)

An IoT-based Smart Agriculture Monitoring System designed to collect real-time environmental data such as soil moisture, temperature, and humidity to help farmers make data-driven irrigation decisions and improve crop productivity.

The system uses NodeMCU (ESP8266) along with sensors to gather environmental data and uploads it to the ThingSpeak cloud platform for remote visualization and analysis. 

smart_agriculture-iot

📌 Project Overview

Traditional farming methods rely on manual observation, which can lead to inefficient irrigation and reduced crop yield. This project integrates IoT technology with agriculture to provide continuous monitoring of field conditions.

The system measures key environmental parameters and uploads them to the cloud, allowing farmers to monitor their fields remotely and analyze trends over time. 

smart_agriculture-iot

🎯 Objectives

Measure soil moisture levels using a soil moisture sensor

Monitor temperature and humidity

Display real-time readings on an OLED screen

Upload environmental data to ThingSpeak Cloud

Visualize environmental trends

Provide a foundation for crop yield prediction

🏗️ System Architecture

The system follows a 3-layer IoT architecture:

1️⃣ Hardware Layer

Sensors collect environmental data.

Components:

NodeMCU (ESP8266) – main microcontroller with WiFi

Soil Moisture Sensor (LM393 module)

DHT11 / DHT22 – temperature and humidity sensor

OLED Display (SSD1306) – real-time local display

Breadboard & Jumper wires

2️⃣ Communication Layer

WiFi (2.4 GHz)

HTTP protocol

3️⃣ Cloud Layer

ThingSpeak IoT Cloud

Data storage

Data visualization dashboards 

smart_agriculture-iot

⚙️ Working Principle

Soil moisture probe measures soil resistance.

LM393 module converts resistance to analog voltage.

NodeMCU reads analog data through A0 pin.

DHT sensor measures temperature and humidity.

Sensor readings are displayed on the OLED display.

Data is sent to ThingSpeak cloud every 20 seconds.

ThingSpeak generates graphical visualization of trends. 

smart_agriculture-iot

🔌 Circuit Connections
Soil Moisture Sensor
VCC → 3V3
GND → GND
A0  → A0 (NodeMCU)
DHT Sensor
VCC → 3V3
GND → GND
DATA → D5
OLED Display (I2C)
VCC → 3V3
GND → GND
SCL → D1
SDA → D2
🧰 Software & Tools Used

Arduino IDE

ESP8266 Board Package

Adafruit SSD1306 Library

DHT Sensor Library

ThingSpeak Cloud Platform

☁️ Cloud Integration

A ThingSpeak channel is created with the following fields:

Field	Parameter
Field 1	Soil Moisture (%)
Field 2	Temperature (°C)
Field 3	Humidity (%)

Data is uploaded using an HTTP GET request at 20-second intervals. 

smart_agriculture-iot

💻 Code (NodeMCU WiFi Setup Example)
#include <ESP8266WiFi.h>

const char* ssid = "arnav";
const char* password = "12345678";

#define LED_PIN D4

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW);
    delay(200);
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
📊 Results

Real-time soil moisture readings successfully obtained

Temperature and humidity detected accurately

Data uploaded and visualized on ThingSpeak

Stable WiFi connectivity maintained

Environmental trends observable over time 

smart_agriculture-iot

🚜 Applications

Smart irrigation systems

Greenhouse monitoring

Precision agriculture

Water conservation systems

⚠️ Limitations

Resistive soil moisture sensors may corrode over time

Automatic irrigation control is not implemented

Crop yield prediction model not fully integrated yet 

smart_agriculture-iot

🚀 Future Enhancements

Use capacitive soil moisture sensors for better durability

Add automatic irrigation using relay and water pump

Integrate machine learning for crop yield prediction

Develop a mobile dashboard application

Power the system using solar energy 

smart_agriculture-iot

👨‍💻 Team Members

Arnav Kumar Gupta

Bargauv Ganesan

Buchipalli Subhash

📜 License

This project is created for educational and research purposes.
