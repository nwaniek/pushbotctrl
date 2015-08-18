#include "gui/MainWindow.hpp"

#include <iostream>

#include <QMenuBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "gui/RobotControlWidget.hpp"

namespace nst { namespace gui {

MainWindow::
MainWindow(QWidget *parent)
: QMainWindow(parent)
{
	// initialize UI
	menuBar()->addMenu("File");

	_mdi = new QMdiArea(this);
	setCentralWidget(_mdi);

	// fill with some stuff
	auto rc = new RobotControlWidget(_mdi);
	rc->resize(250, 250);
	rc->setWindowTitle("Robot Control");
	connect(rc, &RobotControlWidget::closing, this, &MainWindow::onSubwindowClosing);
	_wins.push_back(rc);

	statusBar();
}


MainWindow::
~MainWindow()
{
	// we don't access them anymore, so get rid of them
	for (auto *win: _wins) delete win;
}


void MainWindow::
onSubwindowClosing(QMdiSubWindow *win)
{
	// remove. due to "DeleteOnClose" in all our windows the pointer is
	// invalid afterwards.
	_wins.erase(std::remove(_wins.begin(), _wins.end(), win), _wins.end());
}



}} // nst::gui::
