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
}

void
demo_function_2(RobotControl * const control,
		shared_ptr<DVSEvent> dvs_ev,
		shared_ptr<SensorEvent> sensor_ev)
{
	static int i = 0;
	++i %= 5000;
	if (!i) {
		cout << "demo function 2 called 5000 times. robot " << unsigned(control->id()) << ". ";
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

	// every user function gets called every 15ms without a valid DVS or
	// Sensor event -> use this to send commands to the robot

	// TODO: example to make the robot move
	// voting system to drive left/right --> Braitenberg Vehicle
	if (!sensor_ev && !dvs_ev)
		control->drive(0.0, 0.2);
}
