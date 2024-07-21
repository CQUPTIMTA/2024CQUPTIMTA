/*
 * @Description:
 * @Author: qingmeijiupiao
 * @Date: 2024-02-01 13:16:20
 */
#include <Arduino.h>
#include "ESP32FLASHEEPROM.hpp"
#include "EMMC42V5.hpp"
#include "PINS.hpp"
#include "ESPNOW.hpp"
#define GEARTEETH 30
int ID=0;
NVSDATA DATA;

namespace CROSSBEAM {
    // SemaphoreHandle_t motorMutex = xSemaphoreCreateMutex();
    HardwareSerial motor_ser(2);
    EMMC42V5 left_motor(&motor_ser,1);
    EMMC42V5 right_motor(&motor_ser,2);
	float taget_location_y = 0;
	//获取现在的Y位置,单位是mm
	float get_now_location_y() {

        double dis1=0;
        for (int i = 0; i < 10; i++){
            int64_t m1=-1*left_motor.read_current_location();
            int64_t m2=right_motor.read_current_location();
            double dis1=double(m1+m2)*GEARTEETH*2.0*PI/2*double(65535);
			if (dis1 < 4000 && dis1 >=0) {
                if(dis1==0 && taget_location_y!=0) {
                    continue;
                }
				return dis1 + DATA.Y_ZERO_POINT;
			}
            delay(10);
		}
        return taget_location_y;

        
    }
    //现在的Y位置,单位是mm
    void set_now_location_y(float y) {
        float delta=y-get_now_location_y();
        DATA.Y_ZERO_POINT=DATA.Y_ZERO_POINT+delta;
        DATA.write();
        taget_location_y=y;
    }

    //设置Y电机使能
    void Y_load(bool is_load=true) {
        left_motor.enable(is_load);
        right_motor.enable(is_load);
    }
    //等待Y轴移动完成
    void wait_to_y(float y,float delta=10) {
        int runtime=0;
        while(y-get_now_location_y()>delta && runtime<3000) {
            delay(20);
            runtime+=20;
        }
    }
    //Y轴移动到,单位是mm
    void move_to_y(float y,float speed,float acce=0,bool need_wait=true) {
        
        float now=get_now_location_y();
        float delta=y-now;
        int delta_pulse=delta*200*16/(GEARTEETH*2.0*PI);
        left_motor.pulse_control(-1*delta_pulse,speed,acce,false,true,true);

        right_motor.pulse_control(delta_pulse,speed,acce,false,true,true);
        delay(1);
        left_motor.sync();
		taget_location_y = y;
		// if(need_wait){
        //     wait_to_y(y);
        // }
    }
    //Y轴是否正在移动
    bool is_moving() {
        float taget;
        float real;
        for(int i=0;i<10;i++){
            taget=left_motor.read_target_location();
            real=left_motor.read_current_location();
            if(taget!=0&&real!=0) break;
		}
		float delata = 2.0 * GEARTEETH * PI * (taget - real) / 65535;
        return abs(delata)>20;
    }
    void rezero(){
        if(ID==7) return;
        
        REZREO_parameter pa;
        pa.mode=2;
        pa.direction=DATA.offset_dir>0?0:1;
        pa.speed=100;
        pa.timeout=10000;
        pa.limit_speed=100;
        pa.limit_current=800;
        pa.limit_time=100;
        pa.auto_rezero=0;
        CROSSBEAM::left_motor.change_parameter(0,0,pa);
        pa.direction=!pa.direction;
        CROSSBEAM::right_motor.change_parameter(0,0,pa);
        delay(500);
        CROSSBEAM::right_motor.re_zero(2,true);
        CROSSBEAM::left_motor.re_zero(2,true);
        delay(10);
        left_motor.sync();
		taget_location_y = DATA.Y_ZERO_POINT;
	}
}


namespace EspnowCallback {
    void online_test(data_package redata){
        esp_now_send_package(package_type_response,redata.id,"online_test",redata.data,redata.data_len,receive_MACAddress);
    };
    void rezero(data_package redata){
        esp_now_send_package(package_type_response,redata.id,"rezero",nullptr,0,receive_MACAddress);
        CROSSBEAM::rezero();
    }
    void move_to_y(data_package redata){
        float y=*(float*)redata.data;
        float speed=*(float*)(redata.data+4);
        float acce=*(float*)(redata.data+8);
        esp_now_send_package(package_type_response,redata.id,"move_to_y",nullptr,0,receive_MACAddress);
        CROSSBEAM::move_to_y(y,speed,acce);
        
        // while(abs(CROSSBEAM::get_now_location_y()-y)>10){
        //     delay(20);
        // }
        // char* sname="move_to_y";
        // esp_now_send_package(package_type_normal,redata.id,"action_complete",(uint8_t*)sname,strlen(sname),receive_MACAddress);
    }
    void move_y(data_package redata){
        float delta=*(float*)redata.data;
        float speed=*(float*)(redata.data+4);
        float acce=*(float*)(redata.data+8);
        float now=CROSSBEAM::get_now_location_y();
        float y=now+delta;
        CROSSBEAM::move_to_y(y,speed,acce);
        esp_now_send_package(package_type_response,redata.id,"move_y",nullptr,0,receive_MACAddress);
        // while (abs(CROSSBEAM::get_now_location_y()-y)>10){
        //     delay(20);
        // }
        // char* sname="move_y";
        // esp_now_send_package(package_type_normal,redata.id,"action_complete",(uint8_t*)sname,strlen(sname),receive_MACAddress);
    }
    void get_y(data_package redata){
        float y=CROSSBEAM::get_now_location_y();
        esp_now_send_package(package_type_response,redata.id,"get_y",(uint8_t*)&y,sizeof(y),receive_MACAddress);

    }
    void enable(data_package redata){
        char name=redata.data[0];
        if(name!='Y'){
            return;
        }
        bool is_load=(bool)redata.data[1];
        CROSSBEAM::Y_load(is_load);
        esp_now_send_package(package_type_response,redata.id,"enable",nullptr,0,receive_MACAddress);
    }
    void set_zero_point(data_package redata){
        float point=*(float*)redata.data;
        DATA.Y_ZERO_POINT=point;
        DATA.write();
        esp_now_send_package(package_type_response,redata.id,"set_zero_point",nullptr,0,receive_MACAddress);
    }
    void get_voltage(data_package redata){
        float voltage=CROSSBEAM::left_motor.read_Bus_voltage()/1000.0;
        esp_now_send_package(package_type_response,redata.id,"get_voltage",(uint8_t*)&voltage,sizeof(voltage),receive_MACAddress);
    }
    void is_moving(data_package redata){
        bool is_moving=CROSSBEAM::is_moving();
        esp_now_send_package(package_type_response,redata.id,"is_moveing",(uint8_t*)&is_moving,sizeof(is_moving),receive_MACAddress);
    }
    void set_now_location_y(data_package redata){
        float y=*(float*)redata.data;
        CROSSBEAM::set_now_location_y(y);
        esp_now_send_package(package_type_response,redata.id,"set_now_location_y",nullptr,0,receive_MACAddress);
    }
    void buzz(data_package redata){
        bool state=(bool)redata.data[0];
        digitalWrite(buzz_pin,state);
        esp_now_send_package(package_type_response,redata.id,"buzz",nullptr,0,receive_MACAddress);
    }
    void add_callbacks(){
        callback_map["online_test"]=online_test;
        callback_map["auto_rezero"]=rezero;
        callback_map["move_to_y"]=move_to_y;
        callback_map["move_y"]=move_y;
        callback_map["enable"]=enable;
        callback_map["set_zero_point"]=set_zero_point;
        callback_map["set_now_location"]=set_now_location_y;
        callback_map["get_y"]=get_y;
        callback_map["get_voltage"]=get_voltage;
        callback_map["is_moveing"]=is_moving;
        callback_map["buzz"]=buzz;

    }
}

void setup() {
    DATA.setup();
    DATA.read();
    // DATA.ID=8;
    // DATA.write();
    ID=DATA.ID;



    esp_now_setup();
    EspnowCallback::add_callbacks();
    Serial.begin(115200);
    CROSSBEAM::motor_ser.begin(115200, SERIAL_8N1, 10, 9);
    setup_pins();

    digitalWrite(17,1);
    delay(5000);
    digitalWrite(17,0);
}

void loop() {
    delay(1000);
    Serial.println(ID);
    for (int i = 0; i < DATA.ID; i++){
        digitalWrite(17,1);
        delay(300);
        digitalWrite(17,0);
        delay(300);
    }


}

/*
                                              .=%@#=.                                               
                                            -*@@@@@@@#=.                                            
                                         .+%@@@@@@@@@@@@#=                                          
                                       -#@@@@@@@* =@@@@@@@@*:                                       
                                     =%@@@@@@@@=   -@@@@@@@@@#-                                     
                                  .+@@@@@@@@@@-     .@@@@@@@@@@%=                                   
                                .+@@@@@@@@@@@@-     +@@@@@@@@@@@@@+.                                
                               +@@@@@@@@@@@@@@@    .@@@@@@@@@@@@@@@@+.                              
                             =@@@@@@@@@@@@@@@%-     =%@@%@@@@@@@@@@@@@=                             
                           -%@@@@@@@@@@@@+..     .       -@@@@@@@@@@@@@%-                           
                         .#@@@@@@@@@@@@@#       -@+       +@@@@@@@@@@@@@@#:                         
                        +@@@@@@@@@@@@@@@@+     +@@@+     =@@@@@@@@@@@@@@@@@+                        
                      :%@@@@@@@@@@@@@@@@@+    *@@@@*     =@@@@@@@@@@@@@@@@@@%-                      
                     +@@@@@@@@@@@@@@#+*+-   .#@@@@+       :+*+*@@@@@@@@@@@@@@@*                     
                   :%@@@@@@@@@@@@@@+       :%@@@@-    .-       -@@@@@@@@@@@@@@@%:                   
                  =@@@@@@@@@@@@@@@@-      -@@@@%:    .%@+      =@@@@@@@@@@@@@@@@@=                  
                 *@@@@@@@@@@@@@@@@@@.    =@@@@#.    -@@@@+    =@@@@@@@@@@@@@@@@@@@#                 
               .%@@@@@@@@@@@@@@@@@@+    +@@@@*     =@@@@%:    .#@@@@@@@@@@@@@@@@@@@%.               
              :@@@@@@@@@@@@@@@%:::.    #@@@@+     +@@@@#        .::.*@@@@@@@@@@@@@@@@-              
             -@@@@@@@@@@@@@@@%       .%@@@@=     *@@@@*     +-       *@@@@@@@@@@@@@@@@=             
            =@@@@@@@@@@@@@@@@@#.    -@@@@@-    :%@@@@=    .#@@+     +@@@@@@@@@@@@@@@@@@=            
           =@@@@@@@@@@@@@@@@@@@:    =====.     -+===:     :====     @@@@@@@@@@@@@@@@@@@@+           
          +@@@@@@@@@@@@@@@#%%#-                                     :*%%#%@@@@@@@@@@@@@@@+          
         =@@@@@@@@@@@@@@%.       ...........................              *@@@@@@@@@@@@@@@=         
        =@@@@@@@@@@@@@@@+      .#@@@@@@@@@@@@@@@@@@@@@@@@@@#     .*:      =@@@@@@@@@@@@@@@@-        
       -@@@@@@@@@@@@@@@@@=    .%@@@@@@@@@@@@@@@@@@@@@@@@@@#     :@@@-    =@@@@@@@@@@@@@@@@@@:       
      :@@@@@@@@@@@@@@@@@%.   -@@@@%+=====================:     -@@@@%    :%@@@@@@@@@@@@@@@@@@.      
      %@@@@@@@@@@@@@=-=:    =@@@@#.                           +@@@@#.      -=--%@@@@@@@@@@@@@%      
     #@@@@@@@@@@@@@:       +@@@@*      ............. .       *@@@@*             %@@@@@@@@@@@@@+     
    =@@@@@@@@@@@@@@#.     #@@@@+     +@@@@@@@@@@@@@@@#.    .#@@@@+     +#.     +@@@@@@@@@@@@@@@:    
   .@@@@@@@@@@@@@@@@-   .%@@@@=     *@@@@@@@@@@@@@@@#     :%@@@@-     *@@%:    @@@@@@@@@@@@@@@@%    
   %@@@@@@@@@@@%%%#=   :@@@@@:    .#@@@@+-----------     -@@@@@:     #@@@@=    :#%%%@@@@@@@@@@@@*   
  =@@@@@@@@@@@=       -@@@@%.    :%@@@@-                =@@@@%.    .%@@@@=          :%@@@@@@@@@@@:  
  @@@@@@@@@@@%.      =@@@@#     -@@@@%:    .:::-:      +@@@@#     :@@@@@:    .       +@@@@@@@@@@@#  
 +@@@@@@@@@@@@@.    *@@@@*     =@@@@#.    -@@@@@:     #@@@@+     =@@@@%.    -@#     +@@@@@@@@@@@@@- 
.@@@@@@@@@@@@@#    *@%@%=     +@@@@*     =@@@@#.    .#@@@%=     +@@@@#     =@@@%.   =@@@@@@@@@@@@@% 
+@@@@@@@@*-==-                .          .           . ..       .....      .....     .=+=+@@@@@@@@@-
%@@@@@@@+                                                                                 -@@@@@@@@#
@@@@@@@-       =#%#=     -#%%#-     -#%%*.     +%%%*.    .*%%#=     :#%%#-     =%%%*.      .#@@@@@@@
@@@@@@=.::::::*@@@@@*:::-@@@@@@-:::=@@@@@%::::*@@@@@#::::%@@@@@+:---@@@@@@=---+@@@@@%------:=@@@@@@@
=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@%%##**++===----:::::------===++***##%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@* 
  -#@@@@@@@@@@@@@@@@%#*+=-:.                                        ..::-=+*##%@@@@@@@@@@@@@@@@@#-  
    :=*%@@@@@%#*=-:                                                             .:-=+*#%%%%##+-.    
                                                                                        
                                                                                                              
                                                                                                              
                                                                                                              
         @@@;      ;@@@     @##    ##  ####$     @@@@@@@@@@     ##  o##      ;##o@@@@@@@@@@    @@@            
        @@@@@@    @@@@@@    @@@    @@  @@@@@@@o  @@@@@@@@@     %@@  @@@$     @@@ @@@@@@@@@@    @@@            
       @@@@@@@   @@@@@@@#   @@@   !@@  @@@@@@@@ *@@@@@@@@@     @@@  @@@@     @@@ @@@@@@@@@@   @@@@            
      @@@   @@@ #@@   @@@   @@    @@@  @@    @@@    @@%        @@@  @@@@    @@@@     @@       @@@@            
      @@#   @$  @@@    @@   @@    @@@  @@    @@@    @@         @@@  @@@@    @@@@    #@@      %@@@@            
     @@@        @@     @@   @@    @@@  @@    @@@    @@         @@$  @@@@   @@@@@    @@@      @@#@@            
     @@@       @@@     @@  $@@    @@% @@@    @@@   o@@         @@   @@@@   @@@@@    @@@      @@ @@            
     @@@       @@@     @@  @@@    @@  @@@   @@@    @@@         @@  @@@@@   @@@@$    @@@     @@@ @@            
     @@$       @@@    !@@  @@@    @@  @@@@@@@@@    @@@        .@@  @@#@@* @@ @@     @@o     @@! @@&           
     @@        @@@    @@@  @@@   !@@  @@@@@@@@     @@@        @@@  @@  @@ @@ @@     @@      @@  @@@           
    #@@        @@.    @@@  @@!   @@@  @@@@@@       @@#        @@@  @@  @@@@  @@     @@     @@@@@@@@           
    @@@        @@     @@@  @@    @@@  @@           @@         @@@  @@  @@@@ .@@    $@@     @@@@@@@@           
    @@@        @@     @@@  @@    @@@  @@           @@         @@#  @@  @@@  @@@    @@@    !@@@@@@@@           
    @@@        @@    $@@  .@@    @@; @@@          !@@         @@  %@@  @@@  @@@    @@@    @@@   @@@           
    @@@    @@; @@    @@@  !@@   $@@  @@@          @@@         @@  @@@  @@   @@*    @@@    @@    .@@           
     @@@  @@@  @@@  @@@;   @@@  @@@  @@@          @@@         @@  @@@       @@     @@*   @@@     @@           
     @@@@@@@o  #@@@@@@@    @@@@@@@   @@@          @@@        @@@  @@#       @@     @@    @@@     @@           
      @@@@@@    @@@@@@     #@@@@@!   @@           @@&        @@@  @@       ;@@     @@    @@      @@&          
        $         *@@        .%                                                                               
                   @@                                                                                         
                   @@@@                                                                                       
                   @@@@                                                                                       
                    @@@                                                                                                                                                                                           
*/