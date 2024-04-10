//#include <Arduino.h>
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

//高度闭环控制
void set_high(float high_mm){
  PID high_pid(6,0.001,0.001);
  float error_h=high_mm-high_sensor.get_distance_mm();
  
  while(abs(error_h)>1){
    error_h=high_mm-high_sensor.get_distance_mm();
    Z_motor.speed_control(high_pid.control(error_h),0);
    delay(5);
  }
  Z_motor.speed_control(0,0,false,true);
}
//获取当前X轴位置,单位mm
float get_now_x_location(){
  return pluse_to_mm(X_motor.read_input_pulses())+DATA.X_ZERO_POINT;
}
//设置X轴位置,单位mm
void set_x_location(float location,float speed,uint8_t acce=0,bool need_wait=false){
  X_motor.pulse_control(mm_to_pluse(location-DATA.X_ZERO_POINT),speed,acce);
  //等待
  while(need_wait&&(abs(get_now_x_location()-location)>5)){
    delay(50);    
  }
}


//归零
void rezero(){
  X_motor.speed_control(DATA.offset_dir*60,0);
  while(!digitalRead(11)){
    delay(1);
  }
  X_motor.pulse_control(DATA.offset_dir*-256*1,30);
  delay(1000);
  X_motor.speed_control(DATA.offset_dir*3,0);
  while(!digitalRead(11)){
    delay(1);
  }
  X_motor.speed_control(0,0);
  X_motor.angle_reset();
}

//微动中断函数
void IO11interrupt( ){
  digitalWrite(15,digitalRead(11));
}

float pluse_to_mm(int pluse){
    return 40*PI*pluse/(16*200);
}
int64_t mm_to_pluse(float location){
    return location*16*200/float(40*PI);
}

void update_sensor(void* p){
  while(1){
    high_sensor.update();
    X_sensor.update();
    Y_sensor.update();
    delay(5);
  }

}
//状态指示灯task
void led_update(void *p){
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(11,INPUT_PULLDOWN);
  while(1){
    if(high_sensor.get_distance_mm(false)!=0){
      digitalWrite(6,1);
    }else{
      digitalWrite(6,0);
    }
    if(X_sensor.get_distance_mm(false)!=0){
      digitalWrite(5,1);
    }else{
      digitalWrite(5,0);
    }
    if(Y_sensor.get_distance_mm(false)!=0){
      digitalWrite(4,1);
    }else{
      digitalWrite(4,0);
    }
    if(grap_servo.SERVO_TEMP_READ()<60 &&grap_servo.SERVO_TEMP_READ()!=0){
      digitalWrite(18,1);
    }else{
      digitalWrite(18,0);
    }
    if(Z_motor.read_Bus_voltage()!=0){
      digitalWrite(17,1);
    }else{
      digitalWrite(17,0);
    }
    if( X_motor.read_Bus_voltage()!=0){
      digitalWrite(16,1);
    }else{
      digitalWrite(16,0);
    }
  }

}

void setup() {
  motor_ser.begin(115200,SERIAL_8N1,10,9);
  servo_ser.begin(115200,SERIAL_8N1,35,36);
  high_sensor.setup();
  X_sensor.setup();
  Y_sensor.setup();
  Serial.begin(115200);
 
  DATA.setup();
  DATA.read();
  attachInterrupt(11,IO11interrupt,CHANGE);
  xTaskCreatePinnedToCore(update_sensor,"update_sensor",2048,NULL,2,NULL,0);
  xTaskCreatePinnedToCore(led_update,"led_update",2048,NULL,2,NULL,1);

  delay(2000);
  
}

void loop() {

}

