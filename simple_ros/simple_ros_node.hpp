#ifndef SIMPLE_ROS_NODE_HPP
#define SIMPLE_ROS_NODE_HPP
#include <Arduino.h>
#include "simple_ros_transport.hpp"
#include "simple_ros_Parameter.hpp"
#include "simple_ros_publish.hpp"
#include "simple_ros_service.hpp"
#include "simple_ros_client.hpp"
#include "simple_ros_Subscribe.hpp"
#include <map>
class SimpleRosNode{
    public:
        SimpleRosNode(SimpleRosTransport* transport,String name){
            _transport=transport;
            _name=name;
        };
        ~SimpleRosNode(){
            
        };
        static const void message_loop(void* pvParameters){
            while(1){
                _transport->read_data_to_queue();
                _transport->send_all();
                delay(1);
            }
        }
        void init(){
            xTaskCreatePinnedToCore(message_loop,"message_loop",4096,NULL,1,NULL,1);
        };
    SimpleRosTransport* _transport;
    private:
        String _name;
}

#endif