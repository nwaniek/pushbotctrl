#include "BytestreamParser.hpp"
#include <iostream>
#include <cstdlib>

namespace nst {

BytestreamParser:: BytestreamParser(DVSEvent::timeformat_t fmt)
: QObject(), _timeformat(fmt), _state(0), _response(nullptr), _ev(nullptr)
{
}

void BytestreamParser::
parse(const unsigned char c)
{
	switch (_state) {
	// there may be either a regular character from a response string, or an
	// event starting. We can determine this information by looking at the
	// high bit. If it is set, it is an event
	case 0:
		if ((c & 0x80) == 0) {
			if (c == '\n') {
				emit responseReceived(std::move(_response));
				_response = nullptr;
			}
			else {
				if (!_response) {
					// create a new string and reserve some
					// space to speed up appending of items
					_response = new QString();
					_response->reserve(64);
				}
				_response->append(c);
			}
		}
		else {
			_ev = new DVSEvent;
			_ev->x = static_cast<uint16_t>(c) & 0x7F;
			_ev->y = 0;
			_ev->p = 0;
			_ev->t = 0;
			++_state;
		}
		break;

	case 1:
		_ev->p = (static_cast<uint8_t>(c) & 0x80) >> 7;
		_ev->y = static_cast<uint16_t>(c) & 0x7F;
		if (_timeformat == DVSEvent::TIMEFORMAT_0BYTES) {
			emit eventReceived(std::move(_ev));
			_state = 0;
		}
		else
			++_state;
		break;

	case 2:
		if (_timeformat == DVSEvent::TIMEFORMAT_2BYTES)
			_ev->t |= static_cast<uint64_t>(c) << 8;
		else
			_ev->t |= static_cast<uint64_t>(c) << 16;
		++_state;
		break;

	case 3:
		if (_timeformat == DVSEvent::TIMEFORMAT_2BYTES) {
			_ev->t |= static_cast<uint64_t>(c);
			emit eventReceived(std::move(_ev));
			_state = 0;
		}
		else {
			_ev->t |= static_cast<uint64_t>(c) << 8;
			++_state;
		}
		break;

	case 4:
		_ev->t |= static_cast<uint64_t>(c);
		emit eventReceived(std::move(_ev));
		_state = 0;
		break;

	default:
		// TODO: report the error to some other function, don't just
		// print it
		// found a state that should not be there. exit
		std::cerr << "EE: BytestreamParser in unknown state" << std::endl;
		break;
	}

}


void BytestreamParser::
parseData(const QByteArray &data)
{
	for (const char c: data)
		this->parse(static_cast<unsigned char>(c));
}


void BytestreamParser::
set_timeformat(DVSEvent::timeformat_t fmt)
{
	// TODO: Check if we are outside state0 -> if so, this may introduce
	// errors
	_timeformat = fmt;
}


} // nst::
