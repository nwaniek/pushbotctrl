#ifndef __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E
#define __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E

#include "Datatypes.hpp"
using namespace nst;


/*
 * Declarations of your functions go here
 */
void demo_function_1();
void demo_function_2();


/*
 * Add all the functions that you want to use to this list. Entries in this list
 * need to be of the form {"descriptive name", function_name}.
 */
static const UserFunction user_functions[] = {
	{"First demo function",  demo_function_1},
	{"Second demo function", demo_function_2},
};


#endif /* __USERFUNCTION_HPP__9CBC23DF_9FA8_4117_9F7A_371081D2E31E */

