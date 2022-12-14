#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Configuration
uint16_t motor_delay = 50;
// HC-05
uint8_t rx_pin = 10;
uint8_t tx_pin = 11;
uint8_t key_pin = 9;
bool at_mode = HIGH;

// Init globals
SoftwareSerial BTSerial(rx_pin, tx_pin);
DynamicJsonDocument doc(1024);
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor* ms[4];
String buffer = "";

void setup() {
  pinMode(key_pin, OUTPUT);

  digitalWrite(key_pin, at_mode);

  for(uint8_t i = 0; i < 4; i++) {
    ms[i] = AFMS.getMotor(i + 1);
  }

  Serial.begin(9600);
  BTSerial.begin(38400);
  AFMS.begin();
}

void loop() {
  if (!BTSerial.available())
    return;
  
  char ch = BTSerial.read();

  if (ch != '\n') {
    buffer += ch;
    return;
  }

  deserializeJson(doc, buffer);
  buffer = "";
  
  int16_t x_joy = doc["x"];
  int16_t y_joy = doc["y"];
  bool sw = doc["sw"];
  // Serial.println("x: " + String(x_joy) + " y:" + String(y_joy) + " sw: " + String(sw));

  uint16_t dir = FORWARD;
  uint16_t speed = min(abs(y_joy - 256), 255);
  uint16_t left = 1.0;
  uint16_t right = 1.0;

  if(y_joy < 256)
    dir = BACKWARD;
  if(x_joy > 256 + 10)
    left = 0.5;
  else if(x_joy < 256 - 10)
    right = 0.5;
    
  for(uint8_t i = 0; i < 2; i++) {
    ms[i]->setSpeed(speed * right);
    ms[i]->run(dir);
  }
  for(uint8_t i = 2; i < 4; i++) {
    ms[i]->setSpeed(speed * left);
    ms[i]->run(dir);
  }
}
