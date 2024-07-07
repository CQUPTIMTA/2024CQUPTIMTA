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
#include <map>
#define MAX_RETRY 5
std::map<int,uint8_t*> receive_MACs;//接收MAC
esp_now_peer_info_t peerInfo;
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




std::map<String, data_package> receive_datas;//接收数据缓存
std::map<int, String> susscess_actions;//成功执行的动作


//接收数据时的回调函数，收到数据时自动运行
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  data_package re_data;
  //检查是否是数据包
  if(!re_data.add_package(data,len)) return ;


  // 如果该ID没有在配对MACmap中则添加配对信息,0是广播ID
  if(re_data.id!=0 && (receive_MACs.find(re_data.id)==receive_MACs.end())){
    receive_MACs[re_data.id]=new uint8_t[6]{mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]};
    memcpy(peerInfo.peer_addr, mac, 6);
    esp_now_add_peer(&peerInfo);
  }

  //存入成功执行的动作
  if(String(re_data.name,re_data.name_len)=="action_complete"){
    susscess_actions[re_data.id]=String((char*)re_data.data);
    return ;
  }
  receive_datas[String(re_data.name,re_data.name_len)]=re_data;


}




uint8_t receive_MACAddress[] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//ESP-NOW初始化
void esp_now_setup() {
  //设置WiFi模式为WIFI_AP_STA（AP+Station模式）
  WiFi.mode(WIFI_AP_STA);
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


//通过espnow发送数据
void esp_now_send_package(package_type type,int _id,String name,uint8_t* data,int datalen,uint8_t* receive_MAC=receive_MACAddress){

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
  
  //检查是否配对,如果配对则发送到对应的MAC，没有就发送到广播mac
  if(receive_MAC==receive_MACAddress){
    if(auto i=receive_MACs.find(_id)!=receive_MACs.end()){
      receive_MAC=receive_MACs[_id];
      Serial.println("find mac");
    }
  }
  //发送
  for(int i=0;i<MAX_RETRY;i++){
    auto err = esp_now_send(receive_MAC,send_data_array,send_data.get_len());
    if (err == ESP_OK)  break;
    delay(50);
  }
}

#endif