#include <iostream>
#include <cstdlib>

#include "Datatypes.hpp"
#include "BytestreamParser.hpp"
#include "PushbotConnection.hpp"
#include "Dialog.hpp"

#include <QRect>
#include <QApplication>
#include <QString>
#include <QObject>
#include <QThread>
#include <QTimer>


int
show_dialog(int argc, char *argv[])
{
	using namespace nst;
	QApplication app(argc, argv);

	// create a stupid dialog
	Dialog dialog;
	dialog.setGeometry(QRect(40, 40, 400, 400));
	dialog.show();

	return app.exec();
}

int
main (int argc, char *argv[])
{
	return show_dialog(argc, argv);
}
