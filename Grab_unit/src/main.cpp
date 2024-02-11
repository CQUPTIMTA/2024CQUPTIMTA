#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
ESP32FLASHEEPROM DATA;
#include "SENSOR.hpp"

void setup() {
  DATA.setup();
  DATA.read();
}

void loop() {

}

