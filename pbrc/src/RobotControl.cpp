#include "RobotControl.hpp"
#include "PushbotConnection.hpp"
#include "SensorsProcessor.hpp"
#include "BytestreamParser.hpp"
#include "Datatypes.hpp"
#include "Commands.hpp"
#include "utils.hpp"
#include "RCManager.hpp"

#include <iostream>

namespace nst {

RobotControl::
RobotControl()
{
	// register this instance of the robot control and get an ID
	_id = rcman_register(this);

	// initialize required threads and connections
	_con_thread = new QThread();
	_parser_thread = new QThread();

	_con = new PushbotConnection();
	_con->moveToThread(_con_thread);

	_parser = new BytestreamParser(_id);
	_parser->moveToThread(_parser_thread);

	_sensors = new SensorsProcessor();
	connect(_sensors, &SensorsProcessor::sensorEvent, this, &RobotControl::onSensorEvent);

	// connect the worker objects
	connect(_con, &PushbotConnection::dataReady, _parser, &BytestreamParser::parseData, Qt::QueuedConnection);

	// forward events from the lower level
	connect(_con, &PushbotConnection::connected, this, &RobotControl::onPushbotConnected, Qt::QueuedConnection);
	connect(_con, &PushbotConnection::disconnected, this, &RobotControl::onPushbotDisconnected, Qt::QueuedConnection);
	connect(_parser, &BytestreamParser::eventReceived, this, &RobotControl::onDVSEventReceived, Qt::QueuedConnection);
	connect(_parser, &BytestreamParser::responseReceived, this, &RobotControl::onResponseReceived, Qt::QueuedConnection);

	// manage cleanup
	connect(_parser_thread, &QThread::finished, _parser, &BytestreamParser::deleteLater);
	connect(_parser_thread, &QThread::finished, _parser_thread, &QThread::deleteLater);
	connect(_con_thread, &QThread::finished, _con, &PushbotConnection::deleteLater);
	connect(_con_thread, &QThread::finished, _con_thread, &QThread::deleteLater);

	// start the threads
	_con_thread->start();
	_parser_thread->start();
}


RobotControl::
~RobotControl()
{
	// shut down objects
	_con->disconnect();

	delete _sensors;

	// shut down threads
	_parser_thread->quit();
	_con_thread->quit();

	rcman_unregister(this);
}


void RobotControl::
sendCommand(const commands::Command *cmd)
{
	// TODO: CHANGE THIS!
	_con->sendCommand(cmd);
}


void RobotControl::
onPushbotConnected()
{
	// initiate the robot.
	_is_connected = true;

	// always send an empty command first. This will push the PushBot's
	// state machine to a state that new commands
	_con->sendCommand(new commands::Empty);

	// enable event streaming and motor control
	_con->sendCommand(new commands::DVS(true));
	_con->sendCommand(new commands::MotorDriver(true));
	// reset motor velocities to 0
	_con->sendCommand(new commands::MV0(0));
	_con->sendCommand(new commands::MV1(0));
	// enable sensory data stream for gyro, acc, mag
	_con->sendCommand(new commands::IMU(true));

	emit connected();
}


void RobotControl::
onPushbotDisconnected()
{
	_is_connected = false;
	emit disconnected();
}


void RobotControl::
onDVSEventReceived(DVSEvent *ev)
{
	// turn the pointer into a shared memory object. data comes from the
	// parser and is now in our thread.
	auto _ev = std::make_shared<DVSEvent>(std::move(*ev));
	if (_userfn) _userfn->fn(this, _ev, std::shared_ptr<SensorEvent>());
	emit DVSEventReceived(_ev);
}


void RobotControl::
onResponseReceived(QString *str)
{
	if (!str) return;

	_sensors->parseString(str);
	emit responseReceived(str);
        delete str;
}


void RobotControl::
connectRobot(const QString IP, uint16_t port)
{
	_con->connect(IP, port);
}

void RobotControl::
disconnectRobot()
{
	_con->sendCommand(new commands::MV0(0));
	_con->sendCommand(new commands::MV1(0));
	_con->flush();
	_con->disconnect();
}


bool RobotControl::
isConnected()
{
	return this->_is_connected;
}


void RobotControl::
drive(float x, float y)
{
	if (!_is_connected) return;

	// make sure x,y are in [-1, 1]
	x = clamp(x, -1.0f, 1.0f);
	y = clamp(y, -1.0f, 1.0f);

	// compute speeds
	float max_speed = 100;
	float norm = sqrt(x*x + y*y);
	float m0speed = max_speed * norm * SGNF(y);
	float m1speed = max_speed * norm * SGNF(y);

	// compute angle
	float angle = atan2(y, x);
	float aangle = fabsf(angle);

	float m0mul = 1.0f;
	float m1mul = 1.0f;

	if (aangle < M_PI / 2.0)
		m1mul = aangle / (M_PI / 2.0);
	else
		m0mul = (M_PI - aangle) / (M_PI / 2.0);

	m0speed *= m0mul;
	m1speed *= m1mul;

	// finally send commands. use decaying ones
	_con->sendCommand(new commands::MVD0(static_cast<int>(floor(m0speed))));
	_con->sendCommand(new commands::MVD1(static_cast<int>(floor(m1speed))));
	_con->flush();
}


void RobotControl::
enableEventstream()
{
	if (!_is_connected) return;
	_con->sendCommand(new commands::DVS(true));
}

void RobotControl::
disableEventstream()
{
	if (!_is_connected) return;
	_con->sendCommand(new commands::DVS(false));
}


uint8_t RobotControl::
id() const
{
	return _id;
}


void RobotControl::
setUserFunction(const UserFunction *fn)
{
	_userfn = fn;
}


void RobotControl::
unsetUserFunction()
{
	_userfn = nullptr;
}


void RobotControl::
onSensorEvent(std::shared_ptr<SensorEvent> ev)
{
	if (_userfn) _userfn->fn(this, std::shared_ptr<DVSEvent>(), ev);
	emit sensorEvent(ev);
}

} // nst::
