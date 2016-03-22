#ifndef __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E
#define __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E

#include <memory>
#include "Datatypes.hpp"

using namespace std;
using namespace nst;


/*
 * TODO: DESCRIPTION. Each user function will be called at least every 15ms
 */

void demo_function_1(
		RobotControl * const control,
		shared_ptr<DVSEvent> dvs_ev,
		shared_ptr<SensorEvent> sensor_ev);

void demo_function_2(
		RobotControl * const control,
		shared_ptr<DVSEvent> dvs_ev,
		shared_ptr<SensorEvent> sensor_ev);

void led_tracker(
		RobotControl * const control,
		shared_ptr<DVSEvent> dvs_ev,
		shared_ptr<SensorEvent> sensor_ev);


/*
 * Add all the functions that you want to use to this list. Entries in this list
 * need to be of the form {"descriptive name", function_name}.
 */
static const UserFunction user_functions[] = {
	{"LED Tracker", led_tracker},
	{"First demo function",  demo_function_1},
	{"Second demo function", demo_function_2},
};


#endif /* __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E */

