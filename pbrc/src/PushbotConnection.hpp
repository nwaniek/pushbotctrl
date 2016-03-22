#ifndef __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569
#define __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569

#include <iostream>
#include <memory>
#include <QMutex>
#include <QThread>
#include <QTcpSocket>
#include <QMetaObject>
#include <QString>
#include <QSerialPort>

namespace nst {

// forward declarations
namespace commands {
	struct Command;
} // commands::


/*
 * Pushbots are a DVS hosted on a robot platform.
 */
typedef enum {
	DVS_NETWORK_DEVICE, // network socket connection
	DVS_SERIAL_DEVICE, // serial port connection
	DVS_UNKNOWN_DEVICE
} CONNECTION_TYPE;


class PushbotConnection : public QObject
{
	Q_OBJECT

public:
	PushbotConnection(QObject *parent = 0);
	virtual ~PushbotConnection();

signals:
	void dataReady(const QByteArray &data);
	void connected();
	void disconnected();

public slots:
	void connect(const QString uri, uint16_t port = 56000);
	void disconnect();

	/**
	 * send a command to the PushBot using the ethernet connection. this
	 * will delete the cmd afterwards!
	 */
	void sendCommand(commands::Command *cmd);
	void flush();

private slots:
	void _sock_readyRead();
	void _sock_connected();
	void _sock_disconnected();
	void _sock_onStateChanged(QAbstractSocket::SocketState state);

	void _serial_error(QSerialPort::SerialPortError error);

private:
	QTcpSocket *_sock = nullptr;
	QSerialPort *_serial = nullptr;
	CONNECTION_TYPE _ctype = DVS_UNKNOWN_DEVICE;
};


} // nst::



#endif /* __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569 */

