#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "SENSOR.hpp"
#include "EMMC42V5.hpp"
#include "HEServo.hpp"
// //高度超声波
// SENSOR high_sensor();
// // X轴和Z轴超声波
// SENSOR Y_sensor();
// SENSOR X_sensor();
//电机串口
HardwareSerial motor_ser=HardwareSerial(2);
//舵机串口
HardwareSerial servo_ser=HardwareSerial(1);

ESP32FLASHEEPROM DATA;

EMMC42V5 X_motor(&motor_ser,2);
EMMC42V5 Z_motor(&motor_ser,1);
//夹爪舵机
HEServo grap_servo(&servo_ser,1);
HEServo X_servo(&servo_ser,2);
HEServo Y_servo(&servo_ser,3);


void setup() {
  motor_ser.begin(115200);
  servo_ser.begin(115200);
  DATA.setup();
  DATA.read();
}

void loop() {
  delay(1000); // 每隔1秒发布一次

}

