#ifndef ESP32FLASHEEPROM_HPP
#define ESP32FLASHEEPROM_HPP

#include <Arduino.h>
#include <EEPROM.h>
class ESP32FLASHEEPROM{
    public:
    ESP32FLASHEEPROM(){};
    ~ESP32FLASHEEPROM(){};
    void setup(){
        EEPROM.begin(4096);
    };
    void read(){
        uint8_t temp[4096];
        for (int i = 0; i < 4096; i++){
            temp[i] = EEPROM.read(i);
        }
        memcpy(this, temp, sizeof(ESP32FLASHEEPROM));
        EEPROM.end();
    };
    void write(){
        uint8_t temp[4096];
        memcpy(temp, this, sizeof(ESP32FLASHEEPROM));
        for (int i = 0; i < 4096; i++){
            EEPROM.write(i, temp[i]);
        }
        EEPROM.commit();
        EEPROM.end();
    }
    uint8_t ID=0;

};

#endif