#include "Dialog.hpp"
#include <QPushButton>
#include <QGridLayout>
#include <QThread>
#include <QLineEdit>
#include <QRegularExpression>
#include <QDebug>
#include <iostream>
#include <cmath>
#include <ctime>

#include "Datatypes.hpp"
#include "BytestreamParser.hpp"
#include "PushbotConnection.hpp"
#include "DVSEventWidget.hpp"
#include "NavigationWidget.hpp"
#include "Commands.hpp"
#include "SensorsProcessor.hpp"

namespace nst {

Dialog::
Dialog(QWidget *parent) : QDialog(parent)
{
	// create widgets
	_edtIP = new QLineEdit("10.162.177.XX", this);
	_buttonConnect = new QPushButton("Connect", this);
	_buttonDisconnect = new QPushButton("Disconnect", this);
	_dvswidget = new DVSEventWidget(this);
	_navwidget = new NavigationWidget(this);
	_sensors = new SensorsProcessor(this);

	// connect signals + slots
	connect(_navwidget, &NavigationWidget::navigationUpdate, this, &Dialog::onNavigationUpdate);

	// layout
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(_edtIP, 0, 0, 1, 2);
	layout->addWidget(_navwidget, 1, 0);
	layout->addWidget(_dvswidget, 1, 1);
	layout->addWidget(_buttonConnect, 2, 0);
	layout->addWidget(_buttonDisconnect, 2, 1);
	layout->setSizeConstraint(QLayout::SetDefaultConstraint);
	setLayout(layout);

	// create required threads
	createThreads();
}

#define sgnf(a) (((a) < 0.0) ? -1.0 : 1.0)

void Dialog::
onNavigationUpdate(const QPointF pos)
{
	if (!_is_connected) return;
	// translate position from ([-1,1], [-1,1]) to a motor command

	// compute speeds
	float max_speed = 100;
	float norm = sqrt(pos.x() * pos.x() + pos.y() * pos.y());
	float m0speed = max_speed * norm * sgnf(pos.y());
	float m1speed = max_speed * norm * sgnf(pos.y());

	// compute angle
	float angle = atan2(pos.y(), pos.x());
	float aangle = fabsf(angle);

	float m0mul = 1.0f;
	float m1mul = 1.0f;

	if (aangle < M_PI / 2.0)
		m1mul = aangle / (M_PI / 2.0);
	else
		m0mul = (M_PI - aangle) / (M_PI / 2.0);

	m0speed *= m0mul;
	m1speed *= m1mul;

	_con->sendCommand(new commands::MV0(static_cast<int>(m0speed)));
	_con->sendCommand(new commands::MV1(static_cast<int>(m1speed)));
}

void Dialog::
createThreads() {

	// setup tcp in its own thread
	_con_thread = new QThread();
	_con = new PushbotConnection();
	_con->moveToThread(_con_thread);

	// connect the connection foo here
	connect(_buttonConnect, &QPushButton::clicked, this, &Dialog::onConnectClicked);
	connect(_buttonDisconnect, &QPushButton::clicked, this, &Dialog::onDisconnectClicked);

	// create the parser thread
	_parser_thread = new QThread();
	_parser = new BytestreamParser();
	_parser->moveToThread(_parser_thread);

	// connect the two worker objects
	connect(_con, &PushbotConnection::dataReady, _parser, &BytestreamParser::parseData, Qt::QueuedConnection);
	connect(_con, &PushbotConnection::connected, this, &Dialog::pushbotConnected, Qt::QueuedConnection);
	connect(_con, &PushbotConnection::disconnected, this, &Dialog::pushbotDisconnected, Qt::QueuedConnection);

	// connect the parser back to the Dialog
	connect(_parser, &BytestreamParser::eventReceived, this, &Dialog::onDVSEventReceived);
	connect(_parser, &BytestreamParser::responseReceived, this, &Dialog::onResponseReceived);

	// start the threads
	_con_thread->start();
	_parser_thread->start();
}

void Dialog::
onDVSEventReceived(const DVSEvent *ev)
{
	_dvswidget->newEvent(ev);
	delete ev;
}


void Dialog::
onResponseReceived(const QString *str)
{
	// TODO: parse the response into corresponding structs
	SensorsEvent *se = new SensorsEvent();
	// extract the content of the robot stream for each sensor
        QStringList rawResponse = str->split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
	rawResponse.removeAt(0);
	//qDebug()<<rawResponse;
	_sensors->newSample(se);
	delete se;
	delete str;
}


void Dialog::
pushbotConnected()
{
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
}


void Dialog::
pushbotDisconnected()
{
	std::cout << "disconnected from PushBot" << std::endl;
	_is_connected = false;
}


void Dialog::
onConnectClicked()
{
	_con->connect(_edtIP->text());
}


void Dialog::
onDisconnectClicked()
{
	_con->disconnect();
}

} // nst::
