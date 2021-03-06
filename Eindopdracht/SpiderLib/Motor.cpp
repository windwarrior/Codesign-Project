#include "Arduino.h"
#include "Servo.h"
#include "Motor.h"

Motor::Motor(){
}

void Motor::calibrate(int center, int range){
    _min = center - range;
    _max = center + range;
    _center = center;
}

void Motor::write(int value){    
    _position = value;
    _servo.write(value);
}

int Motor::getPosition(){
    return _position;
}

int Motor::getMin(){
    return _min;
}

int Motor::getCenter(){
    return _center;
}

int Motor::getMax(){
    return _max;
}

void Motor::attach(int pin){
    _servo.attach(pin);
}

void Motor::reset(){
    write(_center);
}
    
