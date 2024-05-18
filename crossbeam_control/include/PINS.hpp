/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-05-17 14:27:12
 */
#ifndef PINS_HPP
#define PINS_HPP
#include <Arduino.h>
#define LEFT_SW_PIN 15
#define RIGHT_SW_PIN 16
#define LEFT_SW_LED 39
#define RIGHT_SW_LED 38
#define MOTOR_LED_PIN 11
#define buzz_pin 35
void left_sw_interrupt() {
    digitalWrite(LEFT_SW_LED, !digitalRead(LEFT_SW_PIN));
}
void right_sw_interrupt() {
    digitalWrite(RIGHT_SW_LED, !digitalRead(RIGHT_SW_PIN));
}
void setup_pins() {

    pinMode(LEFT_SW_LED, OUTPUT);
    pinMode(RIGHT_SW_LED, OUTPUT);
    pinMode(buzz_pin, OUTPUT);
    pinMode(MOTOR_LED_PIN,OUTPUT);
    pinMode(LEFT_SW_PIN, INPUT_PULLUP);
    pinMode(RIGHT_SW_PIN, INPUT_PULLUP);

    digitalWrite(MOTOR_LED_PIN,LOW);
    digitalWrite(LEFT_SW_LED, LOW);
    digitalWrite(RIGHT_SW_LED, LOW);
    digitalWrite(buzz_pin, LOW);
}

#endif