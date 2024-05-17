/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-17 10:13:33
 */
#include <Arduino.h>
#include "SimpleSerialShell.h"

int showID(int /*argc*/ = 0, char** /*argv*/ = NULL)
{
    shell.println(F( "Running " __FILE__ ",\nBuilt " __DATE__));
    return 0;
};
int get_system_time(int /*argc*/ = 0, char** /*argv*/ = NULL){
  shell.print("time: ");
  shell.print(millis());
  shell.println("ms");
  return 0;
}
void setup() {
  Serial.begin(115200);

  shell.attach(Serial);
  shell.addCommand(F("id?"), showID);
  shell.addCommand(F("time"), get_system_time);
  showID();
  Serial.println(F("Ready."));
}

void loop() {
    shell.executeIfInput();
    delay(10);
    
}

