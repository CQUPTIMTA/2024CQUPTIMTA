/*
 * @LastEditors: qingmeijiupiao
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-26 20:23:08
 */
#ifndef ESP32FLASHEEPROM_HPP
#define ESP32FLASHEEPROM_HPP

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
        //注意KEY长度要小于15
        ID=preferences.getInt("ID",0);
        Xdirection=preferences.getInt("Xdirection",1);
        Zdirection=preferences.getInt("Zdirection",1);
        offset_dir = preferences.getInt("offset_dir",1);
        X_ZERO_POINT = preferences.getFloat("X_ZERO_POINT",0);
        grap_servo_open = preferences.getFloat("gso",0);
        grap_servo_close = preferences.getFloat("gsc",0);

    }

    void write(){
        preferences.putInt("ID",ID);
        preferences.putInt("Xdirection",Xdirection);
        preferences.putInt("Zdirection",Zdirection);
        preferences.putInt("offset_dir",offset_dir);
        preferences.putFloat("X_ZERO_POINT",X_ZERO_POINT);
        preferences.putFloat("gso",grap_servo_open);
        preferences.putFloat("gsc",grap_servo_close);
    }
    void close(){
        preferences.end();
    }

    
    
    // ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓这里写需要存放在FLASH的数据↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
    uint8_t ID=0;
    int Xdirection=1;    //X电机的正方向旋转变量
    int Zdirection=1;     //Z电机的正方向旋转变量
    int offset_dir = 1;//归零时应该走向的方向 1为X正方向 -1为X负方向
    float X_ZERO_POINT=0;// 碰到归零点的X坐标，单位mm
    float grap_servo_open=0;//夹爪开角度
    float grap_servo_close=0;//夹爪关角度
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
