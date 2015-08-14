#ifndef __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569
#define __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569

#include <QMutex>
#include <QThread>
#include <QTcpSocket>
#include <QMetaObject>
#include <iostream>

namespace nst {

// forward declarations
namespace commands {
	struct Command;
} // commands::


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
	void connect();
	void disconnect();
	void sendCommand(const commands::Command *cmd);

private slots:
	void _sock_readyRead();
	void _sock_connected();
	void _sock_disconnected();
	void _sock_onStateChanged(QAbstractSocket::SocketState state);

private:
	QTcpSocket *_sock = nullptr;
};


} // nst::



#endif /* __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569 */

