#ifndef LIGHTSENSORS_H
#define LIGHTSENSORS_H


#include <Arduino.h>
#include "Common.h"
#include <Pins.h>


#define MUX_NUM 4
#define LS_NUM 32
#define LS_BUFFER 200

class LightSensors {
public:
    LightSensors() {}
    void init();
    void update();
    float get_direction();

private:
    void read();
    void calibrate();

    float direction = -1.0f;

    const uint8_t addressPins[MUX_NUM] = {17, 16, 15, 14};

    const uint8_t mux1 = 23;
    const uint8_t mux2 = 22;


    uint16_t value[LS_NUM] = {0};
    bool on_white[LS_NUM] = {false};
    uint16_t green[LS_NUM] = {0};
};

#endif