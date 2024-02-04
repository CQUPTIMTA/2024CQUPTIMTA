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
