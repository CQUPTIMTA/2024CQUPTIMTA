/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-02-01 13:16:20
 */
#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "EMMC42V5.hpp"
#include "PINS.hpp"
#define GEARTEETH 30

//ESP32FLASHEEPROM DATA();

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


void setup() {
    Serial.begin(115200);
    CROSSBEAM::motor_ser.begin(115200, SERIAL_8N1, 10, 9);
    setup_pins();
    attachInterrupt(digitalPinToInterrupt(LEFT_SW_PIN), left_sw_interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_SW_PIN), right_sw_interrupt, CHANGE);
    delay(1000);
    CROSSBEAM::Y_load(false);
    //CROSSBEAM::move_to_y(CROSSBEAM::get_now_location_y()+600,100,200);
    // DATA().setup();
    // DATA().read();
}

void loop() {

    Serial.println(CROSSBEAM::get_now_location_y());

    //Serial.print(digitalRead(RIGHT_SW_PIN));
    // Serial.print(" ");
    // Serial.println(right_motor.read_Bus_voltage());
    // Serial.println("Hello World");
    
    delay(1000);
}

