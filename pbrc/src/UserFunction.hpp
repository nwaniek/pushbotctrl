#ifndef __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E
#define __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E

#include <memory>
#include "Datatypes.hpp"

using namespace std;
using namespace nst;


/*
 * Declarations of your functions go here. they all have to accept the same
 * arguments:
 *	RobotControl *control
 *	DVSEvent *ev
 * control is a pointer to the control interface for the robot from which
 * the event is coming, and ev a pointer to the event structure.
 *
 * Do _NOT_ call delete on control or event!
 */

void demo_function_1(RobotControl * const control, const DVSEvent *ev);
void demo_function_2(RobotControl * const control, const DVSEvent *ev);


/*
 * Add all the functions that you want to use to this list. Entries in this list
 * need to be of the form {"descriptive name", function_name}.
 */
static const UserFunction user_functions[] = {
	{"First demo function",  demo_function_1},
	{"Second demo function", demo_function_2},
};


#endif /* __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E */

