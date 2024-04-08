#ifndef SENSOR_HPP
#define SENSOR_HPP
#include <Arduino.h>
#include "filter.hPP"
// 定义了一个名为SENSOR的类，实现超声波测距功能
class SENSOR{
  public:
  SENSOR(int trigpin, int echopin,bool using_filter=false,int filter_size=0){
    this->trigpin = trigpin;
    this->echopin = echopin;
    if (using_filter){
      this->ft = new filter(filter_size);
      this->data_size = filter_size;       
    }
  };
  ~SENSOR(){
    delete ft;
  };
  int data_size;
  // 引脚初始化
  void setup(){
    pinMode(trigpin, OUTPUT); // 设置触发引脚为输出模式
    digitalWrite(trigpin, LOW);
    pinMode(echopin, INPUT); // 设置回响引脚为输入模式
  };
  // 更新距离
  void update(){
    digitalWrite(trigpin, HIGH); // 把信号引脚设为高电平，发出一个10微秒的触发信号
    delayMicroseconds(10);  
    digitalWrite(trigpin, LOW);  // 触发信号结束后，将信号引脚置为低电平
    this->distance = pulseIn(echopin, HIGH)*sound_speed*0.0005;
    if(this->ft!=NULL){
    this->distance = this->ft->get_data(this->distance); // 如果使用滤波器, 返回滤波后的距离值
    }
  }
  // get_distance_mm 函数返回上一次测得的距离，单位毫米
  float get_distance_mm(bool need_update = true){
    if(need_update){
      this->update();
    }
    return this->distance;
  };
  // set_sound_speed 函数设置声速
  void set_sound_speed(float sound_speed){
    if(sound_speed>300&&sound_speed<380){// 限制声速在合理范围
      this->sound_speed = sound_speed;
    }
  }
  
  private:
  float sound_speed = 340.29;  //声速
  float distance = 0.0;//测得距离,单位毫米
  int trigpin; // 定义trigpin为传感器触发引脚
  int echopin; // 定义echopin为传感器检测回波引脚
  filter* ft ;//滤波器指针
};
#endif // SENSOR