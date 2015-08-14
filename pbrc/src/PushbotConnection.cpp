#include "PushbotConnection.hpp"
#include "Commands.hpp"
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
connect(const QString ip)
{
	// make sure to call in the correct thread
	if (thread() != QThread::currentThread()) {
		QMetaObject::invokeMethod(this, "connect", Qt::QueuedConnection, Q_ARG(const QString, ip));
		return;
	}

	this->_sock = new QTcpSocket(this);
	QObject::connect(_sock, &QTcpSocket::connected, this, &PushbotConnection::_sock_connected);
	QObject::connect(_sock, &QTcpSocket::disconnected, this, &PushbotConnection::_sock_disconnected);
	QObject::connect(_sock, &QTcpSocket::stateChanged, this, &PushbotConnection::_sock_onStateChanged);
	QObject::connect(_sock, &QTcpSocket::readyRead, this, &PushbotConnection::_sock_readyRead);
	this->_sock->connectToHost(ip, 56000);
}


void PushbotConnection::
disconnect()
{
	// std::cout << "Pushbot: disconnect called" << std::endl;
	if (!_sock) {
		std::cout << "PushbotConnection: No socket" << std::endl;
		return;
	}
	_sock->disconnectFromHost();
	delete this->_sock;
	this->_sock = nullptr;
}


void PushbotConnection::
_sock_readyRead()
{
	if (!_sock) return;
	auto data = _sock->readAll();
	emit dataReady(std::move(data));
}

void PushbotConnection::
_sock_connected()
{
	// std::cout << "PushbotConnecton: connected" << std::endl;
	emit connected();
}

void PushbotConnection::
_sock_disconnected()
{
	// std::cout << "PushbotConnecton: disconnected" << std::endl;
	emit disconnected();
}

void PushbotConnection::
_sock_onStateChanged(QAbstractSocket::SocketState /*state*/)
{
	// std::cout << "state changed: " << state << std::endl;
}


void PushbotConnection::
sendCommand(const commands::Command *cmd)
{
	// little trick to avoid issues with the threading. this essentially
	// moves the command pointer to the thread
	// TODO: this is presumably not thread-safe! if anyone else accesses the
	// command meanwhile, everything will EXPLODE!
	if (thread() != QThread::currentThread()) {
		QMetaObject::invokeMethod(this, "sendCommand", Qt::QueuedConnection, Q_ARG(const commands::Command*, cmd));
		return;
	}

	if (!_sock || !cmd)
		return;
	_sock << *cmd;

	// TODO: shall we delete the command afterwards?
	// TODO: use a smartpointer instead?
	delete cmd;
}



} // nst::
