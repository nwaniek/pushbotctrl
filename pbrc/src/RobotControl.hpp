#ifndef __ROBOTCONTROL_HPP__32EABE1A_2F0D_4AAB_B831_EFC05DE84126
#define __ROBOTCONTROL_HPP__32EABE1A_2F0D_4AAB_B831_EFC05DE84126

#include <memory>
#include <QObject>

// forward declarations
class QThread;
class QString;

namespace nst {

// forward declarations
class PushbotConnection;
class SensorsProcessor;
class BytestreamParser;

struct UserFunction;
struct DVSEvent;
struct SensorEvent;

namespace commands {
	struct Command;
}


/**
 * RobotControl - Main class to operate the robot.
 *
 * Internally this class manages everything that runs in the background to
 * operate a robot. It will spawn threads for the data processing and socket
 * connections and forward the events to the user (or GUI).
 *
 * It is derived from QObject to expose the signal/slot mechanism such that the
 * GUI can listen on events, or directly hook into slots.
 *
 * Essentially it is a wrapper around all sub-classes, but exposes one interface
 * to 'the outside world' and hiding the internals.
 *
 * TODO: make the userfunction interface thread safe
 */
class RobotControl : public QObject
{
	Q_OBJECT

public:
	RobotControl();
	~RobotControl();

	void connectRobot(const QString IP, uint16_t port = 56000);
	void disconnectRobot();
	bool isConnected();

	/*
	 * reset a robot to its initial state
	 */
	void resetRobot();

	/*
	 * set a user function which will be called everytime an event is
	 * received.
	 */
	void setUserFunction(const UserFunction *fn);
	void unsetUserFunction();

	/*
	 * drive the robot, allowed are commands to live within [-1,1] for each
	 * motor
	 */
	void drive(const float x, const float y);

	/*
	 * enable/disable event streaming
	 */
	void enableEventstream();
	void disableEventstream();

	/**
	 * return the Robot Control ID
	 */
	uint8_t id() const;

signals:
	void connected();
	void disconnected();

	// TODO: change to shared_ptr ?
	void responseReceived(const QString *str);

	void DVSEventReceived(std::shared_ptr<DVSEvent> ev);
	void sensorEvent(std::shared_ptr<SensorEvent> ev);

private slots:
	void onPushbotConnected();
	void onPushbotDisconnected();
	void onDVSEventReceived(DVSEvent *ev);
	void onResponseReceived(QString *str);
	void onSensorEvent(std::shared_ptr<SensorEvent> ev);

private:
	void sendCommand(const commands::Command *cmd);


	QThread *_con_thread = nullptr;
	QThread *_parser_thread = nullptr;

	SensorsProcessor *_sensors = nullptr;
	PushbotConnection *_con = nullptr;
	BytestreamParser *_parser = nullptr;

	const UserFunction *_userfn = nullptr;

	bool _is_connected = false;

	// each robot control gets its own ID
	uint8_t _id;
};


} // nst::



#endif /* __ROBOTCONTROL_HPP__32EABE1A_2F0D_4AAB_B831_EFC05DE84126 */

