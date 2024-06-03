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
        float Front = frontSensor.get_distance_mm(false);
        float Back = backSensor.get_distance_mm(false);
        uint8_t data[8];
        memcpy(data, &Front, sizeof(Front));
        memcpy(data+4, &Back, sizeof(Back));
        esp_now_send_package(package_type_response, ID, "get_sensor_distance", data,8);
    }
}

void get_voltage(data_package Rdata){
    float voltage = 2*analogReadMilliVolts(4)/1000.0;
    esp_now_send_package(package_type_response, ID, "get_voltage",(uint8_t *)&voltage,4);
}

void online_test(data_package redata){
    esp_now_send_package(package_type_response,redata.id,"online_test",nullptr,0,receive_MACAddress);
};

void setup() {
    DATA.setup();
    DATA.read();
    ID=DATA.ID;
    // DATA.ID=11;
    // DATA.write();
    Serial.begin(115200);
    frontSensor.setup();
    backSensor.setup();
    pinMode(1,OUTPUT);
    digitalWrite(1,0);
    callback_map["get_sensor_distance"]=get_sensor_distance;
    callback_map["get_voltage"]=get_voltage;
    callback_map["online_test"]=online_test;
    esp_now_setup();
    

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
    Serial.println(2*analogReadMilliVolts(4));
    // Serial.println(ID);
    // digitalWrite(1,1);
    // delay(300);
    // digitalWrite(1,0);
    delay(200);
}
