#ifndef ESP32FLASHEEPROM_HPP
#define ESP32FLASHEEPROM_HPP

#include <Arduino.h>
#include "Preferences.h"
Preferences preferences;

class ESP32FLASHEEPROM{
    public:
    /*
    * 构造函数
    */
    ESP32FLASHEEPROM(){
    };
    
    /*
    * 析构函数
    */
    ~ESP32FLASHEEPROM(){};
    
    /*
    * 初始化函数
    */
    void setup(){
        preferences.begin("DATA", false);
    };

    /*
    * 读取数据函数
    */
    void read(){
        ID=preferences.getInt("ID",0);
        temperature=preferences.getFloat("temperature",0);
        Xdirection=preferences.getInt("Xdirection",1);
        Zdirection=preferences.getInt("Zdirection",1);
        Ysensor_zero_point=preferences.getFloat("Ysensor_zero_point",0);
        Xsensor_zero_point=preferences.getFloat("Xsensor_zero_point",0);
        PUT_Z_POINT=preferences.getFloat("PUT_Z_POINT",0);
        TAKE_Z_POINT=preferences.getFloat("TAKE_Z_POINT",0);
        sensor_high=preferences.getFloat("sensor_high",0);
        offset_dir = preferences.getInt("offset_dir",1);
        X_ZERO_POINT = preferences.getFloat("X_ZERO_POINT",0);
        X_PLUSE_MM = preferences.getFloat("X_PLUSE_MM",0);
        Z_PLUSE_MM = preferences.getFloat("Z_ZERO_POINT",0);
        grap_servo_open = preferences.getInt("grap_servo_open",0);
        grap_servo_close = preferences.getInt("grap_servo_close",0);

    }

    void write(){
        preferences.putInt("ID",ID);
        preferences.putFloat("temperature",temperature);
        preferences.putInt("Xdirection",Xdirection);
        preferences.putInt("Zdirection",Zdirection);
        preferences.putFloat("Ysensor_zero_point",Ysensor_zero_point);
        preferences.putFloat("Xsensor_zero_point",Xsensor_zero_point);
        preferences.putFloat("PUT_Z_POINT",PUT_Z_POINT);
        preferences.putFloat("TAKE_Z_POINT",TAKE_Z_POINT);
        preferences.putFloat("sensor_high",sensor_high);
        preferences.putInt("offset_dir",offset_dir);
        preferences.putFloat("X_ZERO_POINT",X_ZERO_POINT);
        preferences.putFloat("X_PLUSE_MM",X_PLUSE_MM);
        preferences.putFloat("Z_PLUSE_MM",Z_PLUSE_MM);
        preferences.putInt("grap_servo_open",grap_servo_open);
        preferences.putInt("grap_servo_close",grap_servo_close);
    }
    void close(){
        preferences.end();
    }

    
    
    // ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓这里写需要存放在FLASH的数据↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
    uint8_t ID=0;
    float temperature=0;
    int8_t Xdirection=1;    //X电机的正方向旋转变量
    int8_t Zdirection=1;     //Z电机的正方向旋转变量
    float Ysensor_zero_point=0; //Y超声波相对于抓取中心点的偏移，单位mm
    float Xsensor_zero_point=0;//X超声波相对于抓取中心点的偏移，单位mm
    float PUT_Z_POINT=0;//放砝码时Z超声波的高度，单位mm
    float TAKE_Z_POINT=0;//拿砝码Z超声波的高度，单位mm
    float sensor_high=0;//使用xy超声波定位时向上移动的距离，单位mm
    int8_t offset_dir = 1;//归零时应该走向的方向 1为X正方向 -1为X负方向
    float X_ZERO_POINT=0;// 碰到归零点的X坐标，单位mm
    float X_PLUSE_MM=0;//X轴移动一毫米的脉冲数
    float Z_PLUSE_MM=0;//Z轴移动一毫米的脉冲数
    float grap_servo_open=0;//夹爪开
    float grap_servo_close=0;//夹爪关
};

#endif

        /*坐标轴示意图*/
        // Y
        // ^
        // |             +--------------------------+
        // |             | **                    ** |
        // |             | **                    ** |
        // |             |                          |
        // |             |                          |
        // |             |                          |
        // |             |                          |
        // |             |                          |
        // |             |             o            |
        // |             |                          |
        // |             |                          |
        // |             |                          |
        // |             |             o            |
        // |             |     o       o       o    |
        // |             |        o         o       |
        // |             |        o         o       |
        // |             |     o       o       o    |
        // |             |             o            |
        // |             | **                    ** |
        // |             | **                    ** |
        // |             +--------------------------+
        // +---------------------------> X
