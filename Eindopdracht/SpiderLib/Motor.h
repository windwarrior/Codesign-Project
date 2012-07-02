#ifndef Motor
#define Motor
#include "Arduino.h"
#include "Servo.h"

class Motor{
    public:
        Motor(Motor*);
        void calibrate(int center, int range);
        void write(int value);
        int getPosition();
        int getMin();
        int getCenter();
        int getMax();
        void attach(int pin);
    private:
        Servo _serv;
        int _min;
        int _center;
        int _max;        
        int _postion;
};

#endif
