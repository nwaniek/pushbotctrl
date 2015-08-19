#ifndef __UTILS_HPP__B69DECA2_4EA9_41CC_9E83_A9BBBB6E06C6
#define __UTILS_HPP__B69DECA2_4EA9_41CC_9E83_A9BBBB6E06C6

#include <stdint.h>
#include <algorithm>
#include <deque>

/**
 * sign of a float, either -1 or 1 (0 will return 1 here)
 */
#define sgnf(a) (((a) < 0.0) ? -1.0 : 1.0)


/**
 * clamp a value between lower and upper
 */
template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

/**
 * insert a value into a deque such that the deque is sorted afterwards
 */
template <typename T>
typename std::deque<T>::iterator insert_sorted(std::deque<T> &v, T const& t) {
	return v.insert(std::upper_bound(v.begin(), v.end(), t), t);
}


void release_robot_id(uint8_t id);
uint8_t get_robot_id();

#endif /* __UTILS_HPP__B69DECA2_4EA9_41CC_9E83_A9BBBB6E06C6 */

