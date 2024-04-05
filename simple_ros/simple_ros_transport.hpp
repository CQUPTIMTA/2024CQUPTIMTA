#ifndef SIMPLE_ROS_TRANSPORT_HPP
#define SIMPLE_ROS_TRANSPORT_HPP
#include <Arduino.h>

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
        if( isFull() ){
            return;
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
        SimpleRosTransport(int r_queue_size=128,int t_queue_size=128){
            read_queue=Queue<uint8_t>(r_queue_size);
            write_queue=Queue<uint8_t>(t_queue_size);
        };
        ~SimpleRosTransport(){

        };
        virtual void send(uint8_t b){};
        virtual void send(uint8_t* b,int len){};
        virtual uint8_t read(){};
        virtual bool is_available(){};
        
    private:
        void add_write_data(uint8_t* data,int len){
            for(int i=0;i<len;i++){
                write_queue.put(data[i]);
            }
        }
        void add_write_data(uint8_t data){
            write_queue.put(data);
        }
        void read_data_to_queue(){
            read_queue.put(read());
        }
        void send_all(){
            while(!write_queue.isEmpty()){
                send(write_queue.get());
            }    
        }
        bool is_bussy=false;
        Queue<uint8_t> read_queue;
        Queue<uint8_t> write_queue;

};

class SimpleRosTransport_serial:public SimpleRosTransport{
    public: 
        SimpleRosTransport_serial(HardwareSerial* serial):SimpleRosTransport(serial){};
        ~SimpleRosTransport_serial(){};
        void send(uint8_t b){
            _serial->write(b);
        }
        void send(uint8_t* b,int len){
            _serial->write(b,len);
        }

        uint8_t read(){
            return _serial->read();
        }
        bool is_available(){
            return _serial->available();
        }

    private:
        HardwareSerial* _serial;
}
#endif