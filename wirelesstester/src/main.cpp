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

  shell.addCommand(F("laser"), laser);
  esp_now_setup();
}

void loop() {
    shell.executeIfInput();
    delay(10);

}

