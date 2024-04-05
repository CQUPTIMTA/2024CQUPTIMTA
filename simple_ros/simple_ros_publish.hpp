#ifndef SIMPLE_ROS_PUBLISH_HPP
#define SIMPLE_ROS_PUBLISH_HPP
#include <Arduino.h>
#include <simple_ros_node.hpp>
class SimpleRosPublish{
    public:
        template<typename T>
        SimpleRosPublish(SimpleRosNode* node,String name,T m){
            _node=node;
            _name=name;
            _msg_len=sizeof(m);

        };
        template<typename T>
        ~SimpleRosPublish(T m){
            uint8_t msg[_msg_len];
            memcpy(msg,&m,_msg_len);
            
        };
        void init(void(*callback)(void* p)){
            xTaskCreatePinnedToCore(callback,"message_loop",4096,NULL,1,NULL,1);
        }
    private:
        SimpleRosNode *_node;
        String _name;
        uint8_t _msg_len;

}
#endif