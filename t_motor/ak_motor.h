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

#include <functional>
#include <cstdint>
#include <cmath>

#include "control.h"
#include "ak_params.h"
#include "mit_parser.h"
#include "serial_crc.h"
#include "can_packet.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*****
**AK60 Notes:**

- if you wanna change mode, you must stop motor, and reset power, and after switch the mode
- there are 4 mode : "guide mode", "servo mode" and "MIT power mode", "fw update mode"
- there are 2 main working motor mode : "servo mode" and "MIT power mode",

	Servo Mode: (***)

	- Servo mode is a control mode used for speed and position control of motors.
	- In this mode, the speed and position of the motor are continuously monitored and controlled using feedback signals.
	- Feedback signals are typically provided by encoders or other position sensors.
	- Servo motors are commonly used in applications requiring high precision, speed, and position control.

	---

	MIT (Motor Integrated Tuning) Mode:  (**)

	- MIT mode is an automatic tuning or auto-tuning mode found in motor drives.
	- In this mode, the motor drive automatically analyzes the dynamic behavior between the motor and the load and adjusts control parameters (such as PID controller parameters) automatically.
	- This is used to achieve optimal performance of the motor and ensure system stability.
	- MIT mode can optimize the motor's performance more quickly and accurately compared to manual adjustments.
- if you wanna do fw update, after this you shoul make motor calibration !
**/
 
class ak_motor {

public:

	ak_motor() :id(1) {}

	ak_motor(uint8_t _id) :id(_id) {}

	virtual ~ak_motor() {}
	 
	void start() { 
		can_packet<CAN_PACKET_SIZE> p;
		p.id = get_id();
		std::memcpy(p.data, start_motor_data, CAN_PACKET_SIZE); 
		can_trasnmit(p);
	}

	void stop() {
		can_packet<CAN_PACKET_SIZE> p;
		p.id = get_id();
		std::memcpy(p.data, exit_motor_data, CAN_PACKET_SIZE);
		can_trasnmit(p);
	}
	
	void set_origin() {
		can_packet<CAN_PACKET_SIZE> p;
		p.id = get_id();
		std::memcpy(p.data, set_origin_data, CAN_PACKET_SIZE);
		can_trasnmit(p);
	}
	 
	virtual void receive_packet(can_packet<CAN_PACKET_SIZE>& packet) = 0;
	 
	virtual void can_trasnmit(can_packet<CAN_PACKET_SIZE>& packet) = 0;
	 
	virtual void set_position(float val) = 0;
	virtual void set_velocity(float val) = 0;
	virtual void set_torque(float val) = 0;
	virtual void set_kp(float val) = 0;
	virtual void set_kd(float val) = 0;
	virtual void move() = 0;
	virtual void set_angle(float val) = 0;

	virtual void move(float pos, float vel, float tork, float kp, float kd) = 0;


	void set_id(uint8_t _id) { id = _id; }

	uint8_t get_id() const { return id;  };
 
private:
	 
	uint8_t    id;
};


template <typename MOTOR_TYPE>
class ak_motor_mit_mode : public ak_motor  {

public:

	ak_motor_mit_mode(uint8_t id) : ak_motor(id){}

	ak_motor_mit_mode( ) : ak_motor() {}

	~ak_motor_mit_mode() = default;
	 
	void move() override {
		_move(params);
	};


	void receive_packet(can_packet<CAN_PACKET_SIZE>& packet) override
	{
		if (packet.id == get_id()) {
			parser(packet);
		}
	}

	void can_trasnmit(can_packet<CAN_PACKET_SIZE> &packet)override {
		if(get_id() == packet.id)   // for optional protection
		{
			if(can_transmit_f)
				can_transmit_f(packet);
		} 	
	};

	/** 0 - 360 degree */
	void set_angle(float val)override {  // fake position 
		if (val > -360 && val < 360)
		{ 
			params.position = val * M_PI / 180;  // TODO , test et doðru mu? eksi de ne olacak?
		} 
	}
	void set_position(float val) override {
		// todo control range
		params.position = val;
	}
	void set_velocity(float val)override {
		// todo control range
		params.velocity = val;
	}
	void set_torque(float val)override {
		// todo control range
		params.torque = val;
	}
	void set_kp(float val) override {
		// todo control range
		params.kp = val;
	}
	void set_kd(float val)override {
		// todo control range
		params.kd = val;
	}

	void move(float pos , float vel , float tork , float kp , float kd)override {
		control_params<MOTOR_TYPE> m_param;
		m_param.position = pos;
		m_param.velocity = vel;
		m_param.torque = tork;
		m_param.kp = kp;
		m_param.kd = kd;
		_move(m_param);
	}

	template <typename Function>
	void bind_can_transmit(Function&& function) {
		can_transmit_f = std::bind(std::forward<Function>(function),
			                     std::placeholders::_1 );
	}
	  
	template <typename Function, typename Class>
	void bind_can_transmit(Function&& function, Class* handler) {
		can_transmit_f = std::bind(std::forward<Function>(function),
								 handler,
								 std::placeholders::_1 );
	}
	  
private:
	 
	void _move(control_params<MOTOR_TYPE> &param)
	{
		int32_t pInt = param.get_position_int();
		int32_t vInt = param.get_velocity_int();
		int32_t tInt = param.get_torque_int();
		int32_t kpInt = param.get_kp_int();
		int32_t kdInt = param.get_kd_int();

		can_packet<CAN_PACKET_SIZE> packet;
		 
		packet.id = get_id();

		packet.data[0] = pInt >> 8;
		packet.data[1] = pInt & 0xFF;
		packet.data[2] = vInt >> 4;
		packet.data[3] = ((vInt & 0xF) << 4 | (kpInt >> 8));
		packet.data[4] = kpInt & 0xFF;
		packet.data[5] = kdInt >> 4;
		packet.data[6] = ((kdInt & 0xF) << 4 | (tInt >> 8));
		packet.data[7] = tInt & 0xFF;

		if (can_transmit_f)
			can_transmit_f(packet);
	}


	/*********************/

	mit_receive_manager<MOTOR_TYPE> parser;

	control_params<MOTOR_TYPE> params;

	std::function<void(can_packet<CAN_PACKET_SIZE>)> can_transmit_f;
  
};




template <typename MOTOR_TYPE>
class ak_motor_servo_mode : ak_motor {

	// todo add serial etc..

};