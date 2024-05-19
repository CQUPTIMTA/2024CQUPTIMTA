/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-19 14:44:23
 */
#ifndef esp_now_hpp
#define esp_now_hpp
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

int last_time_receive_time=0;

//包类型
enum package_type {
    package_type_normal = 0,
    package_type_error = 1,
    package_type_request = 2,
    package_type_response = 3
};

//数据包格式
struct data_package {
  uint16_t header=0xFEFE;
  uint8_t id=0;
  uint8_t packge_type;
  uint8_t name_len;
  uint8_t data_len;
  char name[30] ;
  uint8_t data[50] ;
  uint8_t checksum=0;
  bool check(){
    uint8_t sum=header+packge_type+name_len+data_len+id;
    for(int i=0;i<name_len;i++){
        sum+=name[i];
    }
    for(int i=0;i<data_len;i++){
        sum+=data[i];
    }
    return sum==checksum;
  }
  void add_checksum(){
      checksum+=header+packge_type+name_len+data_len+id;
      for(int i=0;i<name_len;i++){
          checksum+=name[i];
      }
      for(int i=0;i<data_len;i++){
          checksum+=data[i];
      }
  }
  bool add_package(const uint8_t *data,int len){
    if(len<2||(data[0]!=0xFE&&data[1]!=0xFE)) return false;
    data_package re_data;
    re_data.id=data[2];
    re_data.packge_type=data[3];
    re_data.name_len=data[4];
    re_data.data_len=data[5];
    for(int i=0;i<re_data.name_len;i++){
      re_data.name[i]=data[6+i];
    }
    for(int i=0;i<re_data.data_len;i++){
      re_data.data[i]=data[6+re_data.name_len+i];
    }
    re_data.checksum=data[len-1];
    if(!re_data.check()) return false;
    //存入数据
    memcpy(this,&re_data,sizeof(re_data));
    return true;
  }
  bool add_package(uint8_t* data,int len){
    const uint8_t* d=(const uint8_t*)data;
    add_package(d,len);
  }
  int get_len(){
    return 6+name_len+data_len+1;
  }
  void get_data(uint8_t* data){
    data[0]=0xFE;
    data[1]=0xFE;
    data[2]=id;
    data[3]=packge_type;
    data[4]=name_len;
    data[5]=data_len;
    for(int i=0;i<name_len;i++){
      data[6+i]=name[i];
    }
    for(int i=0;i<data_len;i++){
      data[6+name_len+i]=data[i];
    }
    add_checksum();
    data[6+name_len+data_len]=checksum;
  }
};


//接收数据缓存
data_package receive_data_buf;


//接收数据时的回调函数，收到数据时自动运行
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  data_package re_data;
  if(!re_data.add_package(data,len)) return ;
  
  // if(len<2||(data[0]!=0xFE&&data[1]!=0xFE)) return;
  // data_package re_data;
  // re_data.id=data[2];
  // re_data.packge_type=data[3];
  // re_data.name_len=data[4];
  // re_data.data_len=data[5];
  // for(int i=0;i<re_data.name_len;i++){
  //   re_data.name[i]=data[6+i];
  // }
  // for(int i=0;i<re_data.data_len;i++){
  //   re_data.data[i]=data[6+re_data.name_len+i];
  // }
  // re_data.checksum=data[len-1];
  // if(!re_data.check()) return;
  //存入接受缓存
  memcpy(&receive_data_buf,&re_data,sizeof(re_data));
  last_time_receive_time=millis();

}



//peerInfo 必须是全局变量否则espnow不可用
esp_now_peer_info_t peerInfo;
uint8_t receive_MACAddress[] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//ESP-NOW初始化
void esp_now_setup() {
  //设置WiFi模式为WIFI_AP_STA（AP+Station模式）
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
      Serial.println("ESP-NOW initialization failed");
      return;
  }

  peerInfo.ifidx = WIFI_IF_STA;
  memcpy(peerInfo.peer_addr, receive_MACAddress, 6);

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
  }
  esp_now_register_recv_cb(OnDataRecv);
} 
#endif