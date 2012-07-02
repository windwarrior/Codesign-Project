#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "Arduino.h"
#include "Servo.h"

class Motor{
    public:
        Motor();
        void calibrate(int center, int range);
        void write(int value);
        int getPosition();
        int getMin();
        int getCenter();
        int getMax();
        void attach(int pin);
        void reset();
    private:
        Servo _servo;
        int _min;
        int _center;
        int _max;        
        int _position;
};

#endif
