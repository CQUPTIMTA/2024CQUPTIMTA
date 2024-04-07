#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "SENSOR.hpp"
#include "EMMC42V5.hpp"
#include "HEServo.hpp"
#include "PID.hpp"
#define laser_pin 42
// //高度超声波
SENSOR high_sensor(37,38,true,10);
// // X轴和Z轴超声波
SENSOR Y_sensor(14,21,true,10);
SENSOR X_sensor(47,48,true,10);
//电机串口
HardwareSerial motor_ser=HardwareSerial(2);
//舵机串口
HardwareSerial servo_ser=HardwareSerial(1);

ESP32FLASHEEPROM DATA;

EMMC42V5 X_motor(&motor_ser,2);
EMMC42V5 Z_motor(&motor_ser,1);
//夹爪舵机
HEServo grap_servo(&servo_ser,1);
HEServo X_servo(&servo_ser,2);
HEServo Y_servo(&servo_ser,3);

void set_high(float high_mm){
  PID high_pid(1*200*256,0,0);
  float error_h=high_mm-high_sensor.get_distance_mm(false);
  while(error_h>1){
    float output_h=high_pid.control(high_mm,error_h);
    error_h=high_mm-high_sensor.get_distance_mm(false);
    X_motor.speed_control(-output_h,0);
  }
  X_motor.speed_control(0,0);
}

void rezero(){
  X_motor.speed_control(60,0);
  while(!digitalRead(11)){
    delay(1);
  }
  X_motor.pulse_control(-256*1,30);
  delay(1000);
  X_motor.speed_control(3,0);
  while(!digitalRead(11)){
    delay(1);
  }
  X_motor.speed_control(0,0);
  X_motor.angle_reset();

}

float get_now_x_location(){
    return 40*PI*X_motor.read_input_pulses()/(16*200);
}
int64_t location_to_pluse(float location){
    return location*16*200/float(40*PI);
}
void update_sensor(){
  high_sensor.update();
  X_sensor.update();
  Y_sensor.update();
  delay(5);
}


void setup() {
  motor_ser.begin(115200,SERIAL_8N1,10,9);
  servo_ser.begin(115200,SERIAL_8N1,35,36);
  //servo_ser.begin(115200);
  high_sensor.setup();
  X_sensor.setup();
  Y_sensor.setup();
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(11,INPUT_PULLDOWN);
  pinMode(laser_pin,OUTPUT_OPEN_DRAIN);
  digitalWrite(laser_pin,0);

  Serial.begin(115200);
  //delay(2000);
  //rezero();
  //delay(5000);
  //X_motor.pulse_control(-1*location_to_pluse(800),1000,240);
  DATA.setup();
  DATA.read();
}

void loop() {
  Serial.println(grap_servo.SERVO_POS_READ());
  delay(500);
  //Serial.println(Z_motor.read_Bus_voltage());

  // Serial.print(high_sensor.get_distance_mm());
  // Serial.print(",");
  // Serial.print(X_sensor.get_distance_mm());
  // Serial.print(",");
  // Serial.print(Y_sensor.get_distance_mm());
  // Serial.print(",");
  // Serial.println(digitalRead(11));
}

