#include "utils.hpp"
#include <deque>

static std::deque<uint8_t> ids;

void
prepare_robot_ids()
{
	// TODO: HAVE FUN! MAKE THIS A COMPILE TIME TEMPLATE :D
	for (uint8_t i = 0; i < 255; i++)
		ids.push_back(i);
	ids.push_back(255);
}


void
return_robot_id(uint8_t id)
{
	insert_sorted(ids, id);
}


uint8_t
get_robot_id()
{
	auto id = ids.front();
	ids.pop_front();
	return id;
}
