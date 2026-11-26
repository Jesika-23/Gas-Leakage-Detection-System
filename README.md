# Gas Leakage Detection and Alert System

An IoT-based gas leakage detector built with a **NodeMCU (ESP8266)** and an **MQ-2 gas sensor**. When gas concentration crosses a set threshold, the system sounds a buzzer, lights an LED, shows the status on an OLED display, and sends a push notification to your phone through the **Blynk IoT** app.

> Mini Project, Department of Computer Science and Engineering, R.M.D. Engineering College

## Team

- Jesika M V (Team Lead)
- Haleema Rustha B A
- Kamalasri K
- Jaya shree R
- Kavitha V
- Jayashree D

## Features

- Detects LPG, methane, butane and smoke using the MQ-2 sensor
- Local alert: buzzer and LED
- Live gas value and status (SAFE / ALERT) on a 0.96" OLED
- Remote monitoring and push notification via Blynk (gauge on virtual pin V0)
- Notification sent once per leak event (no spam), alert clears automatically when air is safe
- Credentials kept out of the source code using `secrets.h`

## Components

| Component | Purpose |
| --- | --- |
| NodeMCU (ESP8266) | Microcontroller with built-in Wi-Fi |
| MQ-2 gas sensor | Gas and smoke detection |
| OLED 0.96" (SSD1306, I2C) | Displays gas value and status |
| Buzzer | Sound alert |
| LED | Visual alert |
| Breadboard, jumper wires | Connections |

## Pin Connections

| Part | NodeMCU pin |
| --- | --- |
| MQ-2 AO | A0 |
| LED | D5 |
| Buzzer | D6 |
| OLED SDA | D2 |
| OLED SCL | D1 |
| VCC / GND | 3V3 (OLED), VIN/5V (MQ-2), GND |

## How It Works

1. MQ-2 resistance changes with gas concentration and produces an analog voltage.
2. NodeMCU reads it on A0 every second.
3. If the value is above `sensorThres`, the LED and buzzer turn on, the OLED shows `ALERT!`, and Blynk sends the event `gas_alert`.
4. When the value drops below the threshold, everything returns to SAFE.

## Setup

### 1. Libraries (Arduino IDE)

Install the ESP8266 board package, then these libraries:

- Blynk
- Adafruit GFX Library
- Adafruit SSD1306

Board: **NodeMCU 1.0 (ESP-12E Module)**

### 2. Blynk

1. Install the Blynk IoT app and create an account.
2. Create a template named `GAS LEAKAGE ALERT` (hardware: ESP8266, connection: Wi-Fi).
3. Add a datastream on **V0** (integer) and an event with code `gas_alert`.
4. Create a device from the template and copy its **Auth Token**.
5. Update `BLYNK_TEMPLATE_ID` and `BLYNK_TEMPLATE_NAME` in the `.ino` file if you use your own template.

### 3. Credentials

```
cd Arduino_Code
cp secrets.h.example secrets.h
```

Edit `secrets.h` with your Wi-Fi name, password and Blynk Auth Token.

### 4. Calibrate and upload

Let the sensor read clean air for about 5 minutes, note the highest value in the Serial Monitor (9600 baud), and set `sensorThres` 50 to 100 above it. Then upload the sketch. The MQ-2 needs a 30 second warm-up at start.

## Repository Structure

```
Gas-Leakage-Detection-System/
├── README.md
├── .gitignore
├── Arduino_Code/
│   ├── gas_leakage_detection.ino
│   └── secrets.h.example
└── Report/
    └── Gas_Leakage_Detection_Report.pdf
```

## Applications

Home kitchens, laboratories, hotels and restaurants, industrial gas storage areas, automotive gas safety.

## Future Scope

- Reduced false detection through calibration and temperature compensation
- Automatic gas valve shut-off
- Cloud log of leak events with timestamps
- Voice alerts
- Multiple sensors on a central dashboard

## Documentation

The full mini project report (abstract, introduction, components, circuit and working diagrams, software implementation, results, conclusion, references and source code) is in [`Report/Gas_Leakage_Detection_Report.pdf`](Report/Gas_Leakage_Detection_Report.pdf).

## Safety Note

This is a learning prototype. Do not rely on it as your only gas safety device.
