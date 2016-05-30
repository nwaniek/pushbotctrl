#include "RobotControl.hpp"

#include "PushbotConnection.hpp"
#include "SensorsProcessor.hpp"
#include "BytestreamParser.hpp"
#include "Datatypes.hpp"
#include "Commands.hpp"
#include "utils.hpp"
#include "RCManager.hpp"

#include <QString>
#include <QThread>
#include <QTimer>

namespace nst {

RobotControl::
RobotControl()
{
	// register this instance of the robot control and get an ID
	_id = rcman_register(this);

	// initialize required timers, threads and connections
	_timer_uf = new QTimer();
	_timer_uf->setInterval(15);
	connect(_timer_uf, &QTimer::timeout, this, &RobotControl::onTimerUFTimeout);

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
	// invoke cleanup of user data (if necessary)
	resetUserData();

	// shut down objects
	_con->disconnect();

	delete _sensors;

	// shut down threads
	_parser_thread->quit();
	_con_thread->quit();

	rcman_unregister(this);
}


void RobotControl::
resetRobot()
{
	// always send an empty command first. This will push the PushBot's
	// state machine to a state that new commands
	_con->sendCommand(new commands::Empty());

	// enable event streaming, sensor streaming, and motor control
	_con->sendCommand(new commands::DVS(true));
	_con->sendCommand(new commands::MotorDriver(true));
	_con->sendCommand(new commands::IMU(true));

	// reset motor velocities to 0
	_con->sendCommand(new commands::MV0(0));
	_con->sendCommand(new commands::MV1(0));

	// disable top LEDs
	_con->sendCommand(new commands::LED());
	_con->sendCommand(new commands::LED());

	// disable buzzer
	_con->sendCommand(new commands::Buzzer());

	// disable laser pointer
	_con->sendCommand(new commands::LaserPointer());

}

void RobotControl::
onPushbotConnected()
{
	// initiate the robot.
	_is_connected = true;
	resetRobot();
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
	// turn off everything
	resetRobot();
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
setMotor0Speed(float m0speed)
{
	_con->sendCommand(new commands::MVD0(static_cast<int>(floor(m0speed))));
	_con->flush();
}


void RobotControl::
setMotorSpeeds(float m0speed, float m1speed)
{
	_con->sendCommand(new commands::MVD0(static_cast<int>(floor(m0speed))));
	_con->sendCommand(new commands::MVD1(static_cast<int>(floor(m1speed))));
	_con->flush();
}


void RobotControl::
setMotor1Speed(float m1speed)
{
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

void RobotControl::
enableLEDs(unsigned base_freq, float relative_front, float relative_back)
{
	if (!_is_connected) return;
	_con->sendCommand(new commands::LED(base_freq, relative_front, relative_back));
}

void RobotControl::
disableLEDs()
{
	if (!_is_connected) return;
	_con->sendCommand(new commands::LED());
}

void RobotControl::
enableLaserPointer(unsigned base_freq, float relative)
{
	if (!_is_connected) return;
	_con->sendCommand(new commands::LaserPointer(base_freq, relative));
}

void RobotControl::
disableLaserPointer()
{
	if (!_is_connected) return;
	_con->sendCommand(new commands::LaserPointer());
}

void RobotControl::
enableBuzzer(unsigned base_freq, float relative)
{
	if (!_is_connected) return;
	_con->sendCommand(new commands::Buzzer(base_freq, relative));
}

void RobotControl::
disableBuzzer()
{
	if (!_is_connected) return;
	_con->sendCommand(new commands::Buzzer());
}

uint8_t RobotControl::
id() const
{
	return _id;
}


void RobotControl::
setUserFunction(const UserFunction *fn)
{
	resetUserData();
	_userfn = fn;
	_timer_uf->start();
}


void RobotControl::
unsetUserFunction()
{
	_userfn = nullptr;
	_timer_uf->stop();
	resetUserData();
}


void RobotControl::
onSensorEvent(std::shared_ptr<SensorEvent> ev)
{
	if (_userfn) _userfn->fn(this, std::shared_ptr<DVSEvent>(), ev);
	emit sensorEvent(ev);
}


void RobotControl::
onTimerUFTimeout()
{
	if (_userfn) _userfn->fn(this, std::shared_ptr<DVSEvent>(), std::shared_ptr<SensorEvent>());
}


void RobotControl::
setUserData(void *data, void (*cleanup_fn)(void *data))
{
	resetUserData();
	this->_user_data = data;
	this->_user_cleanup_fn = cleanup_fn;
}

void* RobotControl::
getUserData()
{
	return _user_data;
}

void RobotControl::
resetUserData()
{
	if (_user_cleanup_fn) _user_cleanup_fn(_user_data);
	_user_data = nullptr;
	_user_cleanup_fn = nullptr;
}

void RobotControl::
sendUserFunctionData(int type, void *data)
{
	// TODO: check thread safety
	emit userFunctionData(_id, type, data);
}


} // nst::
