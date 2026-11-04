#include <LightSensors.h>


///////////////////////////// PUBLIC /////////////////////////////

void LightSensors::init()
{
    for (uint8_t i = 0; i < 2*MUX_NUM; i++) { //Iterates through each light sensor
        pinMode(addressPins[i], OUTPUT); //Sets each pin to INPUT
    }
    calibrate();
}

void LightSensors::update()
{
    read();

    direction = -1.0f;

    for (uint8_t i = 0; i < LS_NUM; i++) {
        if (on_white[(i + LS_NUM - 1) % LS_NUM] && on_white[(i + 1) % LS_NUM]) {
            on_white[i] = true;
        }
    }

    uint8_t cluster_start[6] = {0};
    uint8_t cluster_end[6] = {0};
    uint8_t cluster_num = 0;

    bool in_cluster = false;

    for (uint8_t i = 0; i < LS_NUM; i++) {
        if (!in_cluster) {
            if (on_white[i]) {
                cluster_start[cluster_num] = i;
                in_cluster = true;
            }
        } else {
            if (!on_white[i]) {
                cluster_end[cluster_num] = i - 1;
                in_cluster = false;
                cluster_num++;
            }
        }
    }


    if (on_white[LS_NUM - 1]) {
        if (on_white[0]) {
            cluster_start[0] = cluster_start[cluster_num];
        } else {
            cluster_end[cluster_num] = LS_NUM - 1;
            cluster_num++;
        }
    }


    if (cluster_num == 1) {
        direction = mid_angle_between(cluster_start[0] * 360.0f / (float)LS_NUM, cluster_end[0] * 360.0f / (float)LS_NUM);
    } else if (cluster_num == 2) {
        float cluster1 = mid_angle_between(cluster_start[0] * 360.0f / (float)LS_NUM, cluster_end[0] * 360.0f / (float)LS_NUM);
        float cluster2 = mid_angle_between(cluster_start[1] * 360.0f / (float)LS_NUM, cluster_end[1] * 360.0f / (float)LS_NUM);
    

        float angle = angle_between(cluster1, cluster2);
        if (angle > 180.0f) {
            direction = mid_angle_between(cluster2, cluster1);
        } else {
            direction = mid_angle_between(cluster1, cluster2);
        }
    } else if (cluster_num == 3) {
        float cluster1 = mid_angle_between(cluster_start[0] * 360.0f / (float)LS_NUM, cluster_end[0] * 360.0f / (float)LS_NUM);
        float cluster2 = mid_angle_between(cluster_start[1] * 360.0f / (float)LS_NUM, cluster_end[1] * 360.0f / (float)LS_NUM);
        float cluster3 = mid_angle_between(cluster_start[2] * 360.0f / (float)LS_NUM, cluster_end[2] * 360.0f / (float)LS_NUM);

        float mid12 = mid_angle_between(cluster1, cluster2);
        if (angle_between(cluster1, cluster2) > 180.0f) {
            mid12 = mid_angle_between(cluster2, cluster1);
        }

        float final_dir = mid_angle_between(mid12, cluster3);
        if (angle_between(mid12, cluster3) > 180.0f) {
            final_dir = mid_angle_between(cluster3, mid12);
        }

        direction = final_dir;
    }
}



float LightSensors::get_direction()
{
    return direction;
}


///////////////////////////// PRIVATE /////////////////////////////

void LightSensors::read()
{
    for (uint8_t channel = 0; channel < LS_NUM/2; channel++) {
        for (uint8_t bit = 0; bit < 4; bit++) {
            digitalWrite(addressPins[bit], bitRead(channel, bit));
        }

        delayMicroseconds(2);

        value[channel] = analogRead(mux1);
        value[channel + LS_NUM/2] = analogRead(mux2);
    }

    for (uint8_t i = 0; i < LS_NUM; i++) {
        on_white[i] = value[i] > green[i];
    }
  
}

void LightSensors::calibrate()
{
    for (uint8_t i = 0; i < 10; i++) {
        read();
        for (uint8_t j = 0; j < LS_NUM; j++) {
            green[j] += value[j];
        }
    }

    for (uint8_t i = 0; i < LS_NUM; i++) {
        green[i] /= 10;
        green[i] += LS_BUFFER;
    }
}



