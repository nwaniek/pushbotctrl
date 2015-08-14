#include "Dialog.hpp"
#include <QPushButton>
#include <QGridLayout>
#include <QThread>
#include <iostream>

#include "Datatypes.hpp"
#include "BytestreamParser.hpp"
#include "PushbotConnection.hpp"
#include "DVSEventWidget.hpp"
#include "Commands.hpp"

namespace nst {

Dialog::
Dialog(QWidget *parent) : QDialog(parent)
{
	// create widgets
	_buttonCreate = new QPushButton("Create Threads");
	_buttonConnect = new QPushButton("Connect");
	_buttonDisconnect = new QPushButton("Disconnect");
	_dvswidget = new DVSEventWidget(this);

	// connect signals + slots
	connect(_buttonCreate, &QPushButton::clicked, this, &Dialog::btnCreateClicked);

	// layout
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(_dvswidget, 0, 0);
	layout->addWidget(_buttonCreate, 1, 0);
	layout->addWidget(_buttonConnect, 2, 0);
	layout->addWidget(_buttonDisconnect, 3, 0);
	layout->setSizeConstraint(QLayout::SetDefaultConstraint);
	setLayout(layout);
}

void Dialog::
btnCreateClicked() {

	std::cout << "create clicked" << std::endl;

	// setup tcp in its own thread
	_con_thread = new QThread();
	_con = new PushbotConnection();
	_con->moveToThread(_con_thread);

	// connect the connection foo here
	connect(_buttonConnect, &QPushButton::clicked, _con, &PushbotConnection::connect, Qt::QueuedConnection);
	connect(_buttonDisconnect, &QPushButton::clicked, _con, &PushbotConnection::disconnect, Qt::QueuedConnection);

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
	std::cout << str->toStdString() << std::endl;
	delete str;
}


void Dialog::
pushbotConnected()
{
	std::cout << "connected" << std::endl;

	_con->sendCommand(new commands::DVS(true));
	_con->sendCommand(new commands::MotorDriver(true));
	_con->sendCommand(new commands::MVD0(40));
	_con->sendCommand(new commands::MVD1(-40));
}


void Dialog::
pushbotDisconnected()
{
	std::cout << "disconnected" << std::endl;
}





} // nst::
