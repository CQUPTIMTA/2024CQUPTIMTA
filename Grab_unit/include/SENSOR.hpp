#ifndef SENSOR_H
#define SENSOR_H
#include<Arduino.h>
#include "filter.hPP"
// 定义了一个名为SENSOR的类，实现超声波测距功能
class SENSOR{
  private:
    int trigpin; // 定义trigpin为传感器触发引脚
    int echopin; // 定义echopin为传感器检测回波引脚
    filter* ft ;
  public:
   // 类的构造函数
  SENSOR(int trigpin, int echopin,bool using_filter=false,int filter_size=0){
    this->trigpin = trigpin;
    this->echopin = echopin;
    if (using_filter){
      this->ft = new filter(filter_size);
      this->data_size = filter_size;       
    }
  };
  ~SENSOR();
  int data_size;
  // 引脚初始化
  void setup(){
    pinMode(trigpin, OUTPUT); // 设置触发引脚为输出模式
    digitalWrite(trigpin, LOW);
    pinMode(echopin, INPUT); // 设置回响引脚为输入模式
  };

  // get_distance 函数用于测距，返回两个物体之间的距离
  float get_distance(){
    digitalWrite(trigpin, HIGH); // 把信号引脚设为高电平，发出一个10微秒的触发信号
    delayMicroseconds(10);  
    digitalWrite(trigpin, LOW);  // 触发信号结束后，将信号引脚置为低电平
    this->distance = pulseIn(echopin, HIGH) / 58.3;
    if(this->ft!=NULL){
      return this->ft->get_data(this->distance); // 如果使用滤波器, 返回滤波后的距离值
    }
    else{
      return this->distance; // 否则, 返回原始的距离值
    }
  };
  float distance = 0.0;
};
#endif // SENSOR