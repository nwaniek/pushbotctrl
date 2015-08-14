#ifndef __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619
#define __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619

#include <cstdint>

namespace nst {

/**
 * struct DVSEvent - A single DVS event.
 *
 */
struct DVSEvent {
	uint64_t t;
	uint16_t x, y;
	uint8_t  p;

	typedef enum {
		TIMEFORMAT_0BYTES,
		TIMEFORMAT_2BYTES,
		TIMEFORMAT_3BYTES,
	} timeformat_t;
};


}

/**
 * ostream conversion of an event to a textual representation
 */
inline
std::ostream& operator<<(std::ostream &out, nst::DVSEvent &e) {
	out << "(" << e.x << "," << e.y << "," << (e.p > 0 ? 1 : 0) << "," << e.t << ")";
	return out;
}




#endif /* __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619 */

