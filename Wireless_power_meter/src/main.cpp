#include <Arduino.h>
#include "INA226.h"
#include "U8g2lib.h"
#include "Wire.h"
INA226 INA(0x40);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0,26,25);
char names[6][20] =
{
  "CONFIGURATION: ",
  "        SHUNT: ",
  "  BUS VOLTAGE: ",
  "        POWER: ",
  "      CURRENT: ",
  "  CALIBRATION: "
};


// void updata(){
//     io16state = digitalRead(16);
//     if(io16state!=last_io16state && io16state){
//         bats+=1;
//         bats=bats%7;
//         if(bats==0){
//             bats+=2;
//         }
//     }
//     last_io16state = io16state;
//     io14state = digitalRead(14);
//     if(io14state!=last_io14state && io14state){
//         button1();
//     }
//     last_io14state = io14state;
// }
double crurrent=0;
double bus_voltage=0;
void setup() {
    Serial.begin(9600);

    Wire.begin(25,26);
    
    u8g2.begin();

    u8g2.setFont(u8g2_font_10x20_me);
    pinMode(12,OUTPUT);
    digitalWrite(12,HIGH);


}

void loop() {
  // for (int r = 0; r < 6; r++)
  // {
  //   Serial.print('\t');
  //   Serial.print(names[r]);
  //   Serial.print('\t');
  //   Serial.print(INA.getRegister(r), DEC);
  //   Serial.print('\t');
  //   Serial.println(INA.getRegister(r), HEX);
  // }
  // delay(1000);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print(INA.getRegister(2)*0.00125);
  u8g2.print("V");
  u8g2.setCursor(64, 15);
  int row_data = INA.getRegister(1);
  crurrent = row_data>0xff00 ? 0 : double(row_data)*1e-3;
  u8g2.print(crurrent);
  u8g2.print("A");

  u8g2.setCursor(0, 32);
  u8g2.print(bus_voltage*crurrent);
  u8g2.print("W");
  u8g2.setCursor(70, 32);
    // u8g2.print(bats);
    // u8g2.print("S");
  u8g2.sendBuffer();
  delay(10);
  //   // updata();
  //   // if(bus_voltage<bats*3.3){
  //   //     buzz(true);
  //   // }else{
  //   //     buzz(false);
  //   // }

}

