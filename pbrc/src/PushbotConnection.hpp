#ifndef __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569
#define __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569

#include <QMutex>
#include <QThread>
#include <QTcpSocket>

namespace nst {

class PushbotConnection : public QObject
{
	Q_OBJECT

public:
	PushbotConnection(QObject *parent = 0);
	virtual ~PushbotConnection();

signals:
	void dataReady(const QByteArray &data);

public slots:
	void connect();
	void disconnect();



private slots:
	void readyRead();
	void connected();
	void disconnected();
	void onStateChanged(QAbstractSocket::SocketState state);

private:
	QTcpSocket *_sock = nullptr;
};



} // nst::

#endif /* __PUSHBOTCONNECTION_HPP__3E71037A_2D7F_456C_B919_BF491C99A569 */

