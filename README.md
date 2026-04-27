# ESP8266 IoT Environment Monitor (DHT & OLED)
**By [Idlan Zafran Mohd Zaidie](https://github.com/IdlanZafran)**

[![Follow](https://img.shields.io/github/followers/IdlanZafran?label=Follow%20Me&style=social)](https://github.com/IdlanZafran)

[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/IdlanZafran/RapidBootWiFi)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

_If you like this project, don’t forget to:_
- _⭐ Star this repository_
- _👤 Follow my GitHub for future updates_

## 📌 Overview
A robust, public-safe ESP8266-based IoT sensor node that monitors temperature and humidity using a DHT11 sensor, displays real-time data on an SSD1306 OLED screen, and pushes telemetry to the [ThingsSentral](http://thingssentral.io) dashboard. 

This project features a built-in captive portal using `RapidBootWiFi`, allowing users to configure WiFi credentials and IoT parameters dynamically without hardcoding sensitive data.

## 🚀 Features

* **Real-Time Local Display:** View temperature (°C) and relative humidity (%) directly on a 128x64 OLED screen.
* **Dynamic WiFi & IoT Provisioning:** Uses `RapidBootWiFi` to broadcast a captive portal (Access Point: `PokeboxDHT`) for easy headless setup of WiFi credentials and ThingsSentral routing IDs.
* **Cloud Integration:** Automatically pushes telemetry data to the ThingsSentral IoT platform.
* **Safe for Public Version Control:** The codebase uses placeholders for sensitive IoT IDs, ensuring your private dashboard endpoints remain secure.
* **Auto-Reconnection:** Built-in logic to handle offline states and automatic WiFi reconnections.

## 🛠️ Hardware Requirements

* **Microcontroller:** ESP8266 (e.g., NodeMCU V3, Wemos D1 Mini)
* **Sensor:** DHT11 (or DHT22 with minor code adjustments) Temperature & Humidity Sensor
* **Display:** 0.96" I2C SSD1306 OLED Display (128x64)
* Jumper wires and a breadboard

## 🔌 Wiring & Pinout

| Component | ESP8266 Pin | Notes |
| :--- | :--- | :--- |
| **OLED SCL** | `D1` (GPIO5) | I2C Clock |
| **OLED SDA** | `D2` (GPIO4) | I2C Data |
| **OLED VCC** | `3.3V` | Power |
| **OLED GND** | `GND` | Ground |
| **DHT Data** | `D7` | Digital Data Pin |
| **DHT VCC** | `3.3V` or `5V` | Power (check your DHT module specs) |
| **DHT GND** | `GND` | Ground |

## 📦 Software Dependencies

Ensure you have the ESP8266 board package installed in your Arduino IDE. You will also need to install the following libraries via the Arduino Library Manager (`Sketch > Include Library > Manage Libraries`):

* `ESP8266WiFi` (Built-in with ESP8266 board package)
* `RapidBootWiFi`
* `ThingsSentral`
* `DHT sensor library` by Adafruit
* `Adafruit GFX Library`
* `Adafruit SSD1306`

## ⚙️ Setup & Installation

1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/YOUR_USERNAME/esp8266-iot-environment-monitor-dht-oled.git](https://github.com/YOUR_USERNAME/esp8266-iot-environment-monitor-dht-oled.git)
    ```
2.  **Open the Project:** Open the `.ino` file in the Arduino IDE.
3.  **Local Configuration (Optional but Recommended):** In the code, locate the placeholder IDs:
    ```cpp
    const char DEFAULT_USER_ID[] = "YOUR_USER_ID";               
    const char DEFAULT_HUMIDITY_ID[] = "YOUR_HUMIDITY_ID";       
    const char DEFAULT_TEMPERATURE_ID[] = "YOUR_TEMPERATURE_ID"; 
    ```
    *Note: You can update these locally for your personal device, but **do not commit your real IDs back to GitHub**.*
4.  **Compile and Upload:** Connect your ESP8266 to your computer, select the correct COM port and Board type, and click Upload.

## ⚙️ Setup & Installation (Arduino IDE)

To get this project running on your ESP8266, follow these precise steps:

### 1. Prepare the Folder Structure
Arduino IDE requires the `.ino` file to be inside a folder of the exact same name. 
* Create a folder named `esp8266-iot-environment-monitor-dht-oled`.
* Place your main script inside it and rename it to `esp8266-iot-environment-monitor-dht-oled.ino`.
* **Path:** `Documents/Arduino/esp8266-iot-environment-monitor-dht-oled/esp8266-iot-environment-monitor-dht-oled.ino`

### 2. Install the ESP8266 Board Core
If you haven't used ESP8266 with Arduino before:
1. Go to **File > Preferences**.
2. In "Additional Boards Manager URLs", paste: 
   `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. Go to **Tools > Board > Boards Manager**, search for **ESP8266**, and click **Install**.

### 3. Install Required Libraries
Open the **Library Manager** (**Sketch > Include Library > Manage Libraries**) and install:
* **DHT sensor library** (by Adafruit) — *Note: Also install "Adafruit Unified Sensor" if prompted.*
* **Adafruit SSD1306** (by Adafruit)
* **Adafruit GFX Library** (by Adafruit)
* **RapidBootWiFi** (Search and install)
* **ThingsSentral** (Search and install)



### 4. Configure & Upload
1. **Open the file:** Double-click `esp8266-iot-environment-monitor-dht-oled.ino`.
2. **Select Board:** Go to **Tools > Board > ESP8266 Boards** and select **NodeMCU 1.0 (ESP-12E Module)** or **Generic ESP8266**.
3. **Select Port:** Go to **Tools > Port** and select the COM port your device is plugged into.
4. **Update IDs (Optional):** You can edit these placeholders in the code, or leave them as is and use the **Captive Portal** later:
   ```cpp
   const char DEFAULT_USER_ID[] = "YOUR_USER_ID";
   const char DEFAULT_HUMIDITY_ID[] = "YOUR_HUMIDITY_ID";
   const char DEFAULT_TEMPERATURE_ID[] = "YOUR_TEMPERATURE_ID";
   ```
5. **Upload:** Click the **Upload** button (Right Arrow icon) in the top left.

## 📱 Usage & Captive Portal Configuration

Upon the first boot (or if the ESP8266 cannot find a known WiFi network), it will enter Access Point mode.

1.  Look at the OLED screen. It will instruct you to connect to the AP.
2.  On your phone or computer, connect to the WiFi network named: **`PokeboxDHT`**
3.  A captive portal should automatically open. If it doesn't, navigate to `192.168.4.1` in your web browser.
4.  Enter your local WiFi SSID and Password.
5.  Scroll down to the **Custom Parameters** section and enter your:
    * **ThingsSentral User ID**
    * **Humidity Target ID**
    * **Temperature Target ID**
6.  Save the settings. The ESP8266 will restart, connect to your WiFi, display the "CONNECTED" status on the OLED, and begin pushing data to ThingsSentral!

## 📄 License

This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details.


