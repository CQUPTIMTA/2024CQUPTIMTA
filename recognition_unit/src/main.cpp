#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
ESP32FLASHEEPROM flash_data;
#include "SENSOR.hpp"
SENSOR font_sensor(15, 2);
SENSOR back_sensor(15, 2);

void setup() {
  flash_data.setup();//初始化
  flash_data.read();//读取
  font_sensor.setup();
  back_sensor.setup();
  delay(1000);
}

void loop() {
}

