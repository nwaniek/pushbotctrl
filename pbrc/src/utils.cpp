#include "utils.hpp"
#include <deque>


// compiler will fill in the IDs at compile time this way
static std::deque<uint8_t> ids(256);
static void
prepare_robot_ids()
{
	for (uint8_t i = 0; i < 255; i++)
		ids[i] = i;
	ids[255] = 255;
}
int _q = (prepare_robot_ids(), 0);

void
release_robot_id(uint8_t id)
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
