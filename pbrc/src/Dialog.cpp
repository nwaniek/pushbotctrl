#include "Dialog.hpp"
#include <QPushButton>
#include <QGridLayout>
#include <QThread>
#include <iostream>

#include "PushbotConnection.hpp"

namespace nst {

Dialog::
Dialog(QWidget *parent) : QDialog(parent)
{
	buttonCreate = new QPushButton("Create PushbotConnection");
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


	// start the thread with all th
	_con_thread->start();
}

}
