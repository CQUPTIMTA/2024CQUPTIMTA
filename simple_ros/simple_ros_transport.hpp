#ifndef SIMPLE_ROS_TRANSPORT_HPP
#define SIMPLE_ROS_TRANSPORT_HPP
#include <Arduino.h>
#include <cstring>

enum package_type {
    package_type_OK = 0,
    package_type_error = 1,
    package_type_request = 2,
    package_type_response = 3
}

struct data_package {
    uint16_t header=0xFEFE;
    uint8_t packge_type;
    uint8_t name_len;
    uint8_t data_len;
    char* name;
    uint8_t* data;
    uint8_t checksum=0;
    void add_checksum(){
        checksum+=header+packge_type+name_len+data_len;
        for(int i=0;i<name_len;i++){
            checksum+=name[i];
        }
        for(int i=0;i<data_len;i++){
            checksum+=data[i];
        }
    }
}


static void Processing_data(void* p);
class SimpleRosTransport{
    friend void Processing_data(void* p);
    public:
        SimpleRosTransport(){

        };
        void setup(){
            xtask_create(&Processing_data,"Processing_data",4096,NULL,1,NULL,1);
        };
        ~SimpleRosTransport(){

        };
        virtual void send(data_package data){};
        virtual uint8_t read(){};
        virtual bool is_available(){};
    protected:
        std::vector<uint8_t> r_buffer;


};
static void Processing_data(void* p){
    SimpleRosTransport* port = (SimpleRosTransport*)p;
    while(1){ 
        if(port->r_buffer.size()>2){
            while(!(port->r_buffer[0]==0xFE&&port->r_buffer[1]==0xFE)){
                port->r_buffer.erase(port->r_buffer.begin());
                if(port->r_buffer.size()==0){
                    break;
                }
            }
            if(port->r_buffer.size()>4){
                data_package data;
                data.header = port->r_buffer[0]<<8|port->r_buffer[1];
                data.packge_type = port->r_buffer[2];
                data.name_len = port->r_buffer[3];
                data.data_len = port->r_buffer[4];
                data.name = port->r_buffer.data()+5;
                data.data = port->r_buffer.data()+5+data.name_len;
                
                data.checksum = port->r_buffer[5+data.name_len+data.data_len];
            }
        }
    }
}

class SimpleRosTransport_serial:public SimpleRosTransport{
    public: 
        SimpleRosTransport_serial(HardwareSerial* serial):SimpleRosTransport(serial){};
        ~SimpleRosTransport_serial(){};
        void send(data_package data){
            _serial->write(data.header>>8);
            _serial->write(data.header);
            _serial->write(data.packge_type);
            _serial->write(data.name_len);
            _serial->write(data.data_len);
            for(int i=0;i<data.name_len;i++){
                _serial->write(data.name[i]);
            }
            for(int i=0;i<data.data_len;i++){
                _serial->write(data.data[i]);
            }
            _serial->write(data.checksum);
        }

    private:
        HardwareSerial* _serial;
}
#endif