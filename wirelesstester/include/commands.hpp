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
#include <list>
#include <vector>





std::map<String, String> help_map;

namespace commands{
    bool wait_package(String name,int timeout=1000,bool need_show=true){
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
    bool test(int id){
        esp_now_send_package(package_type_request,id,"test",nullptr,0);
        if(wait_package("test")) return 0;
        receive_datas.erase("test");
        return 1;
    }
    float get_x(int id){
        esp_now_send_package(package_type_request,id,"get_x",nullptr,0);
        if(wait_package("get_x")) return 0;
        float data = *(float*)receive_datas["get_x"].data;
        receive_datas.erase("get_x");
        return data ;
    }
    float get_y(int id){
        esp_now_send_package(package_type_request,id,"get_y",nullptr,0);
        if(wait_package("get_y")) return 0;
        float data = *(float*)receive_datas["get_y"].data;
        receive_datas.erase("get_y");
        return data ;
    }

    float get_z(int id){
        esp_now_send_package(package_type_request,id,"get_z",nullptr,0);
        if(wait_package("get_z")) return 0;
        float data=*(float*)receive_datas["get_z"].data;
        receive_datas.erase("get_z");
        return data ;
    }
    float get_servo_temp(int id){
        esp_now_send_package(package_type_request,id,"get_servo_temp",nullptr,0);
        if(wait_package("get_servo_temp")) return 0;
        float data=*(float*)receive_datas["get_servo_temp"].data;
        receive_datas.erase("get_servo_temp");
        return data;
    }

    struct recognition_unit_data{
        float front_distance=0;
        float back_distance=0;
    };
    recognition_unit_data get_recognition_unit(int id){
        char pra='S';
        esp_now_send_package(package_type_request,id,"get_sensor_distance",(uint8_t*)(&pra),1);
        recognition_unit_data data;
        if(wait_package("get_sensor_distance")) return data;
        data.front_distance=*(float*)receive_datas["get_sensor_distance"].data;
        data.back_distance=*(float*)(receive_datas["get_sensor_distance"].data+4);
        receive_datas.erase("get_sensor_distance");
        return data;
    }

    float get_voltage(int id){
        esp_now_send_package(package_type_request,id,"get_voltage",nullptr,0);
        if(wait_package("get_voltage")) return 0;
        float data=*(float*)receive_datas["get_voltage"].data;
        receive_datas.erase("get_voltage");
        return data;
    }
    bool rezero(int id){
        esp_now_send_package(package_type_request,id,"auto_rezero",nullptr,0);
        if(wait_package("auto_rezero")) return 0;
        receive_datas.erase("auto_rezero");
        return 1;
    }
    void move_y(int id ,float point,int speed=1000,int acce=220){
        uint8_t data[12];
        float _point=point;
        float _speed=speed;
        float _acce=acce;
        memcpy(data,&_point,4);
        memcpy(data+4,&_speed,4);
        memcpy(data+8,&_acce,4);
        esp_now_send_package(package_type_request,id,"move_y",data,12);
        if(wait_package("move_y")) return ;
        receive_datas.erase("move_y");
        return ; 
    }
    void move_to_y(int id ,float point,int speed=1000,int acce=220){
        uint8_t data[12];
        float _point=point;
        float _speed=speed;
        float _acce=acce;
        memcpy(data,&_point,4);
        memcpy(data+4,&_speed,4);
        memcpy(data+8,&_acce,4);
        esp_now_send_package(package_type_request,id,"move_to_y",data,12);
        if(wait_package("move_to_y")) return ;
        receive_datas.erase("move_to_y");
        return ;
    }
    void move_z(int id ,float point,int speed=1000,int acce=250){
        uint8_t data[12];
        float _point=point;
        float _speed=speed;
        float _acce=acce;
        memcpy(data,&_point,4);
        memcpy(data+4,&_speed,4);
        memcpy(data+8,&_acce,4);
        esp_now_send_package(package_type_request,id,"move_z",data,12);
        if(wait_package("move_z")) return ;
        receive_datas.erase("move_z");
        return ;
    }
    void move_to_z(int id ,float point,int speed=1000,int acce=250,bool need_wait=true){
        uint8_t data[12];
        float _point=point;
        float _speed=speed;
        float _acce=acce;
        memcpy(data,&_point,4);
        memcpy(data+4,&_speed,4);
        memcpy(data+8,&_acce,4);
        esp_now_send_package(package_type_request,id,"move_to_z",data,12);
        if(!need_wait) return ;
        if(wait_package("move_to_z")) return ;
        receive_datas.erase("move_to_z");
        return ;
    }
    void move_x(int id ,float point,int speed=250,int acce=220){
        uint8_t data[12];
        float _point=point;
        float _speed=speed;
        float _acce=acce;
        memcpy(data,&_point,4);
        memcpy(data+4,&_speed,4);
        memcpy(data+8,&_acce,4);
        esp_now_send_package(package_type_request,id,"move_x",data,12);
        if(wait_package("move_x")) return ;
        receive_datas.erase("move_x");
        return ;
    }
    void move_to_x(int id ,float point,int speed=800,int acce=220){
        uint8_t data[12];
        float _point=point;
        float _speed=speed;
        float _acce=acce;
        memcpy(data,&_point,4);
        memcpy(data+4,&_speed,4);
        memcpy(data+8,&_acce,4);
        esp_now_send_package(package_type_request,id,"move_to_x",data,12);
        if(wait_package("move_to_x")) return ;
        receive_datas.erase("move_to_x");
        return ;
    }
    void grap(int id,bool state,int delay_time=700){
        bool _state=state;
        esp_now_send_package(package_type_request,id,"grap",(uint8_t*)&_state,1);
        delay(delay_time);
        if(wait_package("grap")) return ;
        receive_datas.erase("grap");
        return ;
    }
    void enable(int id,char axis,bool state){
        int _id=id;
        char pra=axis;
        bool pra2=state;

        uint8_t datas[2]={pra,pra2};
        esp_now_send_package(package_type_request,_id,"enable",datas,2);
    }
    void buzz(int id,bool state){
        bool _state=state;
        esp_now_send_package(package_type_request,id,"buzz",(uint8_t*)&_state,1);
        if(wait_package("buzz")) return ;
        receive_datas.erase("buzz");
        return ;
    }
    bool is_moveing(int id,char axis){
        char _axis=axis;
        esp_now_send_package(package_type_request,id,"is_moveing",(uint8_t*)&_axis,1);
        if(wait_package("is_moveing")) return 1;
        bool state=*(bool*)receive_datas["is_moveing"].data;
        receive_datas.erase("is_moveing");
        return state;
    }
    void wait (int _id,char axis,int dey_time=300,int wHZ=4){
        delay(dey_time);
        while(is_moveing(_id,axis)) delay(1000/wHZ);
    }
    void laser(int id,bool state){
        bool _state=state;
        esp_now_send_package(package_type_request,id,"laser",(uint8_t*)&_state,1);
        if(wait_package("laser")) return ;
        receive_datas.erase("laser");
        return ;
    }
    void set_now(int id,float point){
        float _point=point;
        esp_now_send_package(package_type_request,id,"set_now_location",(uint8_t*)&_point,4);
        if(wait_package("set_now_location")) return ;
        receive_datas.erase("set_now_location");
        return ;
    }

    struct point{
        float x;
        float y;
    };

    std::map<int,point> weight_points={
        {1,{350.481,2375}},
        {2,{350.481,3125}},
        {3,{675.24,2562.5}},
        {4,{675.24,2937.5}},
        {5,{1000,2000}},
        {6,{1000,2375}},
        {7,{1000,3125}},
        {8,{1000,3500}},
        {9,{1324.76,2562.5}},
        {10,{1324.76,2937.5}},
        {11,{1649.519,2375}},
        {12,{1649.519,3125}},
    };
    //识别到的砝码表
    std::list<point> now_weights_point;
    std::list<int> now_weights_id;

    std::vector<commands::point> ID8Crossbeam_weight;
    commands::point ID7Crossbeam_weight;
    std::vector<commands::point> ID6Crossbeam_weight;
    bool get_weight_point(){
        recognition_unit_data datas[6];
        for(int i=11;i<=16;++i){
            datas[i-11]=get_recognition_unit(i);
        }
        auto have_weight=[](float raw)->bool{
            if(raw>50&&raw<350) return true;
            else return false;
        };
        now_weights_point.clear();
        now_weights_id.clear();
        //解析识别到的砝码
        if(have_weight(datas[0].front_distance)) {now_weights_point.push_back(weight_points[6]); now_weights_id.push_back(6);}
        if(have_weight(datas[0].back_distance)) {now_weights_point.push_back(weight_points[5]); now_weights_id.push_back(5);}
        if(have_weight(datas[1].front_distance)) {now_weights_point.push_back(weight_points[3]);now_weights_id.push_back(3);}
        if(have_weight(datas[1].back_distance)) {now_weights_point.push_back(weight_points[1]);now_weights_id.push_back(1);}
        if(have_weight(datas[2].front_distance)) {now_weights_point.push_back(weight_points[4]);now_weights_id.push_back(4);}
        if(have_weight(datas[2].back_distance)) {now_weights_point.push_back(weight_points[2]);now_weights_id.push_back(2);}
        if(have_weight(datas[3].front_distance)) {now_weights_point.push_back(weight_points[7]);now_weights_id.push_back(7);}
        if(have_weight(datas[3].back_distance)) {now_weights_point.push_back(weight_points[8]);now_weights_id.push_back(8);}
        if(have_weight(datas[4].front_distance)) {now_weights_point.push_back(weight_points[10]);now_weights_id.push_back(10);}
        if(have_weight(datas[4].back_distance)) {now_weights_point.push_back(weight_points[12]);now_weights_id.push_back(12);}
        if(have_weight(datas[5].front_distance)) {now_weights_point.push_back(weight_points[9]);now_weights_id.push_back(9);}
        if(have_weight(datas[5].back_distance)) {now_weights_point.push_back(weight_points[11]);now_weights_id.push_back(11);}
        //如果砝码数量为6个认为识别正确
        if (now_weights_point.size()==6){
            ID8Crossbeam_weight.clear();
            ID7Crossbeam_weight={0,0};
            ID6Crossbeam_weight.clear();
            auto find_weight=[](int id)->bool{
                if(std::find(commands::now_weights_id.begin(),commands::now_weights_id.end(),id)!=commands::now_weights_id.end()){
                    return true;
                }else{
                    return false;
                }
            };
            //6号横梁抓取砝码
            if(find_weight(1)){
                ID6Crossbeam_weight.push_back(commands::weight_points[1]);
            }else{
                ID6Crossbeam_weight.push_back(commands::weight_points[3]);
            }
            if(find_weight(11)){
                ID6Crossbeam_weight.push_back(commands::weight_points[11]);
            }else{
                ID6Crossbeam_weight.push_back(commands::weight_points[9]);
            }

            //确保Y小的在前
            if(ID6Crossbeam_weight[0].y>ID6Crossbeam_weight[1].y){
                std::swap(ID6Crossbeam_weight[0],ID6Crossbeam_weight[1]);
            }

            //8号横梁抓取砝码
            if(find_weight(2)){
                ID8Crossbeam_weight.push_back(commands::weight_points[2]);
            }else{
                ID8Crossbeam_weight.push_back(commands::weight_points[4]);
            }
            if(find_weight(12)){
                ID8Crossbeam_weight.push_back(commands::weight_points[12]);
            }else{
                ID8Crossbeam_weight.push_back(commands::weight_points[10]);
            }
            //确保Y小的在前
            if(ID8Crossbeam_weight[0].y>ID8Crossbeam_weight[1].y){
                std::swap(ID8Crossbeam_weight[0],ID8Crossbeam_weight[1]);
            }

            //如果有两个相同的Y,认为该横梁最快抓取，7号就抓该侧砝码
            if(ID6Crossbeam_weight[0].y==ID6Crossbeam_weight[1].y){
                ID7Crossbeam_weight=find_weight(5)?commands::weight_points[5]:commands::weight_points[6];
            }else if(ID8Crossbeam_weight[0].y==ID8Crossbeam_weight[1].y){
                ID7Crossbeam_weight=find_weight(7)?commands::weight_points[7]:commands::weight_points[8];
            }else{//默认抓取6号这侧的砝码
                ID7Crossbeam_weight=find_weight(5)?commands::weight_points[5]:commands::weight_points[6];
            }
            return true;
        }
        //否则认为识别失败
        return false;
    }

    void all_z_to_height(float height){
        Serial.println("all_z_to_height");
        for(int i=1;i<=5;++i){
            commands::move_to_z(i,height);
        }
    }
    void all_x_y_to_setup_point(){
        Serial.println("all_x_y_to_setup_point");
        move_to_y(6,1000);
        move_to_y(7,1550);
        move_to_y(8,2100);
        move_to_x(1,1487);
        move_to_x(2,513);
        move_to_x(3,1000);
        move_to_x(4,1487);
        move_to_x(5,513);
    }
    void all_x_y_to_release_point(){
        Serial.println("all_x_y_to_release_point");
        move_to_y(6,245);
        move_to_y(7,2750);
        move_to_y(8,3755);

        move_to_x(1,1755);
        move_to_x(2,245);
        move_to_x(3,1000);
        move_to_x(4,1755);
        move_to_x(5,245);
    }
    void all_laser(bool state){
        Serial.println("all_laser");
        for(int i=1;i<=5;++i){
            commands::laser(i,state);
        }
    }
    void all_grap(bool state){
        for(int i=1;i<=5;++i){
            commands::grap(i,state,100);
        }
    }
    void all_x_rezero(){
        for(int i=1;i<=5;++i){
            commands::rezero(i);
        }
    }
    void all_y_rezero(){
        for(int i=6;i<=8;++i){
            commands::rezero(i);
        }
    }
}



















//↓↓↓↓↓下面是用于命令行的函数↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓


bool wait_package(String name,int timeout=1000,bool need_show=true){
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
        esp_now_send_package(package_type_request,pra,"online_test",nullptr,0);
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
            esp_now_send_package(package_type_request,id,"online_test",nullptr,0);
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
        esp_now_send_package(package_type_request,_id,"get_sensor_distance",(uint8_t*)(&pra),1);
        //等待响应
        if(wait_package("get_sensor_distance")) return 0;
        //解析响应
        shell.print(F("distance:"));
        float dis1=*(float*)receive_datas["get_sensor_distance"].data;
        float dis2=*(float*)(receive_datas["get_sensor_distance"].data+4);
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
    if (argc < 2 || argc > 3) {
        shell.println("bad argument count");
        shell.println(help_map["rezero"]);
        return -1;
    }
    if(argc==2){
        int pra=strtod(args[1],NULL);
        if(pra>20||pra<0){
            shell.println(F("data error"));
            return -1;
        }
        esp_now_send_package(package_type_request,pra,"auto_rezero",nullptr,0);
        if(wait_package("auto_rezero")) return 0;
        receive_datas.erase("auto_rezero");
        shell.println(F("rezero success"));
    }else if(argc==3){
        int pra=strtod(args[1],NULL);
        int pra2=strtod(args[2],NULL);
        for(int i=pra;i<=pra2;i++){
            commands::rezero(i);
            delay(100);
        }
    };
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
    int _id=0;
    float pra=0;
    float pra2=0;
    float pra3=0;
    if (argc != 5 && argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_y"]);
        return -1;
    }
    if(argc==5){
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=strtod(args[3],NULL);
        pra3=strtod(args[4],NULL);
    }else if (argc==3)
    {
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=1000;
        pra3=220;
    }
    

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
    int _id=0;
    float pra=0;
    float pra2=0;
    float pra3=0;
    if (argc != 5 && argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_y"]);
        return -1;
    }
    if(argc==5){
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=strtod(args[3],NULL);
        pra3=strtod(args[4],NULL);
    }else if (argc==3)
    {
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=1000;
        pra3=220;
    }
    if(_id<6||_id>8){
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
    int _id=0;
    float pra=0;
    float pra2=0;
    float pra3=0;
    if (argc != 5 && argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_y"]);
        return -1;
    }
    if(argc==5){
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=strtod(args[3],NULL);
        pra3=strtod(args[4],NULL);
    }else if (argc==3)
    {
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=800;
        pra3=230;
    }
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
    int _id=0;
    float pra=0;
    float pra2=0;
    float pra3=0;
    if (argc != 5 && argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_y"]);
        return -1;
    }
    if(argc==5){
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=strtod(args[3],NULL);
        pra3=strtod(args[4],NULL);
    }else if (argc==3)
    {
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=800;
        pra3=230;
    }
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

int move_to_z(int argc, char** args){    int _id=0;
    float pra=0;
    float pra2=0;
    float pra3=0;
    if (argc != 5 && argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_y"]);
        return -1;
    }
    if(argc==5){
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=strtod(args[3],NULL);
        pra3=strtod(args[4],NULL);
    }else if (argc==3)
    {
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=250;
        pra3=230;
    }
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
    int _id=0;
    float pra=0;
    float pra2=0;
    float pra3=0;
    if (argc != 5 && argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["move_to_y"]);
        return -1;
    }
    if(argc==5){
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=strtod(args[3],NULL);
        pra3=strtod(args[4],NULL);
    }else if (argc==3)
    {
        _id=strtod(args[1],NULL);
        pra=strtod(args[2],NULL);
        pra2=250;
        pra3=230;
    }
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

int set_now(int argc, char** args){
    if (argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["set_now"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    float pra=strtod(args[2],NULL);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"set_now_location",(uint8_t*)&pra,4);
    if(wait_package("set_now_location")) return 0;
    receive_datas.erase("set_now_location");
    shell.println(F("set now success"));
    return 0;
}
int is_moveing(int argc, char** args){
    if (argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["is_moveing"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    char axis=args[2][0];
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"is_moveing",(uint8_t*)&axis,1);
    if(wait_package("is_moveing")) return 0;
    bool state=*(bool*)receive_datas["is_moveing"].data;
    receive_datas.erase("is_moveing");
    if(state){
        shell.println(F("moveing"));
    }else{
        shell.println(F("stop"));
    }
    return 0;
}

int read_servo_angle(int argc, char** args){
    if (argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["read_servo_angle"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    char axis=args[2][0];
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"read_servo_angle",(uint8_t*)&axis,1);
    if(wait_package("read_servo_angle")) return 0;
    float angle=*(float*)receive_datas["read_servo_angle"].data;
    receive_datas.erase("read_servo_angle");
    shell.println(angle);
    return 0;
}

int set_servo_angle(int argc, char** args){
    if (argc != 5) {
        shell.println("bad argument count");
        shell.println(help_map["set_servo_angle"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    char axis=args[2][0];
    bool state=strtod(args[3],NULL);
    float angle=strtod(args[4],NULL);
    if(_id>20||_id<0){
        shell.println(F("data error"));
        return -1;
    }
    uint8_t data[6]{axis,state,0,0,0,0};
    memcpy(data+2,&angle,4);
    esp_now_send_package(package_type_request,_id,"set_servo_angle",(uint8_t*)&data,6);
    if(wait_package("set_servo_angle")) return 0;
    receive_datas.erase("set_servo_angle");
    shell.println(F("set servo angle success"));
    return 0;
}
int buzz(int argc, char** args){
    if (argc != 3) {
        shell.println("bad argument count");
        shell.println(help_map["buzz"]);
        return -1;
    }
    int _id=strtod(args[1],NULL);
    bool state=strtod(args[2],NULL);
    if(_id>8||_id<6){
        shell.println(F("data error"));
        return -1;
    }
    esp_now_send_package(package_type_request,_id,"buzz",(uint8_t*)&state,1);
    if(wait_package("buzz")) return 0;
    receive_datas.erase("buzz");
    shell.println(F("buzz success"));
    return 0;
}
int setupZ(int argc, char** args){
    if(argc!=2){
        shell.println("bad argument count");
        shell.println(help_map["setupZ"]);
        return -1;
    }
    float pra=strtod(args[1],NULL);
    for(int i=0;i<5;i++){
        commands::move_to_z(i,pra,250,220);
        delay(100);
    }
    return 0;
}
int get_weight(int argc, char** args){
    shell.println("geting weight...");
    if(commands::get_weight_point()){
        shell.println("get weight success");
        shell.println("weight_id:");
        for(auto item =commands::now_weights_id.begin();item!=commands::now_weights_id.end();++item){
            shell.print(*item);
            shell.print(",");
        }
        shell.println();
    }else{
        shell.println("get weight failed");
        shell.println("get points:");
        for(auto item =commands::now_weights_id.begin();item!=commands::now_weights_id.end();++item){
            shell.print(*item);
            shell.print(",");
        }
        shell.println();
    }
    return 0;
}

#endif