#include <iostream>
#include <QApplication>
#include "gui/MainWindow.hpp"
#include "Commands.hpp"

int
main (int argc, char *argv[])
{
	using namespace nst;

	// register the command infrastructure. As we pass along only pointers,
	// use the base class here.
	qRegisterMetaType<const commands::Command*>("const commands::Command*");

	QApplication app(argc, argv);
	gui::MainWindow win;
	win.show();

	return app.exec();
}
