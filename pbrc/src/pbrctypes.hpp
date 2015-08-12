#ifndef __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619
#define __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619

#include <cstdint>

/**
 * struct DVSEvent - A single DVS event.
 *
 */
struct DVSEvent {
	uint16_t x, y, p;
	uint64_t t;
};


/**
 * enum DVSEventFormat - enumeration of different event formats.
 */
enum DVSEventFormat {
	EVENT_FORMAT_0BYTES,
	EVENT_FORMAT_2BYTES,
	EVENT_FORMAT_3BYTES,
};


#endif /* __PBRCTYPES_HPP__FA25AD5E_D235_4C44_88AE_DCAF7423C619 */

