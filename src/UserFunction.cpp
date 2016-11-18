#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <algorithm>
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
	if (!sensor_ev && !dvs_ev)
		control->drive(0.0, 0.2);
}


/*
 *
 * reconstructing the LED tracking mechanism for the robot chain
 *
 */
/*
// {{{
// const float TimeDevCurve[] = {1,9.801987e-01,9.231163e-01,8.352702e-01,7.261490e-01,6.065307e-01,4.867523e-01,3.753111e-01,2.780373e-01,1.978987e-01,1.353353e-01,8.892162e-02,5.613476e-02,3.404745e-02,1.984109e-02,1.110900e-02,5.976023e-03,3.088715e-03,1.533811e-03,7.318024e-04,3.354626e-04,1.477484e-04,6.252150e-05,2.541935e-05,9.929504e-06,3.726653e-06,1.343812e-06,4.655716e-07,1.549753e-07,4.956405e-08,1.522998e-08,4.496349e-09,1.275408e-09,3.475891e-10,9.101471e-11,2.289735e-11,5.534610e-12,1.285337e-12,2.867975e-13,6.148396e-14,1.266417e-14,2.506222e-15,4.765305e-16,8.705427e-17,1.527980e-17,2.576757e-18,4.175010e-19,6.499348e-20,9.720985e-21,1.396944e-21,1.928750e-22,2.558592e-23,3.261027e-24,3.993337e-25,4.698355e-26,5.311092e-27,5.768330e-28,6.019280e-29,6.034861e-30,5.813239e-31,5.380186e-32,4.784149e-33,4.087335e-34,3.355089e-35,2.646038e-36,2.005009e-37,1.459704e-38,1.021037e-39,6.861930e-41,4.430772e-42,2.748785e-43,1.638439e-44,9.383138e-46,5.162905e-47,2.729407e-48,1.386343e-49,6.765524e-51,3.172198e-52,1.429050e-53,6.185328e-55,2.572209e-56,1.027728e-57,3.945282e-59,1.455145e-60,5.156591e-62,1.755688e-63,5.743283e-65,1.805100e-66,5.450930e-68,1.581496e-69,4.408531e-71,1.180723e-72,3.038296e-74,7.511740e-76,1.784346e-77,4.072359e-79,8.929787e-81,1.881327e-82,3.808166e-84,7.406204e-86,1.383897e-87,2.484505e-89,4.285527e-91,7.102264e-93,1.130883e-94,1.730082e-96,2.542986e-98,3.591282e-100,4.872852e-102,6.352508e-104,7.956744e-106,9.575330e-108,1.107134e-109,1.229915e-111,1.312739e-113,1.346200e-115,1.326383e-117,1.255616e-119,1.142017e-121,9.979685e-124,8.378943e-126,6.759115e-128,5.238641e-130,3.900997e-132,2.791007e-134,1.918556e-136,1.267115e-138,8.040555e-141,4.902123e-143,2.871511e-145,1.616088e-147,8.738722e-150,4.540032e-152,2.266200e-154,1.086840e-156,5.007966e-159,2.217100e-161,9.430558e-164,3.854053e-166,1.513304e-168,5.709040e-171,2.069322e-173,7.206449e-176,2.411253e-178,7.751620e-181,2.394255e-183,7.105203e-186,2.025866e-188,5.549749e-191,1.460710e-193,3.693883e-196,8.974915e-199,2.095105e-201,4.699043e-204,1.012608e-206,2.096532e-209,4.170516e-212,7.970881e-215,1.463697e-217,2.582403e-220,4.377491e-223,7.129428e-226,1.115610e-228,1.677252e-231,2.422771e-234,3.362440e-237,4.483582e-240,5.744125e-243,7.070512e-246,8.361921e-249,9.501441e-252,1.037292e-254,1.088030e-257,1.096501e-260,1.061709e-263,9.877109e-267,8.828412e-270,7.581646e-273,6.255654e-276,4.959183e-279,3.777250e-282,2.764201e-285,1.943532e-288,1.312931e-291,8.521586e-295,5.314068e-298,3.183920e-301,1.832843e-304,1.013717e-307,5.386867e-311,2.750325e-314,1.349149e-317,6.378387e-321,0,0,0,0,0,0,0,0};
// const float DistanceDevCurve[] = {1,9.801987e-01,9.231163e-01,8.352702e-01,7.261490e-01,6.065307e-01,4.867523e-01,3.753111e-01,2.780373e-01,1.978987e-01,1.353353e-01,8.892162e-02,5.613476e-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const float xVTable[] = {-36,-3.487507e+01,-3.376800e+01,-3.267878e+01,-3.160742e+01,-3.055391e+01,-2.951826e+01,-2.850047e+01,-2.750053e+01,-2.651845e+01,-2.555422e+01,-2.460785e+01,-2.367934e+01,-2.276868e+01,-2.187588e+01,-2.100093e+01,-2.014384e+01,-1.930461e+01,-1.848323e+01,-1.767971e+01,-1.689404e+01,-1.612623e+01,-1.537628e+01,-1.464418e+01,-1.392994e+01,-1.323355e+01,-1.255503e+01,-1.189435e+01,-1.125153e+01,-1.062657e+01,-1.001947e+01,-9.430219e+00,-8.858826e+00,-8.305289e+00,-7.769608e+00,-7.251783e+00,-6.751814e+00,-6.269701e+00,-5.805444e+00,-5.359043e+00,-4.930498e+00,-4.519809e+00,-4.126976e+00,-3.752000e+00,-3.394879e+00,-3.055614e+00,-2.734205e+00,-2.430653e+00,-2.144956e+00,-1.877116e+00,-1.627131e+00,-1.395003e+00,-1.180730e+00,-9.843140e-01,-8.057536e-01,-6.450493e-01,-5.022010e-01,-3.772088e-01,-2.700725e-01,-1.807924e-01,-1.093682e-01,-5.580011e-02,-2.008804e-02,-2.232004e-03,2.232004e-03,2.008804e-02,5.580011e-02,1.093682e-01,1.807924e-01,2.700725e-01,3.772088e-01,5.022010e-01,6.450493e-01,8.057536e-01,9.843140e-01,1.180730e+00,1.395003e+00,1.627131e+00,1.877116e+00,2.144956e+00,2.430653e+00,2.734205e+00,3.055614e+00,3.394879e+00,3.752000e+00,4.126976e+00,4.519809e+00,4.930498e+00,5.359043e+00,5.805444e+00,6.269701e+00,6.751814e+00,7.251783e+00,7.769608e+00,8.305289e+00,8.858826e+00,9.430219e+00,1.001947e+01,1.062657e+01,1.125153e+01,1.189435e+01,1.255503e+01,1.323355e+01,1.392994e+01,1.464418e+01,1.537628e+01,1.612623e+01,1.689404e+01,1.767971e+01,1.848323e+01,1.930461e+01,2.014384e+01,2.100093e+01,2.187588e+01,2.276868e+01,2.367934e+01,2.460785e+01,2.555422e+01,2.651845e+01,2.750053e+01,2.850047e+01,2.951826e+01,3.055391e+01,3.160742e+01,3.267878e+01,3.376800e+01,3.487507e+01,36};
const float yVTable[] = {-60,-5.999936e+01,-5.999825e+01,-5.999627e+01,-5.999274e+01,-5.998653e+01,-5.997585e+01,-5.995786e+01,-5.992818e+01,-5.988026e+01,-5.980458e+01,-5.968761e+01,-5.951076e+01,-5.924921e+01,-5.887091e+01,-5.833593e+01,-5.759639e+01,-5.659731e+01,-5.527879e+01,-5.357953e+01,-5.144203e+01,-4.881920e+01,-4.568205e+01,-4.202773e+01,-3.788705e+01,-3.333013e+01,-2.846923e+01,-2.345761e+01,-1.848373e+01,-1.376093e+01,-9.512860e+00,-5.956276e+00,-3.282706e+00,-1.641353e+00,-8.206765e-01,1.210878e+00,6.583317e+00,1.509574e+01,2.521450e+01,3.512668e+01,4.350328e+01,4.977979e+01,5.403293e+01,5.668319e+01,5.822562e+01,5.907669e+01,5.952828e+01,5.976163e+01,5.988026e+01,5.994002e+01,5.996999e+01,5.998499e+01,5.999249e+01,5.999625e+01,5.999812e+01,5.999906e+01,5.999953e+01,5.999977e+01,5.999988e+01,5.999994e+01,5.999997e+01,5.999999e+01,5.999999e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,6.000000e+01,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60};
// }}}
*/

// linearized versions of the lookup-tables above
float
fast_xvel(uint32_t x) {
	//if (x < 0)
	//	return -40;
	if (x <= 20)
		return 1.0 * (float)x - 40.0;
	if (x <= 50)
		return 0.5 * (float)x - 30.0;
	if (x <= 80)
		return 1.0/3.0 * (float)x - (21.0 + 2.0/3.0);
	if (x <= 110)
		return 0.5 * (float)x - 35.0;
	if (x <= 130)
		return 1.0 * (float)x - 90.0;
	return 40;
}

float
fast_yvel(uint32_t y) {
	if (y <= 20)
		return -60.0;
	if (y <= 30)
		return 5.5 * (float)y - 170;
	if (y <= 40)
		return 1.0 * y - 35;
	if (y <= 50)
		return 5.5 * y - 215.0;
	return 60.0;
}

#define LED_PERIOD 1000
#define DVS_SIZE 128

/*
 * a point in 2 dimensions
 */
struct Vec2ui {
	uint16_t x, y;
};

struct Vec2f {
	float x, y;
};

/*
 * user data that will be used for LED tracking
 */
struct led_tracking_data {
	uint64_t timestamps[DVS_SIZE][DVS_SIZE] = {{0}};
	Vec2f tracker{64.0, 35.0};
	uint32_t i{0};
	uint32_t j{0};
	uint16_t ev_counter{0};
};


/*
 * delete an allocated data member from the robot control
 */
void cleanup_led_tracking(void *raw_data)
{
	if (raw_data == nullptr) return;
	auto data = static_cast<led_tracking_data*>(raw_data);
	delete data;
}


void* init_led_tracking(RobotControl * const control)
{
	led_tracking_data *data = new led_tracking_data;
	control->setUserData(data, cleanup_led_tracking);
	return static_cast<void*>(data);
}



void
_led_tracker(bool with_driving_commands,
		RobotControl * const control,
		shared_ptr<DVSEvent> dvs_ev,
		shared_ptr<SensorEvent> sensor_ev)
{
	// set up the data struct if necessary
	void *raw_data = control->getUserData();
	if (raw_data == nullptr) raw_data = init_led_tracking(control);
	led_tracking_data *data = static_cast<led_tracking_data*>(raw_data);

	const float tao = 0.8f;

	// TODO: change returns with goto

	// only watch off polarity
	if (dvs_ev && (dvs_ev->p == 0)) {
		// sanity check the event
		float x = float(dvs_ev->y);
		float y = float(dvs_ev->x);
		if (x > 128.f || y > 128.f) return;

		// use only every other event -> reduce workload
		++data->ev_counter;
		data->ev_counter %= 2;
		if (data->ev_counter) return;

		uint64_t DVSTimestamp = dvs_ev->t;
		uint64_t lastTimestamp = data->timestamps[dvs_ev->y][dvs_ev->x];
		int64_t deltaT = DVSTimestamp - lastTimestamp;
		// int64_t timeDiff = (deltaT > LED_PERIOD) ? deltaT - LED_PERIOD : LED_PERIOD - deltaT;
		int64_t timeDiff = std::abs(LED_PERIOD - deltaT);

		// only look at things that have a time-jitter around our
		// preferred period
		constexpr int TIME_JITTER = 5;
		if (timeDiff < TIME_JITTER) {

			// temporal weight, linear
			//float weightT = 1.0 - std::abs(( - float(timeDiff)) / float(TIME_JITTER));
			float weightT = 1.0 - std::abs( timeDiff / float(LED_PERIOD));

			// weightX, linear distance
			float weightX = 1.0 - std::abs((x - data->tracker.x) / 128.f);

			// weightY, linear distance
			float weightY = 1.0 - std::abs((y - data->tracker.y) / 128.f);

			// combine
			weightX = tao * weightX * weightT;
			weightY = tao * weightY * weightT;

			float debug = weightT * weightX;

			// determine certainty by weight
			std::cout << debug << ", " << weightX << ", " << weightY << std::endl;

			// only consider events that really match. ignore this
			// fact every 10th event to allow re-latching on
			// blinking LED
			data->j++;
			data->j %= 10;
			if (data->j != 0 && debug < 0.7) {
				std::cout << "ignoring event" << std::endl;
				return;
			}

			data->tracker.x = (1.f - weightX) * data->tracker.x + weightX * x;
			data->tracker.y = (1.f - weightY) * data->tracker.y + weightY * y;

			// clamp to data range
			data->tracker.x = data->tracker.x > 127.0 ? 127.0 : data->tracker.x;
			data->tracker.y = data->tracker.y > 127.0 ? 127.0 : data->tracker.y;
			data->tracker.x = data->tracker.x < .0 ? .0 : data->tracker.x;
			data->tracker.y = data->tracker.y < .0 ? .0 : data->tracker.y;

			// increase certainty that we think we tracked the LED
			data->i++;
		}

		// update table of timestamps
		data->timestamps[dvs_ev->y][dvs_ev->x] = DVSTimestamp;
	}

	// the function gets called every 15ms without arguments -> send
	// commands to the robot
	if (!dvs_ev && !sensor_ev) {
		// if we are certain that we tracked the LED, move the robot
		if (with_driving_commands && (data->i > 5)) {
			uint32_t x, y;

			x = static_cast<uint32_t>(data->tracker.x);
			y = static_cast<uint32_t>(data->tracker.y);

			// the fast_xvel is designed to have zero crossing at
			// 65, so shift x by 1
			float vVer = fast_xvel(x + 1);
			float vHor = fast_yvel(y);

			// std::cout << "issuing driving command" << std::endl;

			float m0speed = + (vVer + vHor);
			float m1speed = - (vVer - vHor);

			control->setMotorSpeeds(m0speed, m1speed);
		}

		control->sendUserFunctionData(
				UFDT_LED_TRACKING_INFO,
				new led_tracking_info{
					static_cast<unsigned>(data->tracker.x),
					static_cast<unsigned>(data->tracker.y)});
		data->i = 0;
	}


	// emit some information every 1000 calls
	// if (!(data->i))
}


void
led_tracker_plain(
	RobotControl * const control,
	shared_ptr<DVSEvent> dvs_ev,
	shared_ptr<SensorEvent> sensor_ev)
{
	_led_tracker(false, control, dvs_ev, sensor_ev);
}


void
led_tracker_drive(
	RobotControl * const control,
	shared_ptr<DVSEvent> dvs_ev,
	shared_ptr<SensorEvent> sensor_ev)
{
	_led_tracker(true, control, dvs_ev, sensor_ev);
}
