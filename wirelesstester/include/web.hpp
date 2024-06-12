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
#define WIFI_SSID "CQUPTIMTA2"        //WiFi名称
#define WIFI_PASSWORD "znzzjsxh"                 //WiFi密码



//创建一个异步Web服务器
WebServer server(80);  

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
void handleData(){
    
    // 检查是否有有效的HTTP请求体
    if (server.hasArg("plain") == false) {
    server.send(400, "text/plain", "Body not received");
    return;
    }

    // 获取HTTP请求体
    String body = server.arg("plain");

    // 创建JSON文档对象
    StaticJsonDocument<400> doc;
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
    if(func=="grap_update_data"){
        StaticJsonDocument<400> responseDoc;

        // 填充JSON数据
        responseDoc["Zposition"] = 10; // 这里可以替换为实际数据
        responseDoc["Xposition"] = 20; // 这里可以替换为实际数据
        responseDoc["servo_temp"] = 30; // 这里可以替换为实际数据

        // 将JSON数据序列化为字符串
        String jsonResponse;
        serializeJson(responseDoc, jsonResponse);

        // 发送响应
        server.send(200, "application/json", jsonResponse);
    }else if(func=="crossbeam_update_data"){

    }else if(func=="rezero"){

    }else if(func=="load"){

    }else if(func=="move_to_axis"){

    }else if(func=="laser"){
        
    }


}
void web_setup(){
    // 创建wifi访问点，设置名称和密码
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    // 获取wifi访问点的IP地址
    IPAddress apIP = WiFi.softAPIP();
    // 启动DNS服务器，将所有域名解析到wifi访问点的IP地址，实现重定向功能
    server.on("/rec", handlerec); // 根目录请求
    server.on("/test", handletest); // 根目录请求
    server.on("/", handleRoot); // 根目录请求
    server.on("/data",handleData);//处理请求
    server.onNotFound(handleNotFound); // 其他不存在的请求
    server.begin();
}
#endif 