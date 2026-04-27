/*
 * @author: Idlan Zafran Mohd Zaidie
 * Date: 2026
 * @license MIT
 * Description: ESP8266 DHT Sensor node pushing data to ThingsSentral.
 * Includes OLED display support and captive portal WiFi setup.
 */

#include <ESP8266WiFi.h>
#include <RapidBootWiFi.h>
#include <ThingsSentral.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "syokcircuitlogo.h"

#define SCL_PIN D1 // GPIO5
#define SDA_PIN D2 // GPIO4

#define DHT_SENSOR_PIN  D7 
#define DHT_SENSOR_TYPE DHT11

const char DEFAULT_SERVER_URL[] = "http://thingssentral.io"; 
const char DEFAULT_USER_ID[] = "YOUR_USER_ID";               // e.g., "00123"
const char DEFAULT_HUMIDITY_ID[] = "YOUR_HUMIDITY_ID";       // e.g., "0012345678902"
const char DEFAULT_TEMPERATURE_ID[] = "YOUR_TEMPERATURE_ID"; // e.g., "0012345678901"
// =========================================================================

const char* WIFI_NAME = "DHT Sensor";

#define SCREEN_WIDTH 128    // Physical width
#define SCREEN_HEIGHT 64    // Physical height
#define SCREEN_ADDRESS 0x3C 
#define OLED_RESET -1       

float humidity;
float temperature;

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

unsigned long previousMillis = 0;  // stores the last time DHT was read
const long interval = 2000;        // read interval

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LOGICAL_WIDTH = SCREEN_WIDTH;  // 128 pixels
const int LOGICAL_HEIGHT = SCREEN_HEIGHT; // 64 pixels


void sendData(float humidity, float temperature) {
  // Retrieve target IDs dynamically
  String humId = String(myWiFi.getParameterValue("humidity"));
  String tempId = String(myWiFi.getParameterValue("temperature"));

  // Using ThingsSentral Command module to push telemetry. 
  Serial.println("Pushing Humidity...");
  bool successHum = TS.Command.send(humId, String(humidity, 2));
  
  Serial.println("Pushing Temperature...");
  bool successTemp = TS.Command.send(tempId, String(temperature, 2));

  if (successHum && successTemp) {
      Serial.println("Data pushed successfully to ThingsSentral.");
  } else {
      Serial.println("Failed to push data.");
  }
}

void displaySensorData(float humidity, float temperature) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);

  // CHECK WIFI STATUS
  if (WiFi.status() != WL_CONNECTED) {
    // DISPLAY THIS IF DISCONNECTED
    drawCenteredText("WiFi", 20, 2);
    drawCenteredText("Disconnected", 40, 1);
    drawCenteredText("Reconnecting...", 55, 1);
  } 
  else {
    // Top Header
    drawCenteredText("DigitalUKM", 4, 1);
    
    // UI Layout Lines
    display.drawLine(0, 16, SCREEN_WIDTH - 1, 16, SSD1306_WHITE);     // Horizontal separator
    display.drawLine(64, 16, 64, SCREEN_HEIGHT - 1, SSD1306_WHITE);   // Vertical divider

    int16_t x1, y1;
    uint16_t w, h;
    
    // --- Labels Row (Size 1) ---
    display.setTextSize(1);
    
    // Left half: Temp Label
    display.getTextBounds("Temp (C)", 0, 0, &x1, &y1, &w, &h);
    display.setCursor(32 - (w / 2), 22); // Center of left side (32) minus half width
    display.print("Temp (C)");

    // Right half: RH Label
    display.getTextBounds("RH (%)", 0, 0, &x1, &y1, &w, &h);
    display.setCursor(96 - (w / 2), 22); // Center of right side (96) minus half width
    display.print("RH (%)");

    // --- Values Row (Size 2) ---
    display.setTextSize(2);
    
    // Left half: Temp Data
    char tempStr[10];
    dtostrf(temperature, 4, 1, tempStr);
    display.getTextBounds(tempStr, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(32 - (w / 2), 40);
    display.print(tempStr);

    // Right half: Humidity Data
    char humStr[10];
    dtostrf(humidity, 3, 0, humStr); // Humidity usually has fewer digits
    display.getTextBounds(humStr, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(96 - (w / 2), 40);
    display.print(humStr);
  }
  
  display.display();
}

void drawCenteredText(const char* text, int yPos, int size) {
  display.setTextSize(size);
  int16_t x1, y1;
  uint16_t w, h;

  // Get the bounding box of the text based on its size
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  
  // Calculate X position for horizontal centering
  int xPos = (LOGICAL_WIDTH - w) / 2;
  
  // Set the cursor and print
  display.setCursor(xPos, yPos); 
  display.print(text);
}

void setup() {
  WiFi.mode(WIFI_STA); 
  Serial.begin(115200);
  dht_sensor.begin(); 
  Wire.begin(SDA_PIN, SCL_PIN); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 initialization failed."));
    for(;;); 
  }
  display.setRotation(0); 

  // --- 1. RAPID BOOT INITIALIZATION ---
  myWiFi.setAPName(WIFI_NAME);
  myWiFi.begin(); // Handles LittleFS mount, reads/increments boot count, handles Factory/WiFi resets

  int cycle = myWiFi.getCurrentBootCount();
  Serial.println("Boot cycle = " + String(cycle));

  // Display Boot Cycle momentarily
  display.clearDisplay();
  char cycleStr[10];
  sprintf(cycleStr, "%d", cycle);
  drawCenteredText(cycleStr, 40, 3);
  display.display();
  delay(500);
  
  // Show Bitmap
  display.clearDisplay();
  display.drawBitmap(0, 0, epd_bitmap_Type_4, 128, 64, SSD1306_WHITE);
  display.display();
  delay(1500); 

  // --- 2. SHOW INSTRUCTIONS (Before Connect) ---
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  drawCenteredText("Connect to AP:", 15, 1);
  drawCenteredText(WIFI_NAME, 35, 1); // Shows "PokeboxDHT"
  drawCenteredText("to Setup WiFi", 50, 1);
  display.display();
  delay(2000); // Give user time to read

  // If threshold hit, RapidBoot WiFi reset settings. 
  if (myWiFi.wasWiFiReset()) {
    display.clearDisplay();
    drawCenteredText("Resetting", 60, 1);
    display.display();
    delay(1000);
    display.clearDisplay();
    drawCenteredText("Connect", 50, 1);
    drawCenteredText("Wifi", 65, 1);
    display.display();
  }

  // --- 3. CUSTOM PARAMETERS ---
  myWiFi.addParameter("server", "Server URL", DEFAULT_SERVER_URL, 100);
  myWiFi.addParameter("user", "User ID", DEFAULT_USER_ID, 20); // Increased buffer size to handle longer placeholder text
  myWiFi.addParameter("humidity", "Humidity ID", DEFAULT_HUMIDITY_ID, 30); // Increased buffer size
  myWiFi.addParameter("temperature", "Temperature ID", DEFAULT_TEMPERATURE_ID, 30); // Increased buffer size

  // --- 4. ATTEMPT CONNECTION ---
  // This blocks until connected or timeout. If no known network, sets up portal and waits.
  myWiFi.connect();

  // --- 5. SHOW CONNECTED STATUS (After Connect) ---
  if (WiFi.status() == WL_CONNECTED) {
      display.clearDisplay();
      drawCenteredText("CONNECTED", 30, 2);
      display.display();
      delay(2000);
  }

  // --- 6. INITIALIZE THINGS SENTRAL ---
  // Retrieve saved parameters and start the TS engine
  String tsServerURL = String(myWiFi.getParameterValue("server"));
  String tsUserID = String(myWiFi.getParameterValue("user"));
  
  TS.begin(tsUserID, tsServerURL);
  Serial.println("Connected. ThingsSentral Engine Started!");
}

//*******************************************************

void loop() {
  // Let RapidBoot handle automatic offline reconnect logic
  myWiFi.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    humidity = dht_sensor.readHumidity();
    temperature = dht_sensor.readTemperature();
    
    displaySensorData(humidity, temperature);

    if (TS.isOnline()) {
      sendData(humidity, temperature);
    } else {
      Serial.println("Wi-Fi not connected. Skipping upload.");
    }
  }
}