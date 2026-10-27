#include "Motor.h"


void Motor::init(uint8_t a, uint8_t b , uint8_t e)
{
    ina = a;
    inb = b;
    en = e;
    pinMode(ina, OUTPUT);
    pinMode(inb, OUTPUT);
    pinMode(en, OUTPUT);
}


void Motor::movement(int16_t speed)
{
    if (speed > 0) {
        digitalWrite(ina, HIGH);
        digitalWrite(inb, LOW);
        analogWrite(en, speed);
    }
    if (speed < 0) {
        digitalWrite(ina, LOW);
        digitalWrite(inb, HIGH);
        analogWrite(en, abs(speed));
    }
    if (speed == 0) {
        digitalWrite(ina, HIGH);
        digitalWrite(inb, HIGH);
        analogWrite(en, 0);
    }
}