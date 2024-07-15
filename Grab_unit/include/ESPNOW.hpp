/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-19 14:50:24
 */
#ifndef esp_now_hpp
#define esp_now_hpp
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <map>
extern int ID;

int last_time_receive_time=0;

uint8_t receive_MACAddress[] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
esp_now_peer_info_t peerInfo;

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
  uint8_t add_checksum(){
    uint8_t sum=header+packge_type+name_len+data_len+id;
    for(int i=0;i<name_len;i++){
        sum+=name[i];
    }
    for(int i=0;i<data_len;i++){
        sum+=data[i];
    }
    checksum=sum;
    return sum;
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
  void get_data(uint8_t* arr){
    arr[0]=0xFE;
    arr[1]=0xFE;
    arr[2]=id;
    arr[3]=packge_type;
    arr[4]=name_len;
    arr[5]=data_len;
    for(int i=0;i<name_len;i++){
      arr[6+i]=name[i];
    }
    if(data_len!=0){
      for(int i=0;i<data_len;i++){
        arr[6+name_len+i]=data[i];
      }
    }
    add_checksum();
    arr[6+name_len+data_len]=checksum;
  }
};





void esp_now_send_package(package_type type,int _id,String name,uint8_t* data,int datalen,uint8_t* receive_MAC);
// 定义一个函数指针类型，它接受 data_package 作为参数
using DataPackageCallback = void (*)(data_package);

// 创建一个回调函数映射表
std::map<String, DataPackageCallback> callback_map;






data_package re_data;
//处理数据时的回调函数
void package_response(void* p){
  
  String name = String(re_data.name,re_data.name_len); 

  //如果有对应的回调函数，则执行
  if(callback_map.find(name) != callback_map.end() ){
    
    callback_map[name](re_data);
  };
  vTaskDelete(NULL);
}
  


//接收数据时的回调函数，收到数据时自动运行
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  
  if(!re_data.add_package(data,len)) return ;
  if(re_data.id!=ID) return;

  //如果MAC地址不一致，则更新
  if(memcmp(mac, receive_MACAddress, 6) != 0){
    memcpy(receive_MACAddress,mac,6);
    //如果从未配对，则添加配对
    if(esp_now_get_peer(mac, &peerInfo) == ESP_ERR_ESPNOW_NOT_FOUND){
      memcpy(peerInfo.peer_addr, receive_MACAddress, 6);
      esp_now_add_peer(&peerInfo);
    }

  };
  //如果有对应的回调函数，则执行
  xTaskCreate(
	  package_response, "package_response_task", 8192, &re_data, 1, NULL);
  last_time_receive_time=millis();
}

//通过espnow发送数据
void esp_now_send_package(package_type type,int _id,String name,uint8_t* data,int datalen,uint8_t* receive_MAC=receive_MACAddress){

  data_package send_data;
  send_data.packge_type=type;
  send_data.id=_id;
  send_data.name_len=name.length();
  send_data.data_len=datalen;
  for(int i=0;i<name.length();i++){
    send_data.name[i]=name[i];
  }
  for(int i=0;i<datalen;i++){
    send_data.data[i]=data[i];
  }

  uint8_t send_data_array[send_data.name_len+send_data.data_len+7];
  //结构体到数组
  send_data.get_data(send_data_array);
  //发送
  esp_err_t err = esp_now_send(receive_MAC,send_data_array,send_data.get_len());
}





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