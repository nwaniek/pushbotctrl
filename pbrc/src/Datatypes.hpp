#ifndef __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619
#define __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619

#include <ostream>
#include <cstdlib>
#include <cstdint>
#include <bitset>
#include <QString>


namespace nst {

/**
 * struct DVSEvent - A single DVS event.
 *
 */
struct DVSEvent {
	uint8_t id;
	uint64_t t;
	uint16_t x, y;
	uint8_t  p;

	typedef enum {
		TIMEFORMAT_0BYTES,
		TIMEFORMAT_2BYTES,
		TIMEFORMAT_3BYTES,
	} timeformat_t;
};

/**
 * struct SensorsEvent - A single sensors sample.
 *
 */
struct SensorsEvent {
	double *a; // acc  (g)
	double *g; // gyro (deg/s)
	double *m; // mag  (uT)
	typedef enum{
		GYROSCOPE,
		ACCELEROMETER,
		MAGNETOMETER,
	} sensortype_t;
	typedef enum{
		YAXIS,
		ZAXIS,
		XAXIS,
	} sensoraxis_t;
	SensorsEvent(){
			a = (double*)calloc(3, sizeof(double));
			g = (double*)calloc(3, sizeof(double));
			m = (double*)calloc(3, sizeof(double));
		       }
	~SensorsEvent() { free(a); free(g); free(m);}
};

/**
 * struct RPYEvent - A single estimate of RPY from all sensors.
 *
 */
struct RPYEvent {
	double *roll;  // [roll_a, roll_g, roll_m]
	double *pitch; // [pitch_a, pitch_g, pitch_m]
	double *yaw;   // [yaw_a, yaw_g, yaw_m]
	RPYEvent() {
			roll  = (double*)calloc(3, sizeof(double));
			pitch = (double*)calloc(3, sizeof(double));
			yaw   = (double*)calloc(3, sizeof(double));
		   }
	~RPYEvent(){ free(roll); free(pitch); free(yaw); }
};


/**
 * A user function
 */
typedef struct {
	const char *name;
	void (*fn)();
} UserFunction;


}

/**
 * ostream conversion of an event to a textual representation
 */
inline
std::ostream& operator<<(std::ostream &out, nst::DVSEvent &e)
{
	out << "(" << e.x << "," << e.y << "," << (e.p > 0 ? 1 : 0) << "," << e.t << ")";
	return out;
}

/**
 * decode incoming sensory values from IMU (encoding is Q16)
 */
inline
double decodeSensorVal(QString str)
{
    // get the 32 bit binary representation of the decoded decimal in the Q16 string
    std::bitset<32> bStr(str.toUInt(NULL, 16));
    // extract the correct signed value from 2's complement representation
    return  ((bStr.test(31))?(((double)(bStr.flip(31).to_ulong()) - pow(2, 31))/pow(2,16)):((double)(bStr.to_ulong())/pow(2,16)));
}



#endif /* __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619 */

