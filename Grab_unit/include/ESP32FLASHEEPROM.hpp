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
        temperature=preferences.getFloat("temperature",0);
        Xdirection=preferences.getInt("Xdirection",1);
        Zdirection=preferences.getInt("Zdirection",1);
        Ysensor_zero_point=preferences.getFloat("Ys_zero_p",0);
        Xsensor_zero_point=preferences.getFloat("Xs_zero_p",0);
        PUT_Z_POINT=preferences.getFloat("PUT_Z_POINT",0);
        TAKE_Z_POINT=preferences.getFloat("TAKE_Z_POINT",0);
        sensor_high=preferences.getFloat("sensor_high",0);
        offset_dir = preferences.getInt("offset_dir",1);
        X_ZERO_POINT = preferences.getFloat("X_ZERO_POINT",0);
        X_PLUSE_MM = preferences.getFloat("X_PLUSE_MM",0);
        Z_PLUSE_MM = preferences.getFloat("Z_ZERO_POINT",0);
        grap_servo_open = preferences.getFloat("gso",0);
        grap_servo_close = preferences.getFloat("gsc",0);
        senser_x_down = preferences.getFloat("senser_x_down",0);
        senser_x_up = preferences.getFloat("senser_x_up",0);
        senser_y_down = preferences.getFloat("senser_y_down",0);
        senser_y_up = preferences.getFloat("senser_y_up",0);

    }

    void write(){
        preferences.putInt("ID",ID);
        preferences.putFloat("temperature",temperature);
        preferences.putInt("Xdirection",Xdirection);
        preferences.putInt("Zdirection",Zdirection);
        preferences.putFloat("Ys_zero_p",Ysensor_zero_point);
        preferences.putFloat("Xs_zero_p",Xsensor_zero_point);
        preferences.putFloat("PUT_Z_POINT",PUT_Z_POINT);
        preferences.putFloat("TAKE_Z_POINT",TAKE_Z_POINT);
        preferences.putFloat("sensor_high",sensor_high);
        preferences.putInt("offset_dir",offset_dir);
        preferences.putFloat("X_ZERO_POINT",X_ZERO_POINT);
        preferences.putFloat("X_PLUSE_MM",X_PLUSE_MM);
        preferences.putFloat("Z_PLUSE_MM",Z_PLUSE_MM);
        preferences.putFloat("gso",grap_servo_open);
        preferences.putFloat("gsc",grap_servo_close);
        preferences.putFloat("senser_x_down",senser_x_down);
        preferences.putFloat("senser_x_up",senser_x_up);
        preferences.putFloat("senser_y_down",senser_y_down);
        preferences.putFloat("senser_y_up",senser_y_up);
    }
    void close(){
        preferences.end();
    }

    
    
    // ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓这里写需要存放在FLASH的数据↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
    uint8_t ID=0;
    float temperature=0;
    int Xdirection=1;    //X电机的正方向旋转变量
    int Zdirection=1;     //Z电机的正方向旋转变量
    float Ysensor_zero_point=0; //Y超声波相对于抓取中心点的偏移，单位mm
    float Xsensor_zero_point=0;//X超声波相对于抓取中心点的偏移，单位mm
    float PUT_Z_POINT=0;//放砝码时Z超声波的高度，单位mm
    float TAKE_Z_POINT=0;//拿砝码Z超声波的高度，单位mm
    float sensor_high=0;//使用xy超声波定位时向上移动的距离，单位mm
    int offset_dir = 1;//归零时应该走向的方向 1为X正方向 -1为X负方向
    float X_ZERO_POINT=0;// 碰到归零点的X坐标，单位mm
    float X_PLUSE_MM=0;//X轴移动一毫米的脉冲数
    float Z_PLUSE_MM=0;//Z轴移动一毫米的脉冲数
    float grap_servo_open=0;//夹爪开角度
    float grap_servo_close=0;//夹爪关角度
    float senser_x_up=0;//超声波下降对应X的舵机位置
    float senser_x_down=0;//超声波上升对应X的舵机位置
    float senser_y_up=0;//超声波下降对应y的舵机位置
    float senser_y_down=0;//超声波上升对应y的舵机位置
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
