/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-06-11 21:01:01
 */
#ifndef web_hpp
#define web_hpp
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include "roothtml.hpp"
#include "rechtml.hpp"
#include "testhtml.hpp"
#include "calihtml.hpp"
#define WIFI_SSID "CQUPTIMTA2"        //WiFi名称
#define WIFI_PASSWORD "znzzjsxh"                 //WiFi密码

#include "commands.hpp"

//创建一个异步Web服务器
WebServer server(80);  

extern void main_func(void * pvParameters);
extern TaskHandle_t main_func_handler;
// 处理设备检测的请求
void handleDetect() {
  // 发送一个空白的响应
  server.send(204);
}

// 处理不存在的请求
void handleNotFound() {
  // 重定向到根目录，弹出网页
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}



void handleRoot() {
  // 发送网页内容
    server.send(200, "text/html", htmlContent);
}

void handletest(){
    server.send(200, "text/html", testhtml);
}
void handlerec(){
    server.send(200, "text/html", rechtml);
}
void handlecali(){
    server.send(200, "text/html", calihtml);
}
void handleData(){
    
    // 检查是否有有效的HTTP请求体
    if (server.hasArg("plain") == false) {
    server.send(400, "text/plain", "Body not received");
    return;
    }

    // 获取HTTP请求体
    String body = server.arg("plain");

    // 创建JSON文档对象
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    // 检查JSON解析是否成功
    if (error) {
    server.send(400, "text/plain", "Invalid JSON");
    return;
    }
    // 获取JSON数据
    const char* idStr = doc["ID"];
    // 将id从字符串转换为整数
    int id = atoi(idStr);
    String func=doc["func"];
    JsonDocument responseDoc;
    if(func=="grap_update_data"){
        // 填充JSON数据
        responseDoc["Zposition"] = commands::get_z(id); // 这里可以替换为实际数据
        responseDoc["Xposition"] = commands::get_x(id); // 这里可以替换为实际数据
        responseDoc["servo_temp"] = commands::get_servo_temp(id); // 这里可以替换为实际数据

        // 将JSON数据序列化为字符串
        String jsonResponse;
        serializeJson(responseDoc, jsonResponse);
        // 发送响应
        server.send(200, "application/json", jsonResponse);
    }else if(func=="crossbeam_update_data"){
        // 填充JSON数据
        responseDoc["YPositionSpan"] = commands::get_y(id); // 这里可以替换为实际数据
        responseDoc["Voltage"] = commands::get_voltage(id);; // 这里可以替换为实际数据

        // 将JSON数据序列化为字符串
        String jsonResponse;
        serializeJson(responseDoc, jsonResponse);
        // 发送响应
        server.send(200, "application/json", jsonResponse);
    }else if(func=="rezero"){
        commands::rezero(id);
    }else if(func=="load"){
      bool state= doc["state"]=="true"?1:0;
      if(id<6){
        commands::enable(id,'X',state);
        commands::enable(id,'Z',state);
        commands::enable(id,'G',state);
      }else{
        commands::enable(id,'Y',state);
      }
    }else if(func=="move_to_axis"){
        Serial.println("move_to_axis");
        String axis=doc["axis"];
        Serial.println(axis);
        String point=doc["value"];
        float point_float=point.toFloat();
        if(axis==(String)'X'){
          commands::move_to_x(id,point_float);
        }else if(axis==(String)'Y'){
          commands::move_to_y(id,point_float);
        }else if(axis==(String)'Z'){
          commands::move_to_z(id,point_float);
        }
    }else if(func=="move_axis"){
        Serial.println("move_axis");
        String axis=doc["axis"];
        Serial.println(axis);
        String point=doc["value"];
        float point_float=point.toFloat();
        if(axis==(String)'X'){
          commands::move_x(id,point_float);
        }else if(axis==(String)'Y'){
          commands::move_y(id,point_float);
        }else if(axis==(String)'Z'){
          commands::move_z(id,point_float);
        }
    }else if(func=="set_now"){
        float point=doc["value"];
        commands::set_now(id,point);
    }else if(func=="laser"){
        String state=doc["state"];
        commands::laser(id,state=="true");
    //识别网页更新数据
    }else if(func=="get_unit_data"){
        float f_data[6];
        float b_data[6];
        float Voltage[6];

        //获取传感器数据
        for (int i = 11; i <= 16; i++){
          auto u_data=commands::get_recognition_unit(i);
          f_data[i-11]=u_data.front_distance;
          b_data[i-11]=u_data.back_distance;
          Voltage[i-11]=commands::get_voltage(i);
        }
        responseDoc["min"]=5;
        responseDoc["max"]=350;
        // 填充JSON数据
        for(int i=1;i<=6;i++){
          responseDoc["F"+String(i)]=f_data[i-1];
          responseDoc["B"+String(i)]=b_data[i-1];
          responseDoc["V"+String(i)]=Voltage[i-1];
        }
        // 将JSON数据序列化为字符串
        String jsonResponse;
        serializeJson(responseDoc, jsonResponse);
        // 发送响应
        server.send(200, "application/json", jsonResponse);
    }else if (func=="grap"){
      String state=doc["state"];
      commands::grap(id,state!="true");
    }else if(func=="test_func"){
      if(main_func_handler==nullptr){
        xTaskCreatePinnedToCore(main_func, "test", 4096, NULL, 5, &main_func_handler,1);
      }
    }else if (func=="All_Z"){
      String state=doc["state"];
      if(state=="true"){
        commands::all_z_to_height(220);
      }
      else{
        commands::all_z_to_height(5);
      }
    }else if (func=="ALL_TO_START"){
      commands::all_x_y_to_setup_point();
    }else if (func=="ALL_TO_RELEASE"){
      commands::all_x_y_to_release_point();
    }else if (func=="ALL_LASER"){
      String state=doc["state"];
      if(state=="true"){
        commands::all_laser(true);
      }else{
        commands::all_laser(false);
      }
    }else if (func=="ALL_rezero"){
      String axis=doc["axis"];
      if (axis=="X"){
        commands::all_x_rezero();
      }else if (axis=="Y"){
        commands::all_y_rezero();
      }
    }else if (func=="ALL_GRAP"){
      String state=doc["state"];
      if(state=="true"){
        commands::all_grap(true);
      }else{
        commands::all_grap(false);
      }
    }else if (func=="MIAN_FUNC"){
      if(main_func_handler==nullptr){
        xTaskCreatePinnedToCore(main_func, "main_func", 4096, NULL, 5, &main_func_handler,1);
      }
    }

}
void web_setup(){
    // 创建wifi访问点，设置名称和密码
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    // 获取wifi访问点的IP地址
    IPAddress apIP = WiFi.softAPIP();
    server.on("/rec", handlerec);
    server.on("/test", handletest);
    server.on("/", handleRoot); 
    server.on("/cali",handlecali);
    server.on("/data",handleData);//处理请求
    server.on("/favicon.ico",[]{});
    server.onNotFound(handleNotFound); // 其他不存在的请求
    server.begin();
}
#endif 