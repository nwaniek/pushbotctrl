#include "Dialog.hpp"
#include <QPushButton>
#include <QGridLayout>
#include <QThread>
#include <iostream>

#include "Datatypes.hpp"
#include "BytestreamParser.hpp"
#include "PushbotConnection.hpp"

namespace nst {

Dialog::
Dialog(QWidget *parent) : QDialog(parent)
{
	buttonCreate = new QPushButton("Create Threads");
	buttonConnect = new QPushButton("Connect");
	buttonDisconnect = new QPushButton("Disconnect");

	connect(buttonCreate, &QPushButton::clicked, this, &Dialog::btnCreateClicked);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(buttonCreate, 0, 0);
	layout->addWidget(buttonConnect, 1, 0);
	layout->addWidget(buttonDisconnect, 2, 0);
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
	connect(buttonConnect, &QPushButton::clicked, _con, &PushbotConnection::connect, Qt::QueuedConnection);
	connect(buttonDisconnect, &QPushButton::clicked, _con, &PushbotConnection::disconnect, Qt::QueuedConnection);

	// create the parser thread
	_parser_thread = new QThread();
	_parser = new BytestreamParser();
	_parser->moveToThread(_parser_thread);

	// connect the two worker objects
	connect(_con, &PushbotConnection::dataReady, _parser, &BytestreamParser::parseData, Qt::QueuedConnection);

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
	delete ev;
}


void Dialog::
onResponseReceived(const QString *str)
{
	std::cout << str->toStdString() << std::endl;
	delete str;
}


}
