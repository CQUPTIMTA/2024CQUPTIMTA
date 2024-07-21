/*
 * @Description:
 * @Author: qingmeijiupiao
 * @Date: 2024-05-17 10:13:33
 */


#include "commands.hpp"
#include "web.hpp"


void add_help(){
  help_map["clear"]=F("clear");
  help_map["info"]=F("info");
  help_map["test"]=F("test <id> or test <start_id> <end_id>");
  help_map["getY"]=F("getY <id>");
  help_map["getZ"]=F("getZ <id>");
  help_map["getX"]=F("getX <id>");
  help_map["getVoltage"]=F("getVoltage <id>");
  help_map["getSensor"]=F("getSensor <id>");
  help_map["move_to_x"]=F("move_to_x <id> <point> <speed> <acceleration>");
  help_map["move_to_y"]=F("move_to_y <id> <point> <speed> <acceleration>");
  help_map["move_to_z"]=F("move_to_z <id> <point> <speed> <acceleration>");
  help_map["move_x"]=F("move_x <id> <point> <speed> <acceleration>");
  help_map["move_y"]=F("move_y <id> <point> <speed> <acceleration>");
  help_map["move_z"]=F("move_z <id> <point> <speed> <acceleration>");
  help_map["rezero"]=F("rezero <id>");
  help_map["set_zero_point"]=F("set_zero_point <id> <point>");
  help_map["grap"]=F("grap <id> <state>");
  help_map["enable"]=F("enable <id> <name>='X'||'Y'||'Z'  <state>");
  help_map["laser"]=F("laser <id> <state>");
  help_map["is_moveing"]=F("is_moveing <id> <name>='X'||'Y'||'Z'");
  help_map["set_now"]=F("set_now <id> <point>");
  help_map["read_servo_angle"]=F("read_servo_angle <id> <name>='X'||'Y'||'Z'");
  help_map["set_servo_angle"]=F("set_servo_angle <id> <state> <name>='X'||'Y'||'Z' <angle>");
  help_map["buzz"]=F("buzz <id> <state>");
  help_map["setupZ"]=F("setupZ <high>");
  help_map["gw"]=F("gw : using recongnize unit get weight point");
}

int help(int argc = 0, char** argv = NULL) {
  if(argc!=2){
    for(auto i=help_map.begin();i!=help_map.end();i++){
      shell.println(i->second);
    }
  }
  else{
    shell.println(help_map[String(argv[1])]);
  }
  return 0;

}

TaskHandle_t main_func_handler=nullptr;
void main_func(void * pvParameters) {
  commands::grap(1,1);
  commands::grap(2,1);
  commands::move_to_z(1,120);
  commands::move_to_z(2,120);
  commands::move_to_x(1,1000);
  commands::move_to_x(2,500);
  commands::move_to_y(6,2040);


  commands::wait(6,'Y',1000,5);

  commands::move_to_z(1,5,250,0);
  delay(1500);
  commands::move_y(6,-40);
  delay(300);
  commands::grap(1,0);
  commands::move_to_z(1,230);
  delay(300);
  commands::move_to_x(1,1755);
  commands::move_to_x(2,1000);
  delay(500);
  commands::move_to_y(6,2375+40);

  commands::wait(6,'Y',1000,5);
  commands::move_to_z(2,5,250,0);
  delay(1000);
  commands::move_y(6,-40);
  delay(500);
  commands::grap(2,0);
  commands::move_to_z(2,230);
  delay(500);
  commands::move_to_x(2,245);
  delay(500);
  commands::move_to_y(6,245);
  commands::wait(6,'Y',1000,5);
  commands::grap(1,1);
  commands::grap(2,1);
  commands::buzz(6,1);
  delay(3000);
  commands::buzz(6,0);
  delay(500);
  main_func_handler=nullptr;
  vTaskDelete(NULL);
}


int main_func_task(int argc = 0, char** argv = NULL) {
  if(main_func_handler!=nullptr){
    xTaskCreatePinnedToCore(main_func, "main_task", 16384, NULL, 5, &main_func_handler,1);
  }
  return 0;
}
void cmd_task(void * pvParameters) {
  while (1) {
    shell.executeIfInput();
    delay(10);
  }
}
void web_task(void * pvParameters) {
  web_setup();
  while (1) {
    server.handleClient();
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(10,OUTPUT);
  digitalWrite(10,1);
  delay(1000);
  add_help();
  //pinMode(10,OUTPUT);
  //digitalWrite(10,1);
  shell.attach(Serial);
  shell.addCommand(F("help"),help);
  //_id
  shell.addCommand(F("clear"),clear);
  shell.addCommand(F("info"), showinfo);
  shell.addCommand(F("test"), online_test);
  shell.addCommand(F("getY"),get_y);
  shell.addCommand(F("getZ"),get_z);
  shell.addCommand(F("getX"),get_x);
  shell.addCommand(F("getVoltage"),get_voltage);
  shell.addCommand(F("getSensor"),get_sensor);

  //_Id point speed acceleration
  shell.addCommand(F("move_to_x"),move_to_x);
  shell.addCommand(F("move_to_y"),move_to_y);
  shell.addCommand(F("move_to_z"),move_to_z);
  shell.addCommand(F("move_x"),move_x);
  shell.addCommand(F("move_y"),move_y);
  shell.addCommand(F("move_z"),move_z);

  //_Id
  shell.addCommand(F("rezero"),rezero);
  //_Id point
  shell.addCommand(F("set_zero_point"), set_zero_point);
  //_Id state
  shell.addCommand(F("grap"), grap);
  //_Id name state
  shell.addCommand(F("enable"), enable);
  //_Id state
  shell.addCommand(F("laser"), laser);
  
  shell.addCommand(F("is_moveing"), is_moveing);
  shell.addCommand(F("set_now"), set_now);
  shell.addCommand(F("read_servo_angle"), read_servo_angle);
  shell.addCommand(F("set_servo_angle"), set_servo_angle);
  shell.addCommand(F("main"),main_func_task);

  shell.addCommand(F("buzz"),buzz);
  shell.addCommand(F("setupZ"),setupZ);

  shell.addCommand(F("gw"),get_weight);
  
  esp_now_setup();
  digitalWrite(10,0);
  xTaskCreatePinnedToCore(cmd_task, "cmd_task", 2048, NULL, 5, NULL,0);
  xTaskCreatePinnedToCore(web_task, "web_task", 4096, NULL, 4, NULL,1);
}

void loop() {
  delay(3000);
}

