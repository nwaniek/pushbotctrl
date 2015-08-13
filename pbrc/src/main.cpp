#include <iostream>
#include <cstdlib>

#include "Datatypes.hpp"
#include "BytestreamParser.hpp"
#include "PushbotConnection.hpp"
#include "Dialog.hpp"

#include <QApplication>
#include <QString>
#include <QObject>
#include <QThread>
#include <QTimer>


/* layout of the application: have a worker thread that parses the bytestream
 * coming from the pushbot. the worker thread emits events and string responses.
 * string responses may need to be processed further to extract gyro, accelero,
 * magneto data.
 */

void handle_response(const QString *str) {
	std::string s = qPrintable(*str);
	std::cout << s << std::endl;

	delete str;
}

void handle_event(const DVSEvent *ev) {
	std::cout << "ev ("
		<< ev->t
		<< ", "
		<< ev->p
		<< ", "
		<< ev->x
		<< ", "
		<< ev->y
		<< ")" << std::endl;
	delete ev;
}


void parser_test()
{
	using namespace nst;

	// worker object
	BytestreamParser parser;

	QObject::connect(&parser, &BytestreamParser::event_received, handle_event);
	QObject::connect(&parser, &BytestreamParser::response_received, handle_response);

	// test string
	std::string test("This is\nA simple\nTest\n");
	for (const char c : test) {
		parser.parse(c);
	}
}

void
handle_socket_error(int socket_error, const QString &msg)
{
	std::cout << "Socket Error " << socket_error << ": " << msg.toStdString() << std::endl;
}

void
handle_data_received(const QByteArray &) {

}


int
connection_test(int argc, char *argv[])
{
	using namespace nst;
	QApplication app(argc, argv);

	// create a stupid dialog
	Dialog dialog;
	dialog.show();

	return app.exec();
}

int
main (int argc, char *argv[])
{
	return connection_test(argc, argv);
}
