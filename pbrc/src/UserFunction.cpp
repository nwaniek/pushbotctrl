#include <iostream>
#include "UserFunction.hpp"
#include "RobotControl.hpp"

using namespace std;
using namespace nst;


void
demo_function_1(RobotControl * const control,
		shared_ptr<DVSEvent> dvs_ev,
		shared_ptr<SensorEvent> sensor_ev)
{
	static int i = 0;
	++i %= 1000;
	if (!i) {
		cout << "demo function 1 called 1000 times. robot " << unsigned(control->id()) << ". ";

		if (dvs_ev) {
			cout << "triggered by DVS event at ("
				<< dvs_ev->x << ", "
				<< dvs_ev->y << ")"
				<< std::endl;
		}
		if (sensor_ev) {
			cout << "triggered by Sensor Event ("
				<< sensor_ev->imu.m[0] << ", "
				<< sensor_ev->imu.m[1] << ", "
				<< sensor_ev->imu.m[2] << ")"
				<< std::endl;
		}

	}

	// if (!i) cout << "demo function 1 called 1000 times. robot " << unsigned(control->id()) << " event at (" << ev->x << ", " << ev->y << ")" << endl;


	// Do not send commands to the robot too often

	// TODO: example to make the robot move
	// voting system to drive left/right --> Braitenberg Vehicle
	// control->drive(0.1, 0.2);
}


/*
void
demo_function_2(RobotControl * const control, const DVSEvent *ev)
{
	static int i = 0;
	++i %= 5000;
	if (!i) cout << "demo function 2 called 5000 times: robot " << unsigned(control->id()) << " event at (" << ev->x << ", " << ev->y << ")" << endl;
}
*/
