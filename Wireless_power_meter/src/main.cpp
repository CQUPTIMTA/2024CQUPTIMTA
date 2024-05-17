#include <Arduino.h>
#include "INA226.h"
#include "U8g2lib.h"
#include "Wire.h"
TwoWire Wire_1 = TwoWire(1);
INA226 INA(0x40,&Wire_1);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0,14,27);
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
float crurrent=0;
float bus_voltage=0;
bool IO5_FLAG=false;
uint16_t row_data=0;
uint16_t r=0;
void io23interp(){
  IO5_FLAG!=IO5_FLAG;
  digitalWrite(5,IO5_FLAG);
} 



void setup() {
    Wire_1.setPins(25,26);
    Serial.begin(9600);
    pinMode(23,PULLDOWN);
    Wire.begin(25,26);
    
    u8g2.begin();
    
    u8g2.setFont(u8g2_font_10x20_me);
    pinMode(35,OUTPUT);
    digitalWrite(35,HIGH);
    //attachInterrupt(23,io23interp,RISING);

    

}

void loop() {
  //Serial.println(digitalRead(23));
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

  
  //Serial.print(bus_voltage);
  Serial.println("hello");

  u8g2.clearBuffer();

  u8g2.setCursor(0, 15);
  u8g2.print(float(INA.getRegister(2))/800.f);
  u8g2.print("V");
  u8g2.setCursor(64, 15);
  row_data = INA.getRegister(1);
  crurrent = row_data>0xff00 ? 0 : double(row_data)*1e-3;
  u8g2.print(crurrent);
  u8g2.print("A");
  u8g2.setCursor(0, 32);
  u8g2.print((INA.getRegister(2)/800)*crurrent);
  u8g2.print("W");
  u8g2.setCursor(70, 32);
    // u8g2.print(bats);
    // u8g2.print("S");
  u8g2.sendBuffer();
 
  delay(20);


}

