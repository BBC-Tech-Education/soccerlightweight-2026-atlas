#ifndef GOALTRACKING_H
#define GOALTRACKING_H
#include <Arduino.h>
#include <Pins.h>

class GoalTracking {
public:
    GoalTracking() {}
    void init();
    void update();
    void set_target_color(char color);

    int get_angle();
    int get_area();
    bool has_target();

private:
    int goal_angle = 0;
    int goal_area = 0;
    bool target_visible = false;
    unsigned long last_packet_time = 0;

    char buffer[20];
    uint8_t buf_index = 0;
};

#endif