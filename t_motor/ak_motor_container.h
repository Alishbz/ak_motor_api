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
#include <map> 
#include <cstdint>

/** incomplete types  **/
class ak_motor; 
 
 
class ak_motor_container {

public:

	ak_motor_container() {}

	~ak_motor_container() = default;
	 
	bool add(int _index, ak_motor& new_motor) {
		motors.emplace(_index, std::ref(new_motor));
		return true;
	}

	ak_motor* get(int _index) {
		auto it = motors.find(_index);

		if (it == motors.end()) {
			return nullptr;  
		}

		return &(it->second.get());
	}

private:

	std::map<int, std::reference_wrapper<ak_motor>> motors;
	  
};