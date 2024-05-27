/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-20 22:27:24
 */
#ifndef commands_hpp
#define commands_hpp
#include <Arduino.h>
#include "SimpleSerialShell.h"
#include "ESPNOW.hpp"
#include <stdlib.h>
#include <map>

std::map<String, String> help_map;




bool wait_package(String name,int timeout=3000,bool need_show=true){
    int i=0;
    while(receive_datas.find(name)==receive_datas.end()){
      delay(10);
      i++;
      if(i>timeout/10){
        if(need_show) shell.println(F("time out"));
        return true;
      }
    }
    return false;
}


int clear(int /*argc*/ = 0, char** /*argv*/ = NULL){
    shell.print('\f');
    return 0;
}

int showinfo(int /*argc*/ = 0, char** /*argv*/ = NULL)
{
    shell.println(F( "Running " __FILE__ ",\nBuilt " __DATE__));
    shell.print(">");
    return 0;
};
//在线测试
int online_test(int argc, char** args){

    if (argc==2){
        int pra=strtod(args[1],NULL);
        esp_now_send_package(package_type_request,pra,"online_test",nullptr,0,receive_MACAddress);
        //等待响应
        if(wait_package("online_test")) return 0;
        //解析响应
        if(receive_datas["online_test"].id==pra){
            shell.print(F("ID:"));
            shell.print(pra);
            shell.println(F(" is online"));
            //缓存map清除响应包
            receive_datas.erase("online_test");
        }
    }else if(argc==3){
        auto test_func=[&](int id){
            esp_now_send_package(package_type_request,id,"online_test",nullptr,0,receive_MACAddress);
            //等待响应
            if(wait_package("online_test",1000,false)){
                shell.print(F("ID:"));
                shell.print(id);
                shell.println(F(" is #offline#"));
            }else{
                if(receive_datas["online_test"].id==id){
                    shell.print(F("ID:"));
                    shell.print(id);
                    shell.println(F(" is online"));
                    //缓存map清除响应包
                    receive_datas.erase("online_test");
                }
            }
        };
        int start_id=strtod(args[1],NULL);
        int end_id=strtod(args[2],NULL);
        for(int i=start_id;i<=end_id;i++){
            test_func(i);
            delay(10);
        }
    }else{
        shell.println("bad argument count");
        shell.println(help_map["test"]);
        return -1;

    }
    return -1;
}
//获取Y坐标
int get_y(int argc, char** args){
    if (argc != 2) {
        shell.println("bad argument count");
        shell.println(help_map["getY"]);
        return -1;
    }
    int pra=strtod(args[1],NULL);
    if(pra>20||pra<0){
        shell.println(F("data error"));
        return -1;
    }

    esp_now_send_package(package_type_request,pra,"get_y",nullptr,0);
    //等待响应
    if(wait_package("get_y")) return 0;
    //解析响应
    shell.print(F("Y:"));
    shell.println(*(float*)receive_datas["get_y"].data);
    //缓存map清除响应包
    receive_datas.erase("get_y");
    return 0;
}
//获取X坐标
int get_x(int argc, char** args){
    if (argc != 2) {
        shell.println("bad argument count");
        shell.println(help_map["getX"]);
        return -1;
    }
    int pra=strtod(args[1],NULL);
    if(pra>20||pra<0){
        shell.println(F("data error"));
        return -1;
    }

    esp_now_send_package(package_type_request,pra,"get_x",nullptr,0);
    //等待响应
    if(wait_package("get_x")) return 0;
    //解析响应
    shell.print(F("X:"));
    shell.println(*(float*)receive_datas["get_x"].data);
    //缓存map清除响应包
    receive_datas.erase("get_x");
    return 0;
}
//获取Z坐标
int get_z(int argc, char** args){
    if (argc != 2) {
        shell.println("bad argument count");
        shell.println(help_map["getZ"]);
        return -1;
    }
    int pra=strtod(args[1],NULL);
    if(pra>20||pra<0){
        shell.println(F("data error"));
        return -1;
    }

    esp_now_send_package(package_type_request,pra,"get_z",nullptr,0);
    //等待响应
    if(wait_package("get_z")) return 0;
    //解析响应
    shell.print(F("Z:"));
    shell.println(*(float*)receive_datas["get_z"].data);
    //缓存map清除响应包
    receive_datas.erase("get_z");
    return 0;
}

//获取超声波传感器距离
int get_sensor(int argc, char** args){
    if (argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["getSensor"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    char pra=args[2][0];
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    if(pra=='S'){
        esp_now_send_package(package_type_request,_id,"get_sensor_distance",nullptr,0);
        //等待响应
        if(wait_package("get_sensor_distance")) return 0;
        //解析响应
        shell.print(F("distance:"));
        float dis1=*(float*)receive_datas["get_sensor_distance"].data;
        float dis2=*(float*)receive_datas["get_sensor_distance"].data+4;
        shell.print(dis1);
        shell.print(F("mm"));
        shell.print(F("  "));
        shell.print(dis2);
        shell.println(F("mm"));
    }else{
        esp_now_send_package(package_type_request,_id,"get_sensor_distance",nullptr,0);
        //等待响应
        if(wait_package("get_sensor_distance")) return 0;
        //解析响应
        shell.print(F("distance:"));
        shell.println(*(float*)receive_datas["get_sensor_distance"].data);
    }
    //缓存map清除响应包
    receive_datas.erase("get_sensor_distance");
    return 0;
}
int get_voltage(int argc, char** args){
    if (argc != 2) {
        shell.println("bad argument count");
        shell.println(help_map["getVoltage"]);
        return -1;
    }
    int pra=strtod(args[1],NULL);
    if(pra>20||pra<0){
        shell.println(F("data error"));
        return -1;
    }

    esp_now_send_package(package_type_request,pra,"get_voltage",nullptr,0);
    //等待响应
    if(wait_package("get_voltage")) return 0;
    //解析响应
    shell.print(F("voltage:"));
    shell.println(*(float*)receive_datas["get_voltage"].data);
    //缓存map清除响应包
    receive_datas.erase("get_voltage");
    return 0;
}
int get_zero_point(int argc, char** args){
    if (argc != 2) {
        shell.println("bad argument count");
        shell.println(help_map["get_zero_point"]);
        return -1;
    }
    int pra=strtod(args[1],NULL);
    if(pra>20||pra<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,pra,"get_zero_point",nullptr,0);
    //等待响应
    if(wait_package("get_zero_point")) return 0;
    //解析响应
    shell.print(F("zero_point:"));
    shell.println(*(float*)receive_datas["get_zero_point"].data);
    //缓存map清除响应包
    receive_datas.erase("get_zero_point");
    return 0;    
}

int rezero(int argc, char** args){
    if (argc != 2) {
        shell.println("bad argument count");
        shell.println(help_map["rezero"]);
        return -1;
    }
    int pra=strtod(args[1],NULL);
    if(pra>20||pra<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,pra,"auto_rezero",nullptr,0);
    if(wait_package("auto_rezero")) return 0;
    receive_datas.erase("auto_rezero");
    shell.println(F("rezero success"));
    return 0;
}


int set_zero_point(int argc, char** args){
    if (argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["set_zero_point"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    float pra=strtod(args[2],NULL);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }   
    esp_now_send_package(package_type_request,_id,"set_zero_point",(uint8_t*)&pra,4);
    if(wait_package("set_zero_point")) return 0;
    receive_datas.erase("set_zero_point");
    shell.println(F("set zero point success"));
    return 0;
}
int move_to_y(int argc, char** args){
    if (argc != 5) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_y"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    float pra=strtod(args[2],NULL);
    float pra2=strtod(args[3],NULL);
    float pra3=strtod(args[4],NULL);
    uint8_t datas[12];
    memcpy(datas,&pra,4);
    memcpy(datas+4,&pra2,4);
    memcpy(datas+8,&pra3,4);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"move_to_y",datas,12);
    if(wait_package("move_to_y")) return 0;
    receive_datas.erase("move_to_y");
    shell.println(F("moveing ......"));

    return 0;
}

int move_y(int argc, char** args){
    if (argc != 5) {
        shell.println("bad argument count");
        shell.println(help_map["move_y"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    float pra=strtod(args[2],NULL);
    float pra2=strtod(args[3],NULL);
    float pra3=strtod(args[4],NULL);
    if(_id!=6){
        shell.println(F("data error"));
        shell.println(_id);
        return -1;
    }
    uint8_t datas[12];
    memcpy(datas,&pra,4);
    memcpy(datas+4,&pra2,4);
    memcpy(datas+8,&pra3,4);
    esp_now_send_package(package_type_request,_id,"move_y",datas,12);
    if(wait_package("move_y")) return 0;
    receive_datas.erase("move_y");
    shell.println(F("moveing ......"));
    return 0;
}


int move_to_x(int argc, char** args){
    if (argc != 5) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_x"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    float pra=strtod(args[2],NULL);
    float pra2=strtod(args[3],NULL);
    float pra3=strtod(args[4],NULL);
    uint8_t datas[12];
    memcpy(datas,&pra,4);
    memcpy(datas+4,&pra2,4);
    memcpy(datas+8,&pra3,4);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }  
    esp_now_send_package(package_type_request,_id,"move_to_x",datas,12);
    if(wait_package("move_to_x")) return 0;
    receive_datas.erase("move_to_x");
    shell.println(F("moveing ......"));
    return 0;
}

int move_x(int argc, char** args){
    if (argc != 5) {
        shell.println("bad argument count");
        shell.println(help_map["move_x"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    float pra=strtod(args[2],NULL);
    float pra2=strtod(args[3],NULL);
    float pra3=strtod(args[4],NULL);
    uint8_t datas[12];
    memcpy(datas,&pra,4);
    memcpy(datas+4,&pra2,4);
    memcpy(datas+8,&pra3,4);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"move_x",datas,12);
    if(wait_package("move_x")) return 0;
    receive_datas.erase("move_x");
    shell.println(F("moveing ......"));
    return 0;
}

int move_to_z(int argc, char** args){
    if (argc != 5) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_z"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    float pra=strtod(args[2],NULL);
    float pra2=strtod(args[3],NULL);
    float pra3=strtod(args[4],NULL);
    uint8_t datas[12];
    memcpy(datas,&pra,4);
    memcpy(datas+4,&pra2,4);
    memcpy(datas+8,&pra3,4);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"move_to_z",datas,12);
    if(wait_package("move_to_z")) return 0;
    receive_datas.erase("move_to_z");
    shell.println(F("moveing ......"));
    return 0;
}

int move_z(int argc, char** args){
    if (argc != 5) {
        shell.println("bad argument count");
        shell.println(help_map["move_z"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    float pra=strtod(args[2],NULL);
    float pra2=strtod(args[3],NULL);
    float pra3=strtod(args[4],NULL);
    uint8_t datas[12];
    memcpy(datas,&pra,4);
    memcpy(datas+4,&pra2,4);
    memcpy(datas+8,&pra3,4);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"move_z",datas,12);
    if(wait_package("move_z")) return 0;
    receive_datas.erase("move_z");
    shell.println(F("moveing ......"));
    return 0;
}

int grap(int argc, char** args){
    if (argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["grap"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    bool pra=strtod(args[2],NULL);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"grap",(uint8_t*)&pra,1);
    if(wait_package("grap")) return 0;
    receive_datas.erase("grap");
    shell.println(F("grap success"));
    return 0;
}

int enable(int argc, char** args){
    if (argc != 4) {
        shell.println("bad argument count");
        shell.println(help_map["enable"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    char pra=args[2][0];
    uint8_t pra2=strtod(args[3],NULL);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    uint8_t datas[2]={pra,pra2};
    esp_now_send_package(package_type_request,_id,"enable",datas,2);
    if(wait_package("enable")) return 0;
    receive_datas.erase("enable");
    shell.println(F("enable success"));
    return 0;
}
int laser(int argc, char** args){
    if (argc != 3) {
        shell.println("bad argument count");
        return -1;
    }
    int _id=strtod(args[1],NULL);
    bool state=strtod(args[2],NULL);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"laser",(uint8_t*)&state,1);
    if(wait_package("laser")) return 0;
    receive_datas.erase("laser");
    shell.println(F("laser success"));
    return 0;
}
#endif