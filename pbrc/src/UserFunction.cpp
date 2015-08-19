#include <iostream>
#include "UserFunction.hpp"
#include "RobotControl.hpp"

using namespace std;
using namespace nst;


void
demo_function_1(const RobotControl *control, const DVSEvent *ev)
{
	cout << "demo function 1: robot " << control->id() << " event at (" << ev->x << ", " << ev->y << endl;

	// TODO: example to make the robot move
}


void
demo_function_2(const RobotControl *control, const DVSEvent *ev)
{
	cout << "demo function 2: robot " << control->id() << " event at (" << ev->x << ", " << ev->y << endl;
}
