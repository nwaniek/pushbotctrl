#ifndef __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619
#define __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619

#include <cstdint>

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
	int16_t *a; // acc  (g)
	int16_t *g; // gyro (deg/s)
	int16_t *m; // mag  (uT)
	typedef enum{
		ACCELEROMETER,
		GYROSCOPE,
		MAGNETOMETER,
	} sensortype_t;
	typedef enum{
		XAXIS,
		YAXIS,
		ZAXIS,
	} sensoraxis_t;
	SensorsEvent(){
			a = (int16_t*)calloc(3, sizeof(int16_t));
			g = (int16_t*)calloc(3, sizeof(int16_t));
			m = (int16_t*)calloc(3, sizeof(int16_t));
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




#endif /* __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619 */

