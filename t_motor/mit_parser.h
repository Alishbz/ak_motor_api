/****************************************************************************************
* ALI SAHBAZ
*
*
* AK Motors Comminication API, Tmotor / cubeMars solition
*
* Date          : 02.03.2024
* By            : Ali Sahbaz
* e-mail        : ali_sahbaz@outlook.com
*/
#pragma once

#include <cstdint>

#include "can_packet.h"

#include "queue_fifo.h" 

template <typename MOTOR_TYPE>
class mit_receive_manager
{
public:

    struct r_packet {

        int index = 0;  // software packet counter

        int receive_id = 0;
        float position = 0;
        float velocity = 0;
        float current = 0;
        float temp = 0;
        int error_code = 0;
    };
     
    void parse(can_packet<CAN_PACKET_SIZE>& msg) {
        // control packet ?
        _parse(msg);
    }

    r_packet get_last_packet(void) {
        return packets.read();
    }

    void operator()(can_packet<CAN_PACKET_SIZE>& msg) {
        parse(msg);
    } 

private:

    circular_queue<r_packet, 10> packets;

    int packet_counter = 0;

    MOTOR_TYPE params;

    int float_to_uint(float x, float x_min, float x_max, unsigned int bits) {
        float span = x_max - x_min;
        if (x < x_min) x = x_min;
        else if (x > x_max) x = x_max;
        return (int)((x - x_min) * ((float)((1 << bits) / span)));
    }
    float uint_to_float(int x_int, float x_min, float x_max, int bits) {
        float span = x_max - x_min;
        float offset = x_min;
        return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
    }

    void _parse(can_packet<CAN_PACKET_SIZE>& msg) {

        r_packet packet;

        packet.receive_id = msg.data[0]; //Driver ID

        int p_int = (msg.data[1] << 8) | msg.data[2];
        int v_int = (msg.data[3] << 4) | (msg.data[4] >> 4);
        int i_int = ((msg.data[4] & 0xF) << 8) | msg.data[5];
        int temp_int = msg.data[6];
        int error_code = msg.data[7];

        packet.position = uint_to_float(p_int, params.P_min, params.P_max, 16);
        packet.velocity = uint_to_float(v_int, params.V_min, params.V_max, 12);
        packet.current = uint_to_float(i_int, -params.Current_Factor, params.Current_Factor, 12); // todo is it true?? , Current_Factor?
        packet.temp = (float)temp_int;

        packet_counter++;

        packet.index = packet_counter;

        if (packet_counter > 10000)
            packet_counter = 0;

        packets.write(packet);  
    }
};
