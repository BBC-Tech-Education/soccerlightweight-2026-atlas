#ifndef GOALTRACKING_H
#define GOALTRACKING_H
#include <Arduino.h>
#include <Pins.h>

class GoalTracking {
public:
    GoalTracking() {}
    void init();
    void update();
    void set_target_colour(char colour);

    bool has_target();
    int get_angle();
    int get_area();
    float get_turn_correction(float kP);


private:
    void read();

    int goal_angle = 0;
    int goal_area = 0;
    bool target_visible = false;
    unsigned long last_packet_time = 0;

    char buffer[24];
    uint8_t buf_index = 0;
};

#endif