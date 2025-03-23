#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Blynk Authentication Token
char auth[] = "token";
// Wi-Fi credentials
char ssid[] = "bingun";
char pass[] = "Bing@0715";

// Sensor and Pin Configurations
#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN 34
#define LDR_PIN 35
#define RELAY_PIN 26
#define NPK_PIN 32 // NPK Sensor Analog Pin

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensorData() {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    int lightLevel = analogRead(LDR_PIN);
    int npkValue = analogRead(NPK_PIN); // Read NPK sensor value

    // Send data to Blynk
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, hum);
    Blynk.virtualWrite(V2, soilMoisture);
    Blynk.virtualWrite(V3, lightLevel);
    Blynk.virtualWrite(V5, npkValue); // Send NPK data

    // Automated Irrigation Control
    if (soilMoisture < 800) { // Adjust threshold
        digitalWrite(RELAY_PIN, LOW); // Turn on water pump
        Blynk.virtualWrite(V4, "Watering: ON");
    } else {
        digitalWrite(RELAY_PIN, HIGH); // Turn off water pump
        Blynk.virtualWrite(V4, "Watering: OFF");
    }
}

void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    dht.begin();
    pinMode(SOIL_MOISTURE_PIN, INPUT);
    pinMode(LDR_PIN, INPUT);
    pinMode(NPK_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);
    
    timer.setInterval(2000L, sendSensorData);
}

void loop() {
    Blynk.run();
    timer.run();
}

