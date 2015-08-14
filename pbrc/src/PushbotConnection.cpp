#include "PushbotConnection.hpp"
#include <QMutexLocker>
#include <iostream>

namespace nst {

PushbotConnection::
PushbotConnection(QObject *parent)
: QObject(parent)
{

}

PushbotConnection::
~PushbotConnection()
{
	std::cout << "~PushbotConnection" << std::endl;
}


void PushbotConnection::
connect()
{
	this->_sock = new QTcpSocket(this);
	QObject::connect(_sock, &QTcpSocket::connected, this, &PushbotConnection::connected);
	QObject::connect(_sock, &QTcpSocket::disconnected, this, &PushbotConnection::disconnected);
	QObject::connect(_sock, &QTcpSocket::stateChanged, this, &PushbotConnection::onStateChanged);
	QObject::connect(_sock, &QTcpSocket::readyRead, this, &PushbotConnection::readyRead);
	this->_sock->connectToHost("10.162.177.42", 56000);
}


void PushbotConnection::
disconnect()
{
	std::cout << "Pushbot: disconnect called" << std::endl;
	if (!_sock) {
		std::cout << "PushbotConnection: No socket" << std::endl;
		return;
	}
	_sock->disconnectFromHost();
	delete this->_sock;
	this->_sock = nullptr;
}


void PushbotConnection::
readyRead()
{
	if (!_sock) return;
	auto data = _sock->readAll();
	emit dataReady(std::move(data));
}

void PushbotConnection::
connected()
{
	std::cout << "PushbotConnecton: connected" << std::endl;
	_sock->write("!M+\n");
	_sock->write("!MD0=40\n");
	_sock->write("E+\n");
}

void PushbotConnection::
disconnected()
{
	std::cout << "PushbotConnecton: disconnected" << std::endl;
}

void PushbotConnection::
onStateChanged(QAbstractSocket::SocketState state)
{
	std::cout << "state changed: " << state << std::endl;
}



} // nst::
