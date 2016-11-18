#include <iostream>
#include <QApplication>
#include <cstdint>
#include "gui/MainWindow.hpp"
#include "Commands.hpp"
#include "utils.hpp"

int
main (int argc, char *argv[])
{
	using namespace nst;

	// register the command infrastructure. As we pass along only pointers,
	// use the base class here.
	qRegisterMetaType<uint16_t>("uint16_t");
	qRegisterMetaType<commands::Command*>("commands::Command*");
	qRegisterMetaType<const commands::Command*>("const commands::Command*");

	// prepare_robot_ids();
	QApplication app(argc, argv);
	gui::MainWindow win;
	win.show();

	return app.exec();
}
