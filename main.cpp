/****************************************************************************************
* ALI SAHBAZ
*
*
* AK Motors Comminication APP
*
* Date          : 02.03.2024
* By            : Ali Sahbaz
* e-mail        : ali_sahbaz@outlook.com
*/ 
#include <iostream>
#include <memory>
#include <vector>
#include <windows.h>  

#include "ak_motor.h" 
#include "mini_tdd.h" 
#include "serial_port_api.h"  

#include "ak_motor_container.h" 

#define  VIRTUAL_TEST_COM         "COM1"
#define  CAN_TO_SERIAL_ACTIVE     1
 
void _app_run();
  
serial_port_api serial_port;

ak_motor_container motors;


float motor_1_pos = 0;
float motor_2_pos = 0;
float motor_3_pos = 0;


void _can_transmit(can_packet<CAN_PACKET_SIZE> &pck)
{ 
#ifdef CAN_TO_SERIAL_ACTIVE

    char s_packet[50] = { 0 };
    int index = 0;
      
    s_packet[index++] = 0xAA;
    s_packet[index++] = 0xAB;
    s_packet[index++] = 0x00;
    s_packet[index++] = 0x01; // converter's can desc index
    s_packet[index++] = 0x00;
    s_packet[index++] = 0x0C; // packet size

    s_packet[index++] = (uint8_t)(pck.id << 24);
    s_packet[index++] = (uint8_t)(pck.id << 16);
    s_packet[index++] = (uint8_t)(pck.id << 8);
    s_packet[index++] = (uint8_t)(pck.id << 0);

    for (int i = 0; i < 8; i++) {
        s_packet[index++] = pck.data[i]; 
    }
    
    s_packet[index++] = 0;
    s_packet[index++] = 0;
     
    serial_port.transmit(s_packet , index); 
     
#else
    std::cout << "\nCAN Transmit ,  id: \n" << pck.id <<
        " datas: " << std::hex << (int)pck.data[0] << " "
        << (int)pck.data[1] << " " << (int)pck.data[2] << " "
        << (int)pck.data[3] << " " << (int)pck.data[4] << " "
        << (int)pck.data[5] << " " << (int)pck.data[6] << " "
        << (int)pck.data[7] << "\n\n";
#endif
     
}
 
void _serial_receive(const char* packet, int length)
{
    std::cout << "\nSERIAL Receive: ";
    for (int i = 0; i < length; ++i) {
        std::cout << packet[i];
    }
    std::cout << "\n";  

    /*********/

    if (length > 17) {
        if (packet[0] == 0xAA && packet[1] == 0xAB &&
            packet[2] == 0x00 && packet[3] == 0x01 && 
            packet[4] == 0x00 && packet[5] == 0x0C) {
            can_packet<8> rcv_packet;

            rcv_packet.id = (packet[6] << 24) | (packet[7] << 16) | (packet[8] << 8) | (packet[9] << 0);
            for (int i = 0; i < 8; i++) {
                rcv_packet.data[i] = packet[10+i];
            }

            motors.get(0)->receive_packet(rcv_packet);
            motors.get(1)->receive_packet(rcv_packet);
            motors.get(2)->receive_packet(rcv_packet);
        }
    }
}




/** TESTS **************************************/

TEST_CASE(init_test)
{ 
    ASSERT(motors.get(0)->get_id() == 1);
    ASSERT(motors.get(1)->get_id() == 2);
    ASSERT(motors.get(2)->get_id() == 3);
}

/****************************************/

int main(void)
{   
    std::cout << "ASOS AT motor driver test 88   \n" << std::endl;

    if (!serial_port.open(VIRTUAL_TEST_COM)) {
        std::cerr << "Failed to open the serial port!\n";
        return 1; 
    }

    serial_port.bind(_serial_receive);
    serial_port.transmit("ASOS AT motor driver serial port open  \n");
      
    ak_motor_mit_mode<AK60_6> motor_1;     // default id = 1
    ak_motor_mit_mode<AK60_6> motor_2(2);
    ak_motor_mit_mode<AK60_6> motor_3(3);

    motor_1.bind_can_transmit(_can_transmit);
    motor_2.bind_can_transmit(_can_transmit);
    motor_3.bind_can_transmit(_can_transmit);

    motor_1.set_kp(2.0f);
    motor_2.set_kp(2.0f);
    motor_3.set_kp(2.0f);

    motor_1.set_kd(1.0f);
    motor_2.set_kd(1.0f);
    motor_3.set_kd(1.0f);
     
    motor_1.start();
    motor_2.start();
    motor_3.start();

    motors.add(0, motor_1);
    motors.add(1, motor_2);
    motors.add(2, motor_3);
   
    RUN_TEST(init_test);

    _app_run();

    return 0;
}







/*** keyboard capture & timer **************************************************************/



void _pos_down_1() {

    std::cout << "Key Down," << " pos: " << motor_1_pos  << std::endl;

    motor_1_pos--;
    motor_2_pos--;
    motor_3_pos--;

    if (motor_1_pos < 1) {
        motor_1_pos = 0;
        motor_2_pos = 0;
        motor_3_pos = 0;
    }

    motors.get(0)->set_position(motor_1_pos);
    motors.get(1)->set_position(motor_2_pos);
    motors.get(2)->set_position(motor_3_pos);

    motors.get(0)->move();
    motors.get(1)->move();
    motors.get(2)->move();
}

void _pos_up_1() {
     
    std::cout << "Key Up,"<< " pos: "<< motor_1_pos << std::endl;

    motor_1_pos++;
    motor_2_pos++;
    motor_3_pos++;

    if (motor_1_pos > 180) {
        motor_1_pos = 180;
        motor_2_pos = 180;
        motor_3_pos = 180;
    }

    motors.get(0)->set_position(motor_1_pos);
    motors.get(1)->set_position(motor_2_pos);
    motors.get(2)->set_position(motor_3_pos);

    motors.get(0)->move();
    motors.get(1)->move();
    motors.get(2)->move();
     
}

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


struct target_angle {
    float q1 = 0;
    float q2 = 0;
    float q3 = 0;
};

target_angle test_target;

void _deserid_coordinate(target_angle*obj , float x , float y , float z) {
    // todo
    obj->q1 = atan2(y, x) * 180.0 / M_PI; 
    obj->q2 = atan2(sqrt( (x*x) + (z*z) ), y) * 180.0 / M_PI;
    obj->q3 = -obj->q2;
    std::cout << "\ncalculated angles: " << obj->q1 << " , " << obj->q2 << " , " << obj->q3 << "\n";
}


void getThreeFloats(float& num1, float& num2, float& num3) {
    std::cout << "\n" << std::endl;
    std::cout << "please enter 3 x,y,z coord: ";
    std::cin >> num1 >> num2 >> num3;
    std::cout << "\nentered coords: " << num1 << " , " << num2 << " , " << num3 << "\n";
}


void _app_run()
{  
    while (true) {
         
        if (GetAsyncKeyState('W') & 0x8000) {
            _pos_up_1();
        }
         
        if (GetAsyncKeyState('S') & 0x8000) {
            _pos_down_1();
        }

        if (GetAsyncKeyState('P') & 0x8000) {

            float x, y, z;

            getThreeFloats(x,y,z);

            _deserid_coordinate(&test_target , x,y,z);

            motors.get(0)->set_angle(test_target.q1);
            motors.get(1)->set_angle(test_target.q2);
            motors.get(2)->set_angle(test_target.q3);

            motors.get(0)->move();
            motors.get(1)->move();
            motors.get(2)->move();

        }
         
        serial_port.async_run();
        Sleep(100);
    }
}