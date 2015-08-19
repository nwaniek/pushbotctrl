#ifndef __BYTESTREAMPARSER_HPP__4FA5A548_1B33_4536_8BCA_39DE7D602068
#define __BYTESTREAMPARSER_HPP__4FA5A548_1B33_4536_8BCA_39DE7D602068

#include <QObject>
#include <QString>
#include <QByteArray>
#include "Datatypes.hpp"

// TODO: smart pointers for the response string and events?

namespace nst {

/**
 * state machine to parse a bytestream. it will eventually emit an event when
 * new data has arrived, i.e. either a command response or an event.
 */
class BytestreamParser : public QObject
{
	Q_OBJECT

public:
	BytestreamParser(const uint8_t id, DVSEvent::timeformat_t fmt = DVSEvent::TIMEFORMAT_0BYTES);
	virtual ~BytestreamParser();
	void set_timeformat(DVSEvent::timeformat_t fmt);
	uint8_t id() const;

public slots:
	void parseData(const QByteArray &data);

signals:
	void eventReceived(const DVSEvent *ev);
	void responseReceived(const QString *str);

private:
	void parse(const unsigned char c);

	const uint8_t _id;
	DVSEvent::timeformat_t _timeformat;
	int _state;
	QString *_response = nullptr;
	DVSEvent *_ev = nullptr;
};

} // nst::


#endif /* __BYTESTREAMPARSER_HPP__4FA5A548_1B33_4536_8BCA_39DE7D602068 */

