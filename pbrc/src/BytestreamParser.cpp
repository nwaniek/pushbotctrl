#include "BytestreamParser.hpp"
#include <iostream>
#include <cstdlib>
#include "utils.hpp"

namespace nst {

BytestreamParser:: BytestreamParser(uint8_t id, DVSEvent::timeformat_t fmt)
: QObject(), _id(id), _timeformat(fmt), _state(0), _response(new QString()), _ev(nullptr)
{
	_response->reserve(64);
}

BytestreamParser::
~BytestreamParser()
{
	// std::cout << "~BytestreamParser" << std::endl;
}


uint8_t BytestreamParser::
id() const
{
	return _id;
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
			_response->append(c);
			if (c == '\n') {
				if (!_response)
					std::cerr << "EE: BytestreamParser _response is nullptr where it should not be" << std::endl;
				else
					emit responseReceived(std::move(_response));

				_response = new QString();
				_response->reserve(64);
			}
			else
				_response->append(c);
		}
		else {
			_ev = new DVSEvent;
			_ev->id = _id;
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
			_ev = nullptr;
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
			_ev = nullptr;
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
		_ev = nullptr;
		_state = 0;
		break;

	default:
		// TODO: report the error to some other function, don't just
		// print it
		// found a state that should not be there. exit
		std::cerr << "EE: BytestreamParser in unknown state. resetting" << std::endl;
		_state = 0;
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
	_timeformat = fmt;
}


} // nst::
