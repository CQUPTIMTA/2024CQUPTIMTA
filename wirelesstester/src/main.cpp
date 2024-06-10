/*
 * @Description:
 * @Author: qingmeijiupiao
 * @Date: 2024-05-17 10:13:33
 */


#include "commands.hpp"



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
int test_func(int argc = 0, char** argv = NULL) {
  commands::move_to_z(1,120,250,230);
  commands::move_to_z(2,120,250,230);
  commands::move_to_x(1,1000,800,230);
  commands::move_to_x(2,500,800,230);
  commands::move_to_y(6,2020,1000,220);
  commands::wait(6,'Y');
  delay(300);
  commands::move_to_z(1,0,400,0);
  commands::wait(1,'Z');
  delay(500);
  commands::grap(1,0);
  delay(600);
  commands::move_to_z(1,230,250,235);
  delay(300);
  commands::move_to_y(6,2375,1000,220);
  commands::move_to_x(1,1755,800,230);
  commands::move_to_x(2,1000,800,230);
  commands::wait(6,'Y');
  delay(300);
  commands::wait(2,'X');
  commands::move_to_z(2,0,400,0);
  commands::wait(2,'Z');
  delay(1000);
  commands::move_to_z(2,230,250,235);
  delay(300);
  commands::move_to_x(2,245,800,230);
  commands::move_to_y(6,245,1000,220);
  commands::wait(6,'Y');
  commands::grap(1,1);
  commands::buzz(6,1);
  delay(3000);
  commands::buzz(6,0);
  return 0;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  add_help();

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
  shell.addCommand(F("test_func"),test_func);

  shell.addCommand(F("buzz"),buzz);
  shell.addCommand(F("setupZ"),setupZ);
  esp_now_setup();
}

void loop() {
    shell.executeIfInput();
    delay(10);

}

