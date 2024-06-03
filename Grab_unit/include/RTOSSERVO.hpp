/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-06-03 15:37:16
 */
#ifndef RTOSSERVO_HPP
#define RTOSSERVO_HPP
#include <Arduino.h>
#include "HEServo.hpp"
class RTOSSERVO :public HEServo{
public:
    RTOSSERVO(HardwareSerial *ser,uint8_t id):HEServo(ser,id){};
    ~RTOSSERVO(){};
protected:
    virtual void send(uint8_t *data) override{
        xSemaphoreTake(xsemaphore, portMAX_DELAY);//开启互斥锁
        //写入固定包头
        this->serial->write(0x55);
        this->serial->write(0x55);
        //写入数据
        for (int i = 0; i < data[1]+1; i++){
            this->serial->write(data[i]);
        }
        delay(1);
        xSemaphoreGive(xsemaphore);//释放互斥锁
    };

    virtual void read(uint8_t *data,uint8_t len){
        xSemaphoreTake(xsemaphore, portMAX_DELAY);//开启互斥锁
        delay(1);   
        for (int i = 0; i < MAX_TIME; i++){
            if (this->serial->available()){
                data[0]=this->serial->read();
                if (data[0] == this->id){
                    for (int j = 1; j < len; j++){
                        data[j]=this->serial->read();
                    }
                    break;
                }
            }
        }
        xSemaphoreGive(xsemaphore);//释放互斥锁
    }

    static SemaphoreHandle_t  xsemaphore;//互斥锁

};

SemaphoreHandle_t RTOSSERVO::xsemaphore = xSemaphoreCreateMutex();
#endif

