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
  shell.addCommand(F("info"), showinfo);
  shell.addCommand(F("test"), test_esp_now);
  esp_now_setup();
}

void loop() {
    shell.executeIfInput();
    delay(10);

}

