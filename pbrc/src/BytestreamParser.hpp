#ifndef __BYTESTREAMPARSER_HPP__4FA5A548_1B33_4536_8BCA_39DE7D602068
#define __BYTESTREAMPARSER_HPP__4FA5A548_1B33_4536_8BCA_39DE7D602068

#include <QObject>
#include <QString>
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
	BytestreamParser(DVSEvent::timeformat_t fmt = DVSEvent::TIMEFORMAT_0BYTES);
	virtual ~BytestreamParser() {}
	void parse(const char c);

signals:
	void event_received(const DVSEvent *ev);
	void response_received(const QString *str);

private:
	DVSEvent::timeformat_t _timeformat;
	int _state;
	QString *_response;
	DVSEvent *_ev;
};

} // nst::


#endif /* __BYTESTREAMPARSER_HPP__4FA5A548_1B33_4536_8BCA_39DE7D602068 */

