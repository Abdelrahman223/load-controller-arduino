# Load Controller Project

This Arduino project allows you to monitor and control electrical loads using a combination of:
- Blynk App (via ESP8266 Wi-Fi)
- PZEM004T (for voltage/current/power monitoring)
- Relays (for load switching)
- LCD (for local display)
- Overcurrent protection logic

## Features
- Control 3 electrical loads via smartphone or manually
- Monitor voltage, current, power, frequency, and power factor
- Error indicator for abnormal current
- Display readings on a 20x4 I2C LCD

## Hardware Used
- Arduino Mega
- ESP8266 Wi-Fi Module (AT commands)
- PZEM004T v3.0
- 3-channel Relay Module
- I2C LCD (20x4)
- Buzzer/LED on pin 11 (for overcurrent)

## Wiring Summary
- ESP8266: connected via Serial2 (pins 16 & 17)
- PZEM004T: connected via Serial1 (pins 18 & 19)
- Relays: pins 23, 25, 27, 29
- I2C LCD: connected via I2C (SDA/SCL)
- Overcurrent alert: pin 11

## Blynk App Pins
- V0: Relay 1
- V1: Relay 2
- V2: Relay 3
- V3–V6: Voltage, Current, Power, Frequency
- V7: Error indicator

## Setup
1. Upload the code to Arduino Mega.
2. Ensure Wi-Fi credentials are correct.
3. Create a Blynk template using provided virtual pins.
4. Use the LCD and Blynk app to monitor the system.

## License
This project is for educational and demonstration purposes.
