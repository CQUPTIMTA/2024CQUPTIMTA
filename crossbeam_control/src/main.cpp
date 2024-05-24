/*
 * @Description:
 * @Author: qingmeijiupiao
 * @Date: 2024-02-01 13:16:20
 */
#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "EMMC42V5.hpp"
#include "PINS.hpp"
#include "ESPNOW.hpp"
#define GEARTEETH 30
int ID=0;
NVSDATA DATA;

namespace CROSSBEAM {
    HardwareSerial motor_ser(2);
    EMMC42V5 left_motor(&motor_ser,1);
    EMMC42V5 right_motor(&motor_ser,2);
    float zero_point=0;
    //获取现在的Y位置,单位是mm
    float get_now_location_y() {
        int64_t m1=left_motor.read_current_location()+zero_point*65535.0/(GEARTEETH*2.0*PI);
        int64_t m2=-1*right_motor.read_current_location()+zero_point*65535.0/(GEARTEETH*2.0*PI);
        return double(m1+m2)*GEARTEETH*2.0*PI/double(65535*2);


    }
    //现在的Y位置,单位是mm
    void set_now_location_y(float y) {
        left_motor.angle_reset();
        right_motor.angle_reset();
        zero_point=y*65535.0/(GEARTEETH*2.0*PI);
    }

    //设置Y电机使能
    void Y_load(bool is_load=true) {
        left_motor.enable(is_load);
        right_motor.enable(is_load);
    }
    //等待Y轴移动完成
    void wait_to_y(float y,float delta=10) {
        while(y-get_now_location_y()>delta){
            delay(20);
        }
    }
    //Y轴移动到,单位是mm
    void move_to_y(float y,float speed,float acce=0,bool need_wait=true) {
        float now=get_now_location_y();
        float delta=y-now;
        int delta_pulse=delta*200*16/(GEARTEETH*2.0*PI);
        left_motor.pulse_control(delta_pulse,speed,acce,false,true);
        right_motor.pulse_control(-1*delta_pulse,speed,acce,false,true);
        left_motor.sync();
        if(need_wait){
            wait_to_y(y);
        }
    }

}


namespace EspnowCallback {
    void online_test(data_package redata){
        esp_now_send_package(package_type_response,redata.id,"online_test",redata.data,redata.data_len,receive_MACAddress);
    };
    void rezero(data_package redata){
        return;
    }
    void move_to_y(data_package redata){
        float y=*(float*)redata.data;
        float speed=*(float*)(redata.data+4);
        float acce=*(float*)(redata.data+8);
        CROSSBEAM::move_to_y(y,speed,acce);
        esp_now_send_package(package_type_response,redata.id,"move_to_y",nullptr,0,receive_MACAddress);
        while(abs(CROSSBEAM::get_now_location_y()-y)>10){
            delay(20);
        }
        char* sname="move_to_y";
        esp_now_send_package(package_type_normal,redata.id,"action_complete",(uint8_t*)sname,strlen(sname),receive_MACAddress);
    }
    void move_y(data_package redata){
        float delta=*(float*)redata.data;
        float speed=*(float*)(redata.data+4);
        float acce=*(float*)(redata.data+8);
        float now=CROSSBEAM::get_now_location_y();
        float y=now+delta;
        CROSSBEAM::move_to_y(y,speed,acce);
        esp_now_send_package(package_type_response,redata.id,"move_y",nullptr,0,receive_MACAddress);
        while (abs(CROSSBEAM::get_now_location_y()-y)>10){
            delay(20);
        }
        char* sname="move_y";
        esp_now_send_package(package_type_normal,redata.id,"action_complete",(uint8_t*)sname,strlen(sname),receive_MACAddress);
    }
    void get_y(data_package redata){
        float y=CROSSBEAM::get_now_location_y();
        esp_now_send_package(package_type_response,redata.id,"get_y",(uint8_t*)&y,sizeof(y),receive_MACAddress);

    }
    void enable(data_package redata){
        char name=redata.data[0];
        if(name!='Y'){
            return;
        }
        bool is_load=(bool)redata.data[1];
        CROSSBEAM::Y_load(is_load);
        esp_now_send_package(package_type_response,redata.id,"enable",nullptr,0,receive_MACAddress);
    }
    void set_zero_point(data_package redata){
        float point=*(float*)redata.data;
        DATA.Y_ZERO_POINT=point;
        DATA.write();
        esp_now_send_package(package_type_response,redata.id,"set_zero_point",nullptr,0,receive_MACAddress);
    }
    void add_callbacks(){
        callback_map["online_test"]=online_test;
        callback_map["rezero"]=rezero;
        callback_map["move_to_y"]=move_to_y;
        callback_map["move_y"]=move_y;
        callback_map["enable"]=enable;
        callback_map["set_zero_point"]=set_zero_point;
        callback_map["get_y"]=get_y;
    }
}

void setup() {
    DATA.setup();
    DATA.read();
    DATA.close();
    ID=DATA.ID;
    // DATA.ID=7;
    // DATA.write();


    esp_now_setup();
    EspnowCallback::add_callbacks();
    Serial.begin(115200);
    CROSSBEAM::motor_ser.begin(115200, SERIAL_8N1, 10, 9);
    setup_pins();
    // attachInterrupt(digitalPinToInterrupt(LEFT_SW_PIN), left_sw_interrupt, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(RIGHT_SW_PIN), right_sw_interrupt, CHANGE);
    delay(1000);


}

void loop() {
    delay(1000);
    Serial.println(ID);
}

