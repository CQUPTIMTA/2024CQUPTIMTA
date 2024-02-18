#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "SENSOR.hpp"

ESP32FLASHEEPROM DATA;


void setup() {
  DATA.setup();
  DATA.read();
}

void loop() {

}

