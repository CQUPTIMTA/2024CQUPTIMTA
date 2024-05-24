/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-17 10:13:33
 */


#include "commands.hpp"


void setup() {
  Serial.begin(115200);
  delay(1000);
  shell.attach(Serial);
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

  
  esp_now_setup();
}

void loop() {
    shell.executeIfInput();
    delay(10);

}

