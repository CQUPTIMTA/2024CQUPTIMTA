#ifndef PID_HPP
#define PID_HPP
#include <Arduino.h>
class PID{
    public:
        PID(float Kp, float Ki, float Kd):KP(Kp), KI(Ki), KD(Kd){};
        ~PID(){};
        double control(double target, double error){
            double output = error;
            if(last_time != 0){
                double delta_time = millis() - last_time;
                double delta_error = error - last_error;
                last_integral += error * delta_time;
                double derivative = delta_error / delta_time;
                output = KP * error + KI * last_integral + KD * derivative;
            }else{
                output = KP * error + KI * last_integral + KD * 0;
            }
            last_error = error;
            last_time = millis();
            return output;
        };
        void reset(){
            last_error = 0;
            last_integral = 0;
            last_time = 0;
        };
    private:
        double KP;
        double KI;
        double KD;
        double last_error=0;
        double last_integral=0;
        int last_time=0;
};
#endif