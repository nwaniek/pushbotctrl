#ifndef __RCMANAGER_HPP__5D6F2629_8E6B_4D9C_8964_6E4D4D3B45BA
#define __RCMANAGER_HPP__5D6F2629_8E6B_4D9C_8964_6E4D4D3B45BA

#include <stdint.h>

/**
 * manage multiple instances of RobotControl. each new instance needs to be
 * registered here so that 'global' actions can act on all remote controls.
 */

namespace nst {

class RobotControl;


/*
 * Warning: the functions provided in this file are not thread-safe!
 */

/**
 * rcman_register - register a new robot control. will return a unique ID
 */
uint8_t rcman_register(RobotControl *ctrl);

/**
 * rcman_unregister - remove a robot control from the managed list
 */
void rcman_unregister(RobotControl *ctrl);

/**
 * rcman_emergency_shutdown - initiate emergency shutdown on all ctrls
 */
void rcman_emergency_shutdown();



/**
 * get a new unique robot ID. It is usually not necessary to call this function
 * manually. rcman_register will return a unique ID!
 */
uint8_t rcman_get_unique_id();

/**
 * release a robot ID. Usually not required to be called. rcman_unregister
 * automatically does the job.
 */
void rcman_release_id(uint8_t id);


} // nst::


#endif /* __RCMANAGER_HPP__5D6F2629_8E6B_4D9C_8964_6E4D4D3B45BA */

