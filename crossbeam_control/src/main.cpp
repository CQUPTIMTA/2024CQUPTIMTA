/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-02-01 13:16:20
 */
#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "PID.hpp"
#include "EMMC42V5.hpp"
#include "PINS.hpp"
#define GEARTEETH 30
HardwareSerial motor_ser(2);
EMMC42V5 left_motor(&motor_ser,1);
EMMC42V5 right_motor(&motor_ser,2);
float zero_point=0;
//ESP32FLASHEEPROM DATA();

float get_now_location_y() {
    int64_t m1=left_motor.read_current_location()+zero_point;
    int64_t m2=right_motor.read_current_location()-zero_point;
    m2=-m2;
    return double(m1+m2)*GEARTEETH*2.0*PI/double(65535*2);

}

void set_now_location_y(float y) {
    left_motor.angle_reset();
    right_motor.angle_reset();
    zero_point=y*65535.0/(GEARTEETH*2.0*PI);
}

void Y_load(bool is_load=true) {
    left_motor.enable(is_load);
    right_motor.enable(is_load);
}

void move_to_y(float y,float speed,float acce=0,bool need_wait=true) {
    float now=get_now_location_y();
    float delta=y-now;
    int delta_pulse=delta*65535.0/(GEARTEETH*2.0*PI);
    left_motor.pulse_control(delta_pulse,speed,acce);
    right_motor.pulse_control(-delta_pulse,speed,acce);
    if(need_wait){
        wait_to_y(y);
    }
}
void wait_to_y(float y,float delta=10) {
    while(y-get_now_location_y()>delta){
        delay(20);
    }    
}

void setup() {
    Serial.begin(115200);
    motor_ser.begin(115200, SERIAL_8N1, 10, 9);
    Serial.begin(115200);
    setup_pins();
    //attachInterrupt(digitalPinToInterrupt(LEFT_SW_PIN), left_sw_interrupt, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(RIGHT_SW_PIN), right_sw_interrupt, CHANGE);
    
    // DATA().setup();
    // DATA().read();
}

void loop() {

    if(left_motor.read_Bus_voltage()>0){
        digitalWrite(MOTOR_LED_PIN, HIGH);
    }else{
        digitalWrite(MOTOR_LED_PIN, LOW);
    }
    //Serial.print(digitalRead(RIGHT_SW_PIN));
    // Serial.print(" ");
    // Serial.println(right_motor.read_Bus_voltage());
    // Serial.println("Hello World");
    
    delay(1000);
}

