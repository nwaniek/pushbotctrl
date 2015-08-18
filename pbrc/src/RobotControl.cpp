#include "RobotControl.hpp"
#include "PushbotConnection.hpp"
#include "SensorsProcessor.hpp"
#include "BytestreamParser.hpp"
#include "Datatypes.hpp"
#include "Commands.hpp"

#include <iostream>

namespace nst {

RobotControl::
RobotControl()
{
	// initialize required threads and connections
	_con_thread = new QThread();
	_parser_thread = new QThread();

	_con = new PushbotConnection();
	_con->moveToThread(_con_thread);

	_parser = new BytestreamParser();
	_parser->moveToThread(_parser_thread);

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

	// shut down threads
	_parser_thread->quit();
	_con_thread->quit();
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
	std::cout << "connected to PushBot" << std::endl;
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
	std::cout << "disconnected from PushBot" << std::endl;
	_is_connected = false;
	emit disconnected();
}


void RobotControl::
onDVSEventReceived(const DVSEvent *ev)
{
	emit DVSEventReceived(ev);
}


void RobotControl::
onResponseReceived(const QString *str)
{
	// TODO: Sensor parsing stuff goes here... ? maybe it should go
	// somewhere else (like in the parser thread, connected by
	// signals/slots)

	emit responseReceived(str);
}


void RobotControl::
connectRobot(const QString IP, uint16_t port)
{
	_con->connect(IP, port);
}

void RobotControl::
disconnectRobot()
{
	_con->disconnect();
}


bool RobotControl::
isConnected()
{
	return this->_is_connected;
}

} // nst::
