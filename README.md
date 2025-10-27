# Smart_Plant_Watering_System_Using_ESP32_and_Blynk

🔍 1. Project Overview

  This project is designed to automatically water plants depending on the soil’s moisture level.
Using an ESP32 microcontroller, a capacitive or resistive soil moisture sensor, and three water pumps controlled by relays, the system ensures your plants receive the right amount of water.

You can:

  * Monitor real-time soil moisture from anywhere.
  * Switch between Automatic (sensor-based) and Manual (app-based) watering.
  * Control each plant’s pump independently via Blynk IoT mobile app.
  * View live updates of the moisture level on a gauge.
    
This system is ideal for:

  * 🌿 Home gardens
  * 🪴 Greenhouses
  * 🌾 Small farms
  * 🌻 Indoor plant care

⚙️ 2. Hardware Components

| Component                       | Quantity  | Function                             |
| ------------------------------- | --------- | ------------------------------------ |
| **ESP32 Dev Board**             | 1         | Controls relays & reads sensor data  |
| **Soil Moisture Sensor**        | 1         | Detects soil wetness (analog output) |
| **Relay Module (4-channel)**    | 1         | Switches 12V pumps using 3 channels  |
| **Mini DC Water Pump (5V/12V)** | 3         | Waters plants automatically          |
| **Power Supply (5V/12V)**       | 1         | Powers ESP32, relays, and pumps      |
| **Jumper Wires**                | As needed | For connections                      |
| **Blynk IoT App**               | 1         | Smartphone-based remote control      |

🔌 3. Pin Connections

ESP32 Pin Mapping

| ESP32 Pin     | Connected Device      | Function        |
| ------------- | --------------------- | --------------- |
| **GPIO 34**   | Moisture Sensor OUT   | Analog input    |
| **GPIO 25**   | Relay 1 (IN1)         | Controls Pump 1 |
| **GPIO 26**   | Relay 2 (IN2)         | Controls Pump 2 |
| **GPIO 27**   | Relay 3 (IN3)         | Controls Pump 3 |
| **3.3V / 5V** | Sensor VCC            | Power source    |
| **GND**       | All grounds connected | Common ground   |

⚠️ Important Notes:

  * The relay module should be powered from 5V (not from the 3.3V pin).
  * Connect ESP32 GND and Relay GND together for proper switching.
  * Pumps are connected through the relay’s NO (Normally Open) terminal.

💡 4. How the Circuit Works

  1. The Soil Moisture Sensor produces a voltage based on soil wetness:
      * Dry soil: Higher resistance → Lower voltage.
      * Wet soil: Lower resistance → Higher voltage.
  2. The ESP32 analog input (GPIO 34) reads this voltage (0–3.3V range).
  3. The analog reading (0–4095) is converted to a percentage (0–100%).
  4. Depending on this percentage:
      * If the value is below a defined threshold, the ESP32 activates the corresponding relay (turning ON the pump).
      * Once soil moisture reaches the threshold, the pump automatically stops.
  5. In Manual mode, you can directly toggle pumps from the Blynk IoT mobile dashboard, overriding automatic control.

🌐 5. Blynk IoT Setup (Cloud + Mobile App)

Step 1 — Create a New Template

  1. Open the Blynk IoT App or Blynk Web Dashboard (https://blynk.cloud).
  2. Create a New Template:
      * Template Name: Plant Watering
      * Hardware: ESP32
      * Connection: WiFi
  3. Copy the BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME, and BLYNK_AUTH_TOKEN.
     
Step 2 — Add Data Streams (Virtual Pins)

| Name     | Virtual Pin | Type   | Unit |
| -------- | ----------- | ------ | ---- |
| Mode     | V0          | Switch | —    |
| Pump1    | V1          | Switch | —    |
| Pump2    | V2          | Switch | —    |
| Pump3    | V3          | Switch | —    |
| Moisture | V5          | Gauge  | %    |

Step 3 — App Dashboard Widgets

| Widget        | Virtual Pin | Settings               |
| ------------- | ----------- | ---------------------- |
| Switch (Mode) | V0          | Label: “Auto / Manual” |
| Button        | V1          | Pump 1                 |
| Button        | V2          | Pump 2                 |
| Button        | V3          | Pump 3                 |
| Gauge         | V5          | Displays moisture %    |

👉 All buttons should be in Switch Mode (not Push).

🧩 6. Troubleshooting

| Problem                      | Possible Cause                   | Solution                                         |
| ---------------------------- | -------------------------------- | ------------------------------------------------ |
| ESP32 not connecting to WiFi | Wrong SSID or password           | Check your Wi-Fi details                         |
| Pumps not turning ON         | Relay logic inverted             | Try swapping `HIGH` ↔ `LOW`                      |
| Blynk app not updating       | Wrong virtual pins               | Verify V0–V5 mappings                            |
| Moisture readings unstable   | Power fluctuation / interference | Use smoothing or add a capacitor near the sensor |
| Moisture always 0%           | Wrong mapping range              | Recalibrate your sensor                          |

































