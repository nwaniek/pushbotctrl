#include <iostream>
#include "UserFunction.hpp"
#include "RobotControl.hpp"

using namespace std;
using namespace nst;


void
demo_function_1(const RobotControl *control, const DVSEvent *ev)
{
	static int i = 0;
	++i;
	if (i == 1000) {
		cout << "demo function 1 called 1000 times. robot " << unsigned(control->id()) << " event at (" << ev->x << ", " << ev->y << ")" << endl;
		i = 0;
	}


	// TODO: example to make the robot move
	// voting system to drive left/right --> Braitenberg Vehicle

}


void
demo_function_2(const RobotControl *control, const DVSEvent *ev)
{
	cout << "demo function 2: robot " << control->id() << " event at (" << ev->x << ", " << ev->y << endl;
}
