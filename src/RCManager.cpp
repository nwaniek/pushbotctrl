#include "RCManager.hpp"
#include "utils.hpp"
#include "RobotControl.hpp"
#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;

namespace nst {

// static data to hide this from the outside. this effectively works as a
// singleton instance to the outside world
static vector<RobotControl*> ctrls;


uint8_t
rcman_register(RobotControl *ctrl) {
	ctrls.push_back(ctrl);
	return rcman_get_unique_id();
}


void
rcman_unregister(RobotControl *ctrl) {
	ctrls.erase(remove(ctrls.begin(), ctrls.end(), ctrl), ctrls.end());
	rcman_release_id(ctrl->id());
}


void
rcman_emergency_shutdown()
{
	// disconnect from all controls
	for (auto *ctrl: ctrls)
		ctrl->disconnectRobot();
}



// compiler will fill in the IDs at compile time this way
static std::deque<uint8_t> ids(256);
static void
_rcman_prepare_robot_ids()
{
	for (uint8_t i = 0; i < 255; i++)
		ids[i] = i;
	ids[255] = 255;
}
int _q = (_rcman_prepare_robot_ids(), 0);


void
rcman_release_id(uint8_t id)
{
	insert_sorted(ids, id);
}


uint8_t
rcman_get_unique_id()
{
	auto id = ids.front();
	ids.pop_front();
	return id;
}


} // nst::
