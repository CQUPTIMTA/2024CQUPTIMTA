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

void set_high(float high_mm){
  //PID high_pid(2,0.001,0.001);
  float error_h=high_mm-high_sensor.get_distance_mm();
  
  while(abs(error_h)>1){
    error_h=high_mm-high_sensor.get_distance_mm();//计算需要移动的高度
    Z_motor.pulse_control(error_h*(-115),100);//转一圈需要3600个脉冲，水平方向移动10*3.1415926 mm,移动1mm 需要约115个脉冲
    delay(5);
  }
  Z_motor.speed_control(0,0,false,true);
}
  void get_close(){
      grap_servo.SERVO_MOVE_TIME_WRITE(240*DATA.grap_servo_close/1000,0);//745
  }
  void get_open(){
      grap_servo.SERVO_MOVE_TIME_WRITE(240*DATA.grap_servo_open/1000,0);//880
  }
//将X Y轴的测量超声波收起来
void get_XY_back(){
  X_servo.SERVO_MOVE_TIME_WRITE(240*830/1000,0);
  Y_servo.SERVO_MOVE_TIME_WRITE(240*240/1000,0);
  delay(1);
}
void get_XY_centrol(){
  //需要按照需求调整下降高度
  X_servo.SERVO_MOVE_TIME_WRITE(240*344/1000,0);
  Y_servo.SERVO_MOVE_TIME_WRITE(240*719/1000,0);
  delay(1);
}
void get_XY_measure(){
  get_XY_centrol();
  while(abs(X_sensor.get_distance_mm()-Y_sensor.get_distance_mm())<=10){
    //操作X轴，Y轴的步进电机进行调整、直到测量误差值小于10mm
  }
  delay(1000);
  get_XY_back();
}

//归零
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

void IO11interrupt( ){
  digitalWrite(15,digitalRead(11));
}

float get_now_x_location(){
    return 40*PI*X_motor.read_input_pulses()/(16*200);
}
//*************移动毫米为单位******************//
int64_t location_to_pluse(float location){
    return location*16*200/float(40*PI);
}

void update_sensor(void* p){
  while(1){
    high_sensor.update();
    X_sensor.update();
    Y_sensor.update();
    // Serial.print("X:");
    // Serial.println(X_sensor.get_distance_mm());
    // Serial.print("Y:");
    // Serial.println(Y_sensor.get_distance_mm());
    // Serial.print("high:");
    // Serial.println(high_sensor.get_distance_mm());
    // delay(200);
  }
}

//更新LED状态
void led_update(void* p){
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
    delay(500);
  }
}
void Servo_temperature_read(void *p){
  while(1){
    Serial.println(grap_servo.SERVO_TEMP_READ());
    //如果夹爪舵机温度过高
    if(grap_servo.SERVO_TEMP_READ()>70){
      //温度过高让夹爪舵机掉电
    grap_servo.SERVO_LOAD_OR_UNLOAD_WRITE(0);
    }
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);
  motor_ser.begin(115200,SERIAL_8N1,10,9);
  servo_ser.begin(115200,SERIAL_8N1,35,36);
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
  DATA.setup();
  DATA.read();
  attachInterrupt(11,IO11interrupt,CHANGE);
  xTaskCreatePinnedToCore(update_sensor,"update_sensor",2048,NULL,2,NULL,1);
  xTaskCreatePinnedToCore(led_update,"led_update",2048,NULL,3,NULL,0);
  xTaskCreatePinnedToCore(Servo_temperature_read,"Servo_temperature_read",2048,NULL,1,NULL,1);
  Z_motor.pulse_control(-115*10,30,0);
  // rezero();
  // X_motor.pulse_control(location_to_pluse(-500),120,0);
  // while(abs(-500-get_now_x_location())>10){
  //   delay(50);
  // }
  // get_XY_centrol();
  // delay(1000);
  // get_XY_back();
  // delay(1000);
  // set_high(40);
  // delay(1000);

  // delay(1000);
  // set_high(200);
  // get_XY_centrol();
  // delay(1000);
  // get_XY_back();
  // delay(1000);
  // delay(10);
  // // X_motor.pulse_control(location_to_pluse(-200),120,0);
  // while(abs(-700-get_now_x_location())>10){
  //   delay(50);
  // }
  // delay(10);
  // grap_servo.SERVO_MOVE_TIME_WRITE(240*880/1000,0);

  // delay(2000);
  //Z_motor.enable(false,false,true);
}

void loop() {
  // //读取电机电压
  // Serial.print("Z:");
  // Serial.println(Z_motor.read_Bus_voltage());
  // Serial.print("X:");
  // Serial.println(X_motor.read_Bus_voltage());
  // delay(200);
}

