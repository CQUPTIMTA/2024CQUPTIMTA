/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-27 15:06:30
 */
#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "SENSOR.hpp"
#include "ESPNOW.hpp"
ESP32FLASHEEPROM DATA;
SENSOR frontSensor(2, 3, true, 3);
SENSOR backSensor(6, 7, true, 3);
int ID=0;
void get_sensor_distance(data_package Rdata){
    char name =Rdata.data[0];
    if(name=='S'){
        float Front = frontSensor.get_distance_mm();
        float Back = backSensor.get_distance_mm();
        uint8_t data[8];
        memcpy(data, &Front, sizeof(Front));
        memcpy(data+4, &Back, sizeof(Back));
        esp_now_send_package(package_type_response, ID, "get_sensor_distance", data,8);
    }
}


void setup() {
    DATA.setup();
    DATA.read();
    ID=DATA.ID;
    Serial.begin(115200);
    frontSensor.setup();
    backSensor.setup();
    esp_now_setup();
    callback_map["get_sensor_distance"]=get_sensor_distance;
}

void loop() {

    for(int i=0;i<3;i++){
        frontSensor.update();
        delay(10);
        backSensor.update();
        delay(10);
    }
    Serial.print(frontSensor.get_distance_mm());
    Serial.print(" ");
    Serial.println(backSensor.get_distance_mm());
    delay(1000);
}
