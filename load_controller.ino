// === BLYNK CONFIGURATION ===
#define BLYNK_TEMPLATE_ID "TMPL6o_5lx9KW"
#define BLYNK_TEMPLATE_NAME "Load Controller"
#define BLYNK_AUTH_TOKEN "ECfD98FugWIAL7NEXu7X5yRbwuJhR6Z7"
#define BLYNK_PRINT Serial

// === LIBRARIES ===
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === WIFI CONFIGURATION ===
char ssid[] = "Creative Vision";
char pass[] = "ibda3vision";

// === SERIAL & MODULE SETUP ===
#define EspSerial Serial2
#define ESP8266_BAUD 115200
ESP8266 wifi(&EspSerial);

// === OBJECT DECLARATIONS ===
PZEM004Tv30 pzem(Serial1);                   // PZEM004T power sensor on Serial1
LiquidCrystal_I2C lcd(0x27, 20, 4);          // I2C LCD 20x4

// === CONTROL VARIABLES ===
String state = "Normal";

// === BLYNK VIRTUAL PIN CALLBACKS ===
BLYNK_WRITE(V0) { digitalWrite(25, !param.asInt()); }  // Relay 1
BLYNK_WRITE(V1) { digitalWrite(29, !param.asInt()); }  // Relay 2
BLYNK_WRITE(V2) { digitalWrite(27, !param.asInt()); }  // Relay 3

void setup() {
  Serial.begin(115200);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  // Relay outputs
  pinMode(23, OUTPUT); digitalWrite(23, HIGH);
  pinMode(25, OUTPUT); digitalWrite(25, HIGH);
  pinMode(27, OUTPUT); digitalWrite(27, HIGH);
  pinMode(29, OUTPUT); digitalWrite(29, HIGH);

  // Debug/Error output (e.g. buzzer or LED)
  pinMode(11, OUTPUT);

  // Initialize Blynk and LCD
  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0); lcd.print("System Start");
}

void loop() {
  Blynk.run();

  // Read sensor values
  float voltage   = pzem.voltage();
  float current   = pzem.current();
  float power     = pzem.power();
  float energy    = pzem.energy();
  float frequency = pzem.frequency();
  float pf        = pzem.pf();

  // Debug output
  Serial.print("Custom Address: "); Serial.println(pzem.readAddress(), HEX);
  if (isnan(voltage) || isnan(current) || isnan(power) ||
      isnan(energy) || isnan(frequency) || isnan(pf)) {
    Serial.println("Sensor read error");
  } else {
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
    Serial.print("Current: "); Serial.print(current); Serial.println(" A");
    Serial.print("Power: ");   Serial.print(power);   Serial.println(" W");
    Serial.print("Energy: ");  Serial.print(energy);  Serial.println(" kWh");
    Serial.print("Freq: ");    Serial.print(frequency); Serial.println(" Hz");
    Serial.print("PF: ");      Serial.println(pf);
  }

  // Display on LCD
  lcd.clear();
  lcd.setCursor(2, 0); lcd.print("Load Controller");
  lcd.setCursor(0, 1); lcd.print("V: "); lcd.print(voltage, 1); lcd.print("V");
  lcd.setCursor(10, 1); lcd.print("I: "); lcd.print(current); lcd.print("A");
  lcd.setCursor(0, 2); lcd.print("P: "); lcd.print(power, 1); lcd.print("W");
  lcd.setCursor(10, 2); lcd.print("F: "); lcd.print(frequency, 1); lcd.print("Hz");
  lcd.setCursor(2, 3); lcd.print("State: "); lcd.print(state);

  // Send to Blynk App
  Blynk.virtualWrite(V3, voltage);
  Blynk.virtualWrite(V4, current);
  Blynk.virtualWrite(V5, power);
  Blynk.virtualWrite(V6, frequency);

  // Overcurrent Protection
  if (current > 0.2) {
    digitalWrite(11, HIGH);
    Blynk.virtualWrite(V7, 1);  // Indicate error on app
    state = "Error";
  } else {
    digitalWrite(11, LOW);
    Blynk.virtualWrite(V7, 0);  // Indicate normal state
    state = "Normal";
  }

  delay(2000);
}
