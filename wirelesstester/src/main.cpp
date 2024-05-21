/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-17 10:13:33
 */
#include <Arduino.h>
#include "SimpleSerialShell.h"
#include "ESPNOW.hpp"
#include <stdlib.h>
//通过espnow发送数据
void esp_now_send_package(package_type type,int _id,String name,uint8_t* data,int datalen,uint8_t* receive_MAC){
  data_package send_data;
  send_data.packge_type=type;
  send_data.id=_id;
  for(int i=0;i<name.length();i++){
    send_data.name[i]=name[i];
  }
  for(int i=0;i<datalen;i++){
    send_data.data[i]=data[i];
  }
  send_data.name_len=name.length();
  send_data.data_len=datalen;
  send_data.add_checksum();

  
  uint8_t send_data_array[send_data.name_len+send_data.data_len+7];
  //结构体到数组
  send_data.get_data(send_data_array);
  //发送
  esp_err_t err = esp_now_send(receive_MAC,send_data_array,send_data.get_len());
}


//测试函数
int test_esp_now(int argc, char** args){
    if (argc != 2) {
        shell.println("bad argument count");
        return -1;
    }
    float pra=strtod(args[1],NULL);

    esp_now_send_package(package_type_request,6,"test",(uint8_t*)&pra,4,receive_MACAddress);
    int i=0;
    while(receive_data_buf.id==0){
      delay(10);
      i++;
      if(i>300){
        shell.println(F("time out"));
        return 0;
      }
    }
    receive_data_buf.id=0;
    int data_int=*(float*)&receive_data_buf.data;
    shell.print("data:");
    shell.println(data_int);
    return 0;
}

int showinfo(int /*argc*/ = 0, char** /*argv*/ = NULL)
{

    
    shell.println(F( "Running " __FILE__ ",\nBuilt " __DATE__));
    shell.print(">");
    return 0;
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  shell.attach(Serial);
  shell.addCommand(F("info"), showinfo);
  shell.addCommand(F("test"), test_esp_now);
  esp_now_setup();
}

void loop() {
    shell.executeIfInput();
    delay(10);

}

