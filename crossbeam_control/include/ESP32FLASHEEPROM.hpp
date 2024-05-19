#ifndef ESP32FLASHEEPROM_HPP
#define ESP32FLASHEEPROM_HPP

#include <Arduino.h>
#include "Preferences.h"
Preferences preferences;
class NVSDATA{
    public:
    void setup(){
        preferences.begin("DATA", false);
    };

    void read(){
        ID=preferences.getInt("ID",0);
        offset_dir=preferences.getInt("offset_dir",1);
        X_ZERO_POINT=preferences.getFloat("X_ZERO_POINT",0);
        Y_PLUSE_MM=preferences.getFloat("Y_PLUSE_MM",0);
    }

    void write(){
        preferences.putInt("ID",ID);
        preferences.putInt("offset_dir",offset_dir);
        preferences.putFloat("X_ZERO_POINT",X_ZERO_POINT);
        preferences.putFloat("Y_PLUSE_MM",Y_PLUSE_MM);
    }
    void close(){
        preferences.end();
    }

    // ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓这里写需要存放在FLASH的数据↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
    uint8_t ID=0;
    int8_t offset_dir = 1;//归零时应该走向的方向 1为Y正方向 -1为Y负方向
    float X_ZERO_POINT=0;// 碰到归零点的Y坐标，单位mm
    float Y_PLUSE_MM=0;//X轴移动一毫米的脉冲数

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
