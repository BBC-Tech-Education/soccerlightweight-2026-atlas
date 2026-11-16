#include <GoalTracking.h>


///////////////////////////// PUBLIC /////////////////////////////


void GoalTracking::init()
{
    pinMode(RX5, INPUT);
    pinMode(TX5, OUTPUT);
    Serial1.begin(115200);
}

void GoalTracking::update()
{
    read();
}

void GoalTracking::set_target_colour(char colour)
{
    Serial1.write(colour);
}

bool GoalTracking::has_target()
{
    return target_visible;
}

int GoalTracking::get_angle()
{
    return goal_angle;
}

int GoalTracking::get_area()
{
    return goal_area;
}

float GoalTracking::get_turn_correction(float kP)
{
    if (!target_visible) {
        return 0.0f;
    }
    return (float)goal_angle * kP;
}


///////////////////////////// PRIVATE /////////////////////////////

void GoalTracking::read()
{
    while (Serial1.available() > 0) {
        char c = Serial1.read();

        if (c == '\n') {
            buffer[buf_index] = '\0';

            char* comma = strchr(buffer, ',');
            if (comma != NULL) {
                *comma = '\0';
                goal_angle = atoi(buffer);
                goal_area = atoi(comma + 1);
                target_visible = true;
                last_packet_time = millis();
            }
            buf_index = 0;
        } else if (buf_index < sizeof(buffer) - 1) {
            buffer[buf_index++] = c;
        }
    }
}