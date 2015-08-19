#ifndef __ROBOTCONTROL_HPP__32EABE1A_2F0D_4AAB_B831_EFC05DE84126
#define __ROBOTCONTROL_HPP__32EABE1A_2F0D_4AAB_B831_EFC05DE84126

#include <QObject>

// forward declarations
class QThread;
class QString;

namespace nst {

// forward declarations
class PushbotConnection;
class SensorsProcessor;
class BytestreamParser;
struct DVSEvent;

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
	 * drive the robot, allowed are commands to live within [-1,1] for each
	 * motor
	 */
	void drive(const float x, const float y);

	/*
	 * enable/disable event streaming
	 */
	void enableEventstream();
	void disableEventstream();

	// TODO: change this!!
	void sendCommand(const commands::Command *cmd);

signals:
	void connected();
	void disconnected();
	void DVSEventReceived(const DVSEvent *ev);
	void responseReceived(const QString *str);

private slots:
	void onPushbotConnected();
	void onPushbotDisconnected();
	void onDVSEventReceived(const DVSEvent *ev);
	void onResponseReceived(const QString *str);

private:
	QThread *_con_thread;
	QThread *_parser_thread;

	PushbotConnection *_con;
	BytestreamParser *_parser;

	bool _is_connected = false;
};


} // nst::

#endif /* __ROBOTCONTROL_HPP__32EABE1A_2F0D_4AAB_B831_EFC05DE84126 */

