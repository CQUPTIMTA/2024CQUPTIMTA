#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
ESP32FLASHEEPROM flash_data();
#include "SENSOR.hpp"
SENSOR font_sensor(15, 2);
SENSOR back_sensor(15, 2);

void setup() {
  flash_data.setup();
  flash_data.read();
  font_sensor.setup();
  back_sensor.setup();
}

void loop() {

}

