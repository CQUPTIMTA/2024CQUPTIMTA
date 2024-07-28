/*
 * @Description:
 * @Author: qingmeijiupiao
 * @Date: 2024-05-17 10:13:3
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




float ground_hight=4;//抓取高度
float release_hight=220;//放下高度
float safe_distance=40;//安全距离
TaskHandle_t ID6task_handler=nullptr;
bool ID6complite=false; //6号横梁抓完标志
bool ID8complite=false; //8号横梁抓完标志
void ID6task(void * pvParameters) {
  commands::wait(6,'Y');
  if(commands::ID6Crossbeam_weight[0].y==commands::ID6Crossbeam_weight[1].y){
    
    commands::move_to_z(1,ground_hight);
    commands::move_to_z(2,ground_hight);
    commands::wait(1,'Z');
    commands::move_y(6,-1*safe_distance);
    commands::wait(6,'Y');
    commands::grap(1,0,0);
    commands::grap(2,0,700);
    commands::move_to_z(1,release_hight);
    commands::move_to_z(2,release_hight);
  }else{
    int first_id=commands::ID6Crossbeam_weight[0].x>commands::ID6Crossbeam_weight[1].x?1:2;
    int second_id=first_id==1?2:1;
    //先抓第一个
    commands::move_to_z(first_id,ground_hight);
    commands::wait(first_id,'Z');
    commands::move_y(6,-1*safe_distance);
    commands::wait(6,'Y');
    commands::grap(first_id,0,700);
    commands::move_to_z(first_id,release_hight);
    delay(500);
    //再抓第二个
    commands::move_to_y(6,commands::ID6Crossbeam_weight[1].y+safe_distance);
    commands::wait(6,'Y');
    commands::move_to_z(second_id,ground_hight);
    commands::wait(second_id,'Z');
    commands::move_y(6,-1*safe_distance);
    commands::wait(6,'Y');
    commands::grap(second_id,0,700);
    commands::move_to_z(second_id,release_hight);
  }
  delay(500);
  commands::move_to_y(6,245);
  ID6task_handler=nullptr;
  commands::move_to_x(1,1755);
  commands::move_to_x(2,245);
  
  commands::wait(6,'Y');
  delay(2000);//等砝码稳定
  commands::grap(1,1,0);
  commands::grap(2,1,0);
  ID6complite=true;
  vTaskDelete( NULL );
}
TaskHandle_t ID8task_handler=nullptr;
void ID8task(void * pvParameters) {

  
  commands::wait(8,'Y');

  if(commands::ID8Crossbeam_weight[0].y==commands::ID8Crossbeam_weight[1].y){
    commands::move_to_z(4,ground_hight);
    commands::move_to_z(5,ground_hight);
    commands::wait(4,'Z');
    commands::move_y(8,-1*safe_distance);
    commands::wait(8,'Y');
    commands::grap(4,0,0);
    commands::grap(5,0,700);
    commands::move_to_z(4,release_hight);
    commands::move_to_z(5,release_hight);
  }else{
    int first_id=commands::ID8Crossbeam_weight[0].x>commands::ID8Crossbeam_weight[1].x?4:5;
    int second_id=first_id==4?5:4;
    //抓第一个
    commands::move_to_z(first_id,ground_hight);
    commands::wait(first_id,'Z');
    commands::move_y(8,-1*safe_distance);
    commands::wait(8,'Y');
    commands::grap(first_id,0,700);
    commands::move_to_z(first_id,release_hight);
    
    //抓第二个
    commands::move_to_y(8,commands::ID8Crossbeam_weight[1].y+safe_distance);
    commands::wait(8,'Y');
    commands::move_to_z(second_id,ground_hight);
    commands::wait(second_id,'Z');
    commands::move_y(8,-1*safe_distance);
    commands::wait(8,'Y');
    commands::grap(second_id,0,700);

    commands::move_to_z(second_id,release_hight);
  }
  delay(500);
  commands::move_to_x(4,1755);
  commands::move_to_x(5,245);
  commands::move_to_y(8,3755);
  ID8task_handler=nullptr;
  commands::wait(8,'Y');

  delay(2000);//等砝码稳定
  commands::grap(4,1,0);
  commands::grap(5,1,0);
  ID8complite=true;
  vTaskDelete( NULL );
}

TaskHandle_t main_func_handler=nullptr;
void main_func(void * pvParameters) {
  ID6complite=false; //6号横梁抓完标志
  ID8complite=false; //8号横梁抓完标志
  commands::all_z_to_height(120);
  //先启动Y
  commands::move_to_y(8,commands::ID8Crossbeam_weight[0].y+safe_distance);
  commands::move_to_y(7,2750);
  commands::move_to_y(6,commands::ID6Crossbeam_weight[0].y+safe_distance);

  //获取最小和最大的X坐标
  auto get_mini_x=[](std::vector<commands::point> point_list)->float{
    return point_list[0].x<point_list[1].x?point_list[0].x:point_list[1].x;
  };
  auto get_max_x=[](std::vector<commands::point> point_list)->float{
    return point_list[0].x>point_list[1].x?point_list[0].x:point_list[1].x;
  };
  //再启动X
  commands::move_to_x(1,get_max_x(commands::ID6Crossbeam_weight));//X小的分配给1号
  commands::move_to_x(2,get_mini_x(commands::ID6Crossbeam_weight));//X大的分配给2号
  //commands::move_to_x(3,commands::ID7Crossbeam_weight.x);
  commands::move_to_x(4,get_max_x(commands::ID8Crossbeam_weight));//X小的分配给4号
  commands::move_to_x(5,get_mini_x(commands::ID8Crossbeam_weight));//X大的分配给5号
  Serial.println("start");
  xTaskCreate(ID6task, "ID6task", 4096, NULL, 5, &ID6task_handler);//创建6号横梁的任务
  xTaskCreate(ID8task, "ID8task", 4096, NULL, 5, &ID8task_handler);//创建8号横梁的任务

  if(commands::ID7Crossbeam_weight.y<2750){//判断是先抓远端还是先抓近端砝码
    while(ID6task_handler!=nullptr){
      delay(10);
    }
  }else{
    while(ID8task_handler!=nullptr){
      delay(10);
    }
  }
  Serial.println("7 move");
  //移动到砝码安全距离
  commands::move_to_y(7,commands::ID7Crossbeam_weight.y+safe_distance);
  commands::wait(7,'Y');
  commands::move_to_z(3,ground_hight);
  commands::wait(3,'Z');
  commands::move_y(7,-1*safe_distance);
  commands::wait(7,'Y');

  commands::grap(3,0,700);//抓取
  commands::move_to_z(3,120);
  delay(500);
  commands::move_to_y(7,2750);
  commands::wait(7,'Y');
  delay(1000);//等砝码稳定
  commands::grap(3,1,0);
  
  //等待执行完毕
  while(!ID8complite||!ID6complite){
    delay(10);
  }
  //执行完毕,蜂鸣器响
  commands::buzz(6,1);
  commands::buzz(7,1);
  commands::buzz(8,1);
  delay(4000);
  commands::buzz(6,0);
  commands::buzz(7,0);
  commands::buzz(8,0);
  main_func_handler=nullptr;
  vTaskDelete(NULL);
}


int main_func_task(int argc = 0, char** argv = NULL) {
  if(main_func_handler!=nullptr){
    xTaskCreatePinnedToCore(main_func, "main_task", 8192, NULL, 8, &main_func_handler,1);
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

void add_shell_commands() {
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
}

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,1);
  delay(1000);

  add_help();
  add_shell_commands();

  esp_now_setup();
  digitalWrite(LED,0);
  xTaskCreatePinnedToCore(cmd_task, "cmd_task", 4096, NULL, 5, NULL,0);
  xTaskCreatePinnedToCore(web_task, "web_task", 8182, NULL, 4, NULL,1);
  
  commands::ID6Crossbeam_weight.push_back(commands::weight_points[1]);
  commands::ID6Crossbeam_weight.push_back(commands::weight_points[9]);
  commands::ID7Crossbeam_weight=commands::weight_points[5];
  commands::ID8Crossbeam_weight.push_back(commands::weight_points[4]);
  commands::ID8Crossbeam_weight.push_back(commands::weight_points[12])
}

void loop() {
  delay(3000);
}

