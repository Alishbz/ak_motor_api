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

#include <cstring> 

#define CAN_PACKET_SIZE  8

/*** For canFD you must do -> PACKET_SIZE = 64  ***/

template <size_t PACKET_SIZE = 8> 
class can_packet
{ 
public:
    enum class id_type 
    {
        STANDARD, EXTENDED
    };

    can_packet(id_type _type) : type(_type), id(0) {
        std::memset(data, 0, sizeof(data));
    };

    can_packet() : type(id_type::EXTENDED), id(0) {
        std::memset(data, 0, sizeof(data));
    };

    unsigned int  id;

    unsigned char data[PACKET_SIZE];

    id_type get_id_type(){ return type; }

private:
    id_type  type; 
};