#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "PIN.h"
ESP32FLASHEEPROM DATA();
void setup() {
    DATA().setup();
    DATA().read();
}

void loop() {

}

