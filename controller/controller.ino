#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// Configuration
uint16_t send_delay = 50;
// HC-05
uint8_t rx_pin = 10;
uint8_t tx_pin = 11;
uint8_t key_pin = 9;
bool at_mode = HIGH;
// Joystick
uint8_t sw_pin = 8;
uint8_t x_pin = A0;
uint8_t y_pin = A1;

// Init globals
SoftwareSerial BTSerial(rx_pin, tx_pin);
DynamicJsonDocument doc(1024);

void setup()
{
  pinMode(key_pin, OUTPUT);
  pinMode(x_pin, INPUT);
  pinMode(y_pin, INPUT);
  pinMode(sw_pin, INPUT_PULLUP);

  digitalWrite(key_pin, at_mode);
  Serial.begin(9600);
  BTSerial.begin(38400);
}

void loop()
{
  int x_joy = map(analogRead(x_pin), 0, 1023, 0, 512);
  int y_joy = map(analogRead(y_pin), 0, 1023, 0, 512);
  bool sw_joy = digitalRead(sw_pin);

  doc["x"] = x_joy;
  doc["y"] = y_joy;
  doc["sw"] = (uint8_t)!sw_joy;

  serializeJson(doc, BTSerial);
  BTSerial.println();
  delay(send_delay);
}