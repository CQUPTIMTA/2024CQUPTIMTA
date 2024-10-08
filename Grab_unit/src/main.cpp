#include "ESP32FLASHEEPROM.hpp"
#include "EMMC42V5.hpp"
#include "RTOSSERVO.hpp"
#include "ESPNOW.hpp"
#define laser_pin 42


int ID=0;
// 控制相关函数
namespace GrapUnit{

  //电机串口
  HardwareSerial motor_ser=HardwareSerial(2);
  //舵机串口
  HardwareSerial servo_ser=HardwareSerial(1);

  ESP32FLASHEEPROM DATA;

  EMMC42V5 X_motor(&motor_ser,2);
  EMMC42V5 Z_motor(&motor_ser,1);
  //夹爪舵机
  RTOSSERVO grap_servo(&servo_ser,1);
  //电机互斥锁,用于保护电机串口总线
  SemaphoreHandle_t motorMutex = xSemaphoreCreateMutex();

  float taget_x=0;//目标X轴位置

  //机械爪的开合
  void grap(bool state){
    if(state){
        grap_servo.SERVO_MOVE_TIME_WRITE(DATA.grap_servo_open,0);
    }else{
        grap_servo.SERVO_MOVE_TIME_WRITE(DATA.grap_servo_close,0);
    }
  }

  //归零
  void rezero(){
    X_motor.speed_control(60*DATA.offset_dir,0);
    while(!digitalRead(11)){
      delay(1);
    }
    X_motor.pulse_control(-256*1*DATA.offset_dir,30);
    delay(1000);
    X_motor.speed_control(3*DATA.offset_dir,0);
    while(!digitalRead(11)){
      delay(1);
    }
    X_motor.speed_control(0,0);
    X_motor.angle_reset();
    taget_x=0;
  }

  void IO11interrupt( ){
    digitalWrite(15,digitalRead(11));
  }
  void IO37interrupt( ){
    digitalWrite(6,!digitalRead(37));
  }

//获取当前X轴电机所处位置
  float get_now_location_x() {
    xSemaphoreTake(motorMutex, portMAX_DELAY);
    double location=0;
    double m2=0;
    for (int i = 0; i < 10; i++){
      int64_t m1=X_motor.read_current_location();
      m2=double(m1)*40.0*PI/65535.0;
      location=m2+DATA.X_ZERO_POINT;
      if(abs(location)<2000&&m1!=0){
        xSemaphoreGive(motorMutex);//释放互斥锁
        return location;
      }
      delay(10);
    }
    xSemaphoreGive(motorMutex);//释放互斥锁
    return taget_x;
  }
//等待X轴电机移动到指定位置
  void wait_to_x(float x){
    while(abs(x-get_now_location_x())>10){
      delay(20);
    }
  }
//移动到相对于X轴零点的距离
  void move_to_x(float x,float speed,float acce=0,bool need_wait=false) {
    float now=get_now_location_x();
    float delta=x-now;
    int delta_pulse=delta*200*16/(20*2.0*PI);
    xSemaphoreTake(motorMutex, portMAX_DELAY);
    X_motor.pulse_control(delta_pulse,speed,acce);
    xSemaphoreGive(motorMutex);//释放互斥锁
    taget_x=x;
    if(need_wait){
        wait_to_x(x);
    }
  }
  // //无限位归零
  // void rezero_Z(){
  //   REZREO_parameter pa;
  //   pa.mode=2;
  //   pa.direction=DATA.Zdirection>0?1:0;
  //   pa.speed=100;
  //   pa.timeout=10000;
  //   pa.limit_speed=300;
  //   pa.limit_current=500;
  //   pa.limit_time=100;
  //   pa.auto_rezero=0;
  //   Z_motor.change_parameter(0,0,pa);
  //   delay(300);
  //   Z_motor.re_zero(2);
  // }

  //有限位Z归零
  void rezero_Z(){
    if(digitalRead(37)){
      Z_motor.speed_control(-60*DATA.Zdirection,0);
    }
    while(digitalRead(37)){
      delay(1);
    }
    Z_motor.pulse_control(3000*1*DATA.Zdirection,30);
    delay(1000);
    Z_motor.speed_control(-10*DATA.Zdirection,0);
    while(digitalRead(37)){
      delay(1);
    }
    Z_motor.speed_control(0,0);
    Z_motor.angle_reset();
  }
  void move_to_grund(){
    if(digitalRead(37)){
      Z_motor.speed_control(-60*DATA.Zdirection,0);
    }
    while(digitalRead(37)){
      delay(1);
    }
    Z_motor.pulse_control(3000*1*DATA.Zdirection,30);
    delay(1000);
    Z_motor.speed_control(-10*DATA.Zdirection,0);
    while(digitalRead(37)){
      delay(1);
    }
    Z_motor.speed_control(0,0);
  }


  /* 获取Z轴当前高度         */
  float get_location_z(){
    xSemaphoreTake(motorMutex, portMAX_DELAY);
    float high=0;
    for (int i = 0; i < 10; i++){
      int64_t m1=Z_motor.read_current_location();
      high=DATA.Zdirection*5*2.0*PI*m1/65535.0;
      if(high!=0&&abs(high)<300){
        break;
      }
      delay(10);
    }
    xSemaphoreGive(motorMutex);//释放互斥锁
    return high;
  }
  void wait_to_z(float z){
    while(abs(z-get_location_z())>10){
      delay(20);
    }
  }
  //Z轴移动到相对于地面的距离
  void move_to_z(float z,float speed,float acce=0,bool need_wait=false) {
    float now=get_location_z();
    float delta=z-now;
    int delta_pulse=delta*200*16/(5*2.0*PI);
    xSemaphoreTake(motorMutex, portMAX_DELAY);
    Z_motor.pulse_control(DATA.Zdirection*delta_pulse,speed,acce);
    xSemaphoreGive(motorMutex);//释放互斥锁
    if(need_wait){
        wait_to_z(z);
    }
  }
  bool is_moveing(char axis){
    int64_t taget=0;
    int64_t now=0;
    if(axis=='X'){
      xSemaphoreTake(motorMutex, portMAX_DELAY);
      taget=X_motor.read_target_location();
      now=X_motor.read_current_location();
      xSemaphoreGive(motorMutex);//释放互斥锁
    }
    else if (axis=='Z'){
      xSemaphoreTake(motorMutex, portMAX_DELAY);
      taget=Z_motor.read_target_location();
      now=Z_motor.read_current_location();
      xSemaphoreGive(motorMutex);//释放互斥锁
    }
    if(taget==0&&now==0) return true;
    float delta=abs(taget-now);
    return delta>10000?true:false;
  }

  //更新LED状态
  void led_update(void* p){
    while(1){
      digitalWrite(6,!digitalRead(37));//Z微动
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
      delay(2000);
    }
  }
  //夹爪过温保护（每四秒检查一次）
  void Servo_temperature_read(void *p){
    while(1){
      //如果夹爪舵机温度过高
      if(grap_servo.SERVO_TEMP_READ()>70){
        //温度过高让夹爪舵机掉电
        grap_servo.SERVO_LOAD_OR_UNLOAD_WRITE(0);
      }
      delay(4000);
    }
  }
}
//无线指令回调函数
namespace EspnowCallback{
  //测试对应ID号的ESP32是否在线
  void online_test(data_package redata){
      esp_now_send_package(package_type_response,redata.id,"online_test",nullptr,0,receive_MACAddress);
  };
  //自动校准零点
  void auto_rezero(data_package redata){
    esp_now_send_package(package_type_response,redata.id,"auto_rezero",nullptr,0,receive_MACAddress);
    GrapUnit::rezero();
    
  }
  //移动x轴上的绝对位移（相对于全局坐标系）
  void move_to_x(data_package redata){
    esp_now_send_package(package_type_response,redata.id,"move_to_x",nullptr,0,receive_MACAddress);
    float y=*(float*)redata.data;
    float speed=*(float*)(redata.data+4);
    float acce=*(float*)(redata.data+8);
    
    GrapUnit::move_to_x(y,speed,acce);
    
  }
//移动X轴上的相对位移（相对于当前位置）
  void move_x(data_package redata){
    float delta_x=*(float*)redata.data;
    float speed=*(float*)(redata.data+4);
    float acce=*(float*)(redata.data+8);
    esp_now_send_package(package_type_response,redata.id,"move_x",nullptr,0,receive_MACAddress);
    GrapUnit::move_to_x(delta_x+GrapUnit::get_now_location_x(),speed,acce);
    
  }
  //获取X轴当前位置
  void get_x(data_package redata){
    float x=GrapUnit::get_now_location_x();
    esp_now_send_package(package_type_response,redata.id,"get_x",(uint8_t*)&x,4,receive_MACAddress);
  }
  //移动Z轴上的绝对位移（相对于地面）
  void move_to_z(data_package redata){
    float y=*(float*)redata.data;
    float speed=*(float*)(redata.data+4);
    float acce=*(float*)(redata.data+8);
    esp_now_send_package(package_type_response,redata.id,"move_to_z",nullptr,0,receive_MACAddress);
    GrapUnit::move_to_z(y,speed,acce);
    
  }
  //移动Z轴上的相对位移（相对于当前位置）
  void move_z(data_package redata){
    float delta_z=*(float*)redata.data;
    float speed=*(float*)(redata.data+4);
    float acce=*(float*)(redata.data+8);
    GrapUnit::move_to_z(delta_z+GrapUnit::get_location_z(),speed,acce);
    esp_now_send_package(package_type_response,redata.id,"move_z",nullptr,0,receive_MACAddress);
  }

  void rezeroZ(data_package redata){
    esp_now_send_package(package_type_response,redata.id,"rezeroZ",nullptr,0,receive_MACAddress);
    GrapUnit::rezero_Z();
  }
  void move_to_grund(data_package redata){
    esp_now_send_package(package_type_response,redata.id,"movetogrund",nullptr,0,receive_MACAddress);
    GrapUnit::move_to_grund();
  }


  //获取Z轴当前位置
  void get_z(data_package redata){
    float z=GrapUnit::get_location_z();
    esp_now_send_package(package_type_response,redata.id,"get_z",(uint8_t*)&z,4,receive_MACAddress);
  }

  //电机使能
  void enable(data_package redata){
    char name=redata.data[0];
    bool state=(bool)redata.data[1];
    if(name=='X'){
      GrapUnit::X_motor.enable(state);
    }else if(name=='Z'){
      GrapUnit::Z_motor.enable(state);
    }else if(name=='G'){
      GrapUnit::grap_servo.SERVO_LOAD_OR_UNLOAD_WRITE(state);
    }
    esp_now_send_package(package_type_response,redata.id,"enable",nullptr,0,receive_MACAddress);
  }
  //设置零点位置
  void set_zero_point(data_package redata){
    float point=*(float*)redata.data;
    GrapUnit::DATA.X_ZERO_POINT=point; 
  }
  //激光定位
  void laser(data_package redata){
    digitalWrite(laser_pin,!bool(redata.data[0]));
    esp_now_send_package(package_type_response,redata.id,"laser",nullptr,0,receive_MACAddress);
  }
  //机械爪的开合
  void grap(data_package redata){
    bool flag = redata.data[0];
    esp_now_send_package(package_type_response,redata.id,"grap",nullptr,0,receive_MACAddress);
    GrapUnit::grap(flag);
    
  }

  //获取电机电压
  void get_voltage(data_package redata){
    float voltage=GrapUnit::X_motor.read_Bus_voltage()/1000.0;
    esp_now_send_package(package_type_response,redata.id,"get_voltage",(uint8_t*)&voltage,4,receive_MACAddress);
  }
  //设置当前X的位置,用于校准归零零点
  void set_now_location(data_package redata){
    float set_x=*(float*)redata.data;
    float deleta=set_x-GrapUnit::get_now_location_x();
    GrapUnit::DATA.X_ZERO_POINT=GrapUnit::DATA.X_ZERO_POINT+deleta;
    GrapUnit::DATA.write();
    esp_now_send_package(package_type_response,redata.id,"set_now_location",nullptr,0,receive_MACAddress);
  }
  //是否在运动
  void is_moveing(data_package redata){
    char axis=redata.data[0];
    bool state=GrapUnit::is_moveing(axis);
    esp_now_send_package(package_type_response,redata.id,"is_moveing",(uint8_t*)&state,1,receive_MACAddress);
  }
  //读取舵机角度
  void read_servo_angle(data_package redata){
    char axis=redata.data[0];
    float angle=0;
    if(axis=='G'){
     angle=GrapUnit::grap_servo.SERVO_ANGLE_READ();
    }
    esp_now_send_package(package_type_response,redata.id,"read_servo_angle",(uint8_t*)&angle,4,receive_MACAddress);
  }
  //设置舵机角度
  void set_servo_angle(data_package redata){
    bool state=*(bool*)(redata.data);
    float set_angle=*(float*)(redata.data+1);
    if(state)
      GrapUnit::DATA.grap_servo_open=set_angle;
    else
      GrapUnit::DATA.grap_servo_close=set_angle;
    GrapUnit::DATA.write();
    esp_now_send_package(package_type_response,redata.id,"set_servo_angle",nullptr,0,receive_MACAddress);
  }
  void get_servo_temp(data_package redata){
    float temp=GrapUnit::grap_servo.SERVO_TEMP_READ();
    esp_now_send_package(package_type_response,redata.id,"get_servo_temp",(uint8_t*)&temp,4,receive_MACAddress);
  }

  //添加回调函数到map
  void add_callbacks(){
    callback_map["online_test"]=online_test;
    callback_map["auto_rezero"]=auto_rezero;
    callback_map["move_to_x"]=move_to_x;
    callback_map["move_x"]=move_x;
    callback_map["move_to_z"]=move_to_z;
    callback_map["move_z"]=move_z;
    callback_map["movetogrund"]=move_to_grund;
    callback_map["rezeroZ"]=rezeroZ;
    callback_map["enable"]=enable;
    callback_map["set_zero_point"]=set_zero_point;
    callback_map["laser"]=laser;
    callback_map["grap"]=grap;
    callback_map["get_voltage"]=get_voltage;
    callback_map["set_now_location"]=set_now_location;
    callback_map["get_x"]=get_x;
    callback_map["get_z"]=get_z;
    callback_map["is_moveing"]=is_moveing;
    callback_map["read_servo_angle"]=read_servo_angle;
    callback_map["set_servo_angle"]=set_servo_angle;
    callback_map["get_servo_temp"]=get_servo_temp;
    
  }
}


//初始化引脚
void PINSetup(){
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(11,INPUT_PULLDOWN);
  pinMode(37,INPUT_PULLUP);
  pinMode(laser_pin,OUTPUT_OPEN_DRAIN);
  digitalWrite(laser_pin,HIGH);
  attachInterrupt(11,GrapUnit::IO11interrupt,CHANGE);
  attachInterrupt(37,GrapUnit::IO37interrupt,CHANGE);
  digitalWrite(6,!digitalRead(37));
}
void setup() {
  Serial.begin(115200);
  GrapUnit::motor_ser.begin(115200,SERIAL_8N1,10,9);
  GrapUnit::servo_ser.begin(115200,SERIAL_8N1,35,36);
  //从NVS中读取数据,实现代码的复用
  GrapUnit::DATA.setup();
  GrapUnit::DATA.read();


  // GrapUnit::DATA.ID=5;
  // GrapUnit::DATA.X_ZERO_POINT=128.29;
  // GrapUnit::DATA.grap_servo_open=139;
  // GrapUnit::DATA.grap_servo_close=195;
  // GrapUnit::DATA.offset_dir=-1;
  // GrapUnit::DATA.Xdirection=-1;
  // GrapUnit::DATA.Zdirection=-1;
  // GrapUnit::DATA.write();
  //初始化引脚
  PINSetup();
  /*无线控制部分*/
  EspnowCallback::add_callbacks();
  esp_now_setup();
  ID=GrapUnit::DATA.ID; 
  //指示灯更新任务
  xTaskCreatePinnedToCore(GrapUnit::led_update,"led_update",2048,NULL,3,NULL,1);
  //舵机高温保护
  //xTaskCreatePinnedToCore(GrapUnit::Servo_temperature_read,"Servo_protect",2048,NULL,1,NULL,1);
  delay(3000);
  GrapUnit::grap(1);
  GrapUnit::rezero_Z();
}

void loop() {
  for(int i = 1;i<=ID;i++){
    digitalWrite(7,1);
    delay(300);
    digitalWrite(7,0);
    delay(300);
  }
  // Serial.println(GrapUnit::DATA.X_ZERO_POINT);
  // Serial.println(GrapUnit::DATA.grap_servo_open);
  // Serial.println(GrapUnit::DATA.grap_servo_close);
  delay(1000);

}

