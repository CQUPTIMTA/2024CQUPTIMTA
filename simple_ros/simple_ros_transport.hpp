#ifndef SIMPLE_ROS_TRANSPORT_HPP
#define SIMPLE_ROS_TRANSPORT_HPP
#include <Arduino.h>


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

template <typename T>
class Queue {
public:
    Queue(int size) {
        _size = size;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~Queue() {
        delete[] data;
    }

    // 从队列中获取数据，并且退队
    T get() {
        T t = data[front];
        front = (front + 1) % _size;
        return t;
    }

    // 从队列中获取数据，但不退队
    T peek() { 
        if(isEmpty()){
            return 0;
        }
        return data[front];
    }

    // 向队列中放入数据
    void put(T t) {
        if (isFull()) {
            front--;
            if (front < 0) {
                front = _size - 1;
            }
            tail++;
            if (tail >= _size) {
                tail = 0;
            }
            data[tail] = t;
        }
        data[tail] = t;
        tail = (tail + 1) % _size;
    }
    int get_len(){  
        return (tail-front+_size)%_size;
    }

private:
    T* data = nullptr;
    int front = 0;
    int tail = 0;
    int _size = 0;

    bool isEmpty(){
        return (tail == front) && (get_len() == 0);
    }

    bool isFull(){
        return (tail + 1) % _size == front;

    }
};


class SimpleRosTransport{
    friend class SimpleRosNode;
    public:
        SimpleRosTransport(int r_queue_size=128){
            read_queue=Queue<uint8_t>(r_queue_size);
        };
        ~SimpleRosTransport(){

        };
        virtual void send(data_package data){};
        virtual uint8_t read(){};
        virtual bool is_available(){};
        
    protected:
        Queue<uint8_t> read_queue;

};

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