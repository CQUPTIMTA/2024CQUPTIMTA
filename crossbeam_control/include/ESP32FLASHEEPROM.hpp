#ifndef ESP32FLASHEEPROM_HPP
#define ESP32FLASHEEPROM_HPP

#include <Arduino.h>
#include <EEPROM.h>

class ESP32FLASHEEPROM{
    public:
    /*
    * 构造函数
    */
    ESP32FLASHEEPROM(){
       DATA_size= sizeof(ESP32FLASHEEPROM);
    };
    
    /*
    * 析构函数
    */
    ~ESP32FLASHEEPROM(){};
    
    /*
    * 初始化函数
    */
    void setup(){
        EEPROM.begin(DATA_size);
    };
    
    /*
    * 读取数据函数
    */
    void read(){
        uint8_t temp[4096];
        for (int i = 0; i < DATA_size; i++){
            temp[i] = EEPROM.read(i);
        }
        memcpy(this, temp, DATA_size);
        EEPROM.end();
    };
    
    /*
    * 写入数据函数
    */
    void write(){
        uint8_t temp[4096];
        memcpy(temp, this, DATA_size);
        for (int i = 0; i < DATA_size; i++){
            EEPROM.write(i, temp[i]);
        }
        EEPROM.commit();
        EEPROM.end();
    }
    
    // 数据大小
    uint16_t DATA_size=DATA_size;
    
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
