#include "PushbotConnection.hpp"
#include "Commands.hpp"
#include <QMutexLocker>
#include <QCoreApplication>
#include <iostream>
#include "utils.hpp"

namespace nst {


PushbotConnection::
PushbotConnection(QObject *parent)
: QObject(parent)
{

}

PushbotConnection::
~PushbotConnection()
{ }


void PushbotConnection::
connect(const QString uri, uint16_t port)
{
	// make sure to call in the correct thread
	if (thread() != QThread::currentThread()) {
		QMetaObject::invokeMethod(this, "connect", Qt::QueuedConnection, Q_ARG(const QString, uri), Q_ARG(uint16_t, port));
		return;
	}
	auto uri_lower = uri.toLower();

	// if we already have a socket connection, re-establish the socket first
	if (this->_sock || this->_serial) disconnect();

	// figure out the type of the connection
	this->_ctype = DVS_NETWORK_DEVICE;
	int rate_idx = uri_lower.indexOf("baudrate");
	if (rate_idx > 0) this->_ctype = DVS_SERIAL_DEVICE;

	switch (_ctype) {
	case DVS_NETWORK_DEVICE:
		this->_sock = new QTcpSocket(this);
		QObject::connect(_sock, &QTcpSocket::connected, this, &PushbotConnection::_sock_connected);
		QObject::connect(_sock, &QTcpSocket::disconnected, this, &PushbotConnection::_sock_disconnected);
		QObject::connect(_sock, &QTcpSocket::stateChanged, this, &PushbotConnection::_sock_onStateChanged);
		QObject::connect(_sock, &QTcpSocket::readyRead, this, &PushbotConnection::_sock_readyRead);
		this->_sock->connectToHost(uri, port);
		break;

	case DVS_SERIAL_DEVICE: {
		this->_serial = new QSerialPort(this);
		QString portname = uri.left(rate_idx - 1);
		QString baudratestr = uri.right(uri.length() - portname.length() - 10);
		int baudrate = baudratestr.toInt();
		this->_serial->setPortName(portname);
		this->_serial->setBaudRate(baudrate);

		QObject::connect(_serial, &QSerialPort::readyRead, this, &PushbotConnection::_sock_readyRead);
		QObject::connect(_serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &PushbotConnection::_serial_error);
		if (this->_serial->open(QIODevice::ReadWrite))
			emit connected();
		else {
			// TODO: proper handling
			std::cout << "NO CONNECTION TO SERIAL DEVICE!" << std::endl;
		}
		break;
		}

	case DVS_UNKNOWN_DEVICE:
		break;
	}
}

void PushbotConnection::
flush()
{
	// make sure to call in the correct thread
	if (thread() != QThread::currentThread()) {
		QMetaObject::invokeMethod(this, "flush", Qt::QueuedConnection);
		return;
	}


	switch(_ctype) {
	case DVS_NETWORK_DEVICE:
		if (_sock) _sock->flush();
		break;

	case DVS_SERIAL_DEVICE:
		if (_serial) _serial->flush();
		break;

	case DVS_UNKNOWN_DEVICE:
		break;
	}

}


void PushbotConnection::
disconnect()
{
	if (thread() != QThread::currentThread()) {
		QMetaObject::invokeMethod(this, "disconnect", Qt::QueuedConnection);
		return;
	}
	switch (_ctype) {
	case DVS_NETWORK_DEVICE:
		if (_sock) {
			_sock->disconnectFromHost();
			delete this->_sock;
		}
		break;

	case DVS_SERIAL_DEVICE:
		if (_serial) {
			_serial->close();
			delete this->_serial;
			emit disconnected();
		}
		break;

	case DVS_UNKNOWN_DEVICE:
		break;
	}

	this->_sock = nullptr;
	this->_serial = nullptr;
}


void PushbotConnection::
_sock_readyRead()
{
	switch (_ctype) {
	case DVS_NETWORK_DEVICE:
		if (_sock) {
			auto data = _sock->readAll();
			emit dataReady(std::move(data));
		}
		break;
	case DVS_SERIAL_DEVICE:
		if (_serial) {
			auto data = _serial->readAll();
			emit dataReady(std::move(data));
		}
		break;
	case DVS_UNKNOWN_DEVICE:
		break;
	}
}




void PushbotConnection::
_sock_connected()
{
	emit connected();
}

void PushbotConnection::
_sock_disconnected()
{
	emit disconnected();
}

void PushbotConnection::
_sock_onStateChanged(QAbstractSocket::SocketState /*state*/)
{ }


void PushbotConnection::
_serial_error(QSerialPort::SerialPortError error)
{
	if (error != QSerialPort::NoError) {
		// TODO: better error handling
		std::cerr << "QSerialPort Error: " << error << "\n";
		std::cerr << "Cannot recover, will exit now." << std::endl;
		QCoreApplication::exit(1);
	}
}



void PushbotConnection::
sendCommand(commands::Command *cmd)
{
	// little trick to avoid issues with the threading. this essentially
	// moves the command pointer to the thread
	if (thread() != QThread::currentThread()) {
		QMetaObject::invokeMethod(this, "sendCommand", Qt::QueuedConnection, Q_ARG(commands::Command*, cmd));
		return;
	}

	if (!cmd) return;

	switch (_ctype) {
	case DVS_NETWORK_DEVICE:
		_sock << *cmd;
		break;
	case DVS_SERIAL_DEVICE:
		_serial << *cmd;
		break;
	case DVS_UNKNOWN_DEVICE:
		break;
	}
	delete cmd;
}



} // nst::
