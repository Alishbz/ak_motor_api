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

#include "ak_params.h"  

const unsigned char start_motor_data[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0XFC };
const unsigned char exit_motor_data[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0XFD };
const unsigned char set_origin_data[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0XFE };
 




template <typename MOTOR_TYPE>
struct control_params
{
	float position = 0;
	float velocity = 0;
	float torque = 0;
	float kp = 0;
	float kd = 0;
     
    int32_t get_position_int() {
        return convert_float_to_int(position , params.P_min , params.P_max , 16);
    }

    int32_t get_velocity_int() {
        return convert_float_to_int(velocity, params.V_min, params.V_max, 12);
    }

    int32_t get_torque_int() {
        return convert_float_to_int(torque, params.T_min, params.T_max, 12);
    }

    int32_t get_kp_int() {
        return convert_float_to_int(kp, params.Kp_min, params.Kp_max, 12);
    }

    int32_t get_kd_int() {
        return convert_float_to_int(kd, params.Kd_min, params.Kd_max, 12);
    }

    MOTOR_TYPE& get_params() const { return params; };
     
private:

    MOTOR_TYPE params;

    int32_t  convert_float_to_int(float val, float min, float max, uint8_t bits)
    { 
        if (val > max)
        {
            val = max;
        }
        else if (val < min)
        {
            val = min;
        }

        float span = max - min;

        return (int32_t)((val - min) * ((float)((1 << bits) - 1)) / span);
    }
}; 




 