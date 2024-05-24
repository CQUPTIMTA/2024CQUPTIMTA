//#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "SENSOR.hpp"
#include "EMMC42V5.hpp"
#include "HEServo.hpp"
#include "PID.hpp"
#include "ESPNOW.hpp"
#define laser_pin 42


int ID=0;
namespace GrapUnit{
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


  float get_now_location_x() {
    int64_t m1=X_motor.read_current_location()+DATA.X_ZERO_POINT*65535.0/(20*2.0*PI);
    return double(m1)*40.0*PI/65535.0;
  }

  void wait_to_x(float x){
    while(abs(x-get_now_location_x())>10){
      delay(20);
    }
  }

  void move_to_x(float x,float speed,float acce=0,bool need_wait=true) {
    float now=get_now_location_x();
    float delta=x-now;
    int delta_pulse=delta*200*16/(20*2.0*PI);
    X_motor.pulse_control(delta_pulse,speed,acce,false,true);
    if(need_wait){
        wait_to_x(x);
    }
  }
  /*          */
  float get_location_z(){
    
    return high_sensor.get_distance_mm(true);
  }
    void wait_to_z(float z){
    while(abs(z-get_location_z())>10){
      delay(20);
    }
  }

  void move_to_z(float z,float speed,float acce=0,bool need_wait=true) {
    float now=get_location_z();
    float delta=z-now;
    int delta_pulse=delta*200*16/(5*2.0*PI);
    Z_motor.pulse_control(DATA.Zdirection*delta_pulse,speed,acce,false,true);
    if(need_wait){
        wait_to_z(z);
    }
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
      delay(20);
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
      if(grap_servo.SERVO_TEMP_READ()<70 &&grap_servo.SERVO_TEMP_READ()!=0){
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
      delay(4000);
    }
  }
}

namespace EspnowCallback{
  void online_test(data_package redata){
      esp_now_send_package(package_type_response,redata.id,"online_test",nullptr,0,receive_MACAddress);
  };
  void auto_rezero(data_package redata){
    GrapUnit::rezero();
    esp_now_send_package(package_type_response,redata.id,"auto_rezero",nullptr,0,receive_MACAddress);
  }
  void move_to_x(data_package redata){
    float y=*(float*)redata.data;
    float speed=*(float*)(redata.data+4);
    float acce=*(float*)(redata.data+8);
    GrapUnit::move_to_x(y,speed,acce);
    esp_now_send_package(package_type_response,redata.id,"move_to_x",nullptr,0,receive_MACAddress);
  }
  void move_x(data_package redata){
    float delta_x=*(float*)redata.data;
    float speed=*(float*)(redata.data+4);
    float acce=*(float*)(redata.data+8);
    GrapUnit::move_to_x(delta_x+GrapUnit::get_now_location_x(),speed,acce);
    esp_now_send_package(package_type_response,redata.id,"move_x",nullptr,0,receive_MACAddress);
  }
  void move_to_z(data_package redata){
    float y=*(float*)redata.data;
    float speed=*(float*)(redata.data+4);
    float acce=*(float*)(redata.data+8);
    GrapUnit::move_to_z(y,speed,acce);
    esp_now_send_package(package_type_response,redata.id,"move_to_z",nullptr,0,receive_MACAddress);
  }
  void move_z(data_package redata){
    float delta_z=*(float*)redata.data;
    float speed=*(float*)(redata.data+4);
    float acce=*(float*)(redata.data+8);
    GrapUnit::move_to_z(delta_z+GrapUnit::get_location_z(),speed,acce);
    esp_now_send_package(package_type_response,redata.id,"move_z",nullptr,0,receive_MACAddress);
  }
  void enable(data_package redata){
    char name=redata.data[0];
    bool state=(bool)redata.data[1];
    if(name=='X'){
      GrapUnit::X_motor.enable(state);
    }else if(name=='Z'){
      GrapUnit::Z_motor.enable(state);
    }
    esp_now_send_package(package_type_response,redata.id,"enable",nullptr,0,receive_MACAddress);
  }
  void set_zero_point(data_package redata){
    float point=*(float*)redata.data;
    GrapUnit::DATA.X_ZERO_POINT=point; 
  }
  void laser(data_package redata){
    digitalWrite(laser_pin,!bool(redata.data[0]));
    esp_now_send_package(package_type_response,redata.id,"laser",nullptr,0,receive_MACAddress);
  }
  void grap(data_package redata){
    float flag = *(float*)redata.data;
    if(flag==1){
      GrapUnit::get_open();
    }
    else{
      GrapUnit::get_close();
    }
    esp_now_send_package(package_type_response,redata.id,"grap",nullptr,0,receive_MACAddress);
  }
  void get_sensor_distance(data_package redata){
    char name=redata.data[0];
    float distance=0;
    if(name=='X'){
      distance=GrapUnit::X_sensor.get_distance_mm();
    }else if(name=='Y'){
      distance=GrapUnit::Y_sensor.get_distance_mm();
    }else if(name=='Z'){
      distance=GrapUnit::high_sensor.get_distance_mm();
    }
    esp_now_send_package(package_type_response,redata.id,"get_sensor_distance",(uint8_t*)&distance,4,receive_MACAddress);
  }
  void get_voltage(data_package redata){
    float voltage=GrapUnit::X_motor.read_Bus_voltage()/1000.0;
    esp_now_send_package(package_type_response,redata.id,"get_voltage",(uint8_t*)&voltage,4,receive_MACAddress);
  }

  void set_now_location(data_package redata){
    float set_x=*(float*)redata.data;
    float deleta=set_x-GrapUnit::get_now_location_x();
    GrapUnit::DATA.X_ZERO_POINT=GrapUnit::DATA.X_ZERO_POINT+deleta;
    GrapUnit::DATA.write();
    esp_now_send_package(package_type_response,redata.id,"set_now_location",nullptr,0,receive_MACAddress);

  }
  void add_callbacks(){
    callback_map["online_test"]=online_test;
    callback_map["auto_rezero"]=auto_rezero;
    callback_map["move_to_x"]=move_to_x;
    callback_map["move_x"]=move_x;
    callback_map["move_to_z"]=move_to_z;
    callback_map["move_z"]=move_z;
    callback_map["enable"]=enable;
    callback_map["set_zero_point"]=set_zero_point;
    callback_map["laser"]=laser;
    callback_map["grap"]=grap;
    callback_map["get_sensor_distance"]=get_sensor_distance;
    callback_map["get_voltage"]=get_voltage;
    callback_map["set_now_location"]=set_now_location;
  }
}



void setup() {
  Serial.begin(115200);
  GrapUnit::motor_ser.begin(115200,SERIAL_8N1,10,9);
  GrapUnit::servo_ser.begin(115200,SERIAL_8N1,35,36);
  GrapUnit::high_sensor.setup();
  GrapUnit::X_sensor.setup();
  GrapUnit::Y_sensor.setup();
  GrapUnit::DATA.setup();
  GrapUnit::DATA.read();
  EspnowCallback::add_callbacks();
  ID=GrapUnit::DATA.ID;
  attachInterrupt(11,GrapUnit::IO11interrupt,CHANGE);
  //xTaskCreatePinnedToCore(GrapUnit::update_sensor,"update_sensor",2048,NULL,2,NULL,1);
  //xTaskCreatePinnedToCore(GrapUnit::led_update,"led_update",2048,NULL,3,NULL,0);
  //xTaskCreatePinnedToCore(GrapUnit::Servo_temperature_read,"Servo_temperature_read",2048,NULL,1,NULL,1);

  esp_now_setup();
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
  digitalWrite(laser_pin,HIGH);
  

}

void loop() {
  for(int i=0;i<ID;i++){
    digitalWrite(7,HIGH);
    delay(200);
    digitalWrite(7,LOW);
    delay(200);
  }
  delay(2000);
}

