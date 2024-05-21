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



bool wait_package(String name){
    int i=0;
    while(receive_datas.find(name)==receive_datas.end()){
      delay(10);
      i++;
      if(i>300){
        shell.println(F("time out"));
        return true;
      }
    }
    return false;
}

bool check_arg(int argc, char** args, int arg_num){
    if (argc != arg_num) {
        shell.println("bad argument count");
        return -1;
    }
    int pra=strtod(args[1],NULL);
    if(pra>20||pra<0){
        shell.println(F("data error"));
        return -1;
    }
}

int showinfo(int /*argc*/ = 0, char** /*argv*/ = NULL)
{
    shell.println(F( "Running " __FILE__ ",\nBuilt " __DATE__));
    shell.print(">");
    return 0;
};
//在线测试
int online_test(int argc, char** args){
    //检查参数
    check_arg(argc,args,2);
    int pra=strtod(args[1],NULL);
    //发送请求
    esp_now_send_package(package_type_request,pra,"online_test",nullptr,0,receive_MACAddress);
    //等待响应
    if(wait_package("online_test")) return 0;
    //解析响应
    if(wait_package("online_test").id==pra){
        shell.print(F("ID:"));
        shell.print(pra);
        shell.println(F(" is online"));
    }
    //缓存map清除响应包
    wait_package.erase("online_test");
    return 0;
}
//获取横梁主控Y坐标
int get_y(int /*argc*/ = 0, char** /*argv*/ = NULL){
    if (argc != 2) {
        shell.println("bad argument count");
        return -1;
    }
    if(pra>20||pra<0){
        shell.println(F("data error"));
        return -1;
    }

    esp_now_send_package(package_type_request,pra,"get_y",nullptr,0,receive_MACAddress);
    
}

#endif