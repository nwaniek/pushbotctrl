#include "gui/MainWindow.hpp"

#include <iostream>

#include <QMenuBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QApplication>
#include <QToolBar>

#include "utils.hpp"
#include "RCManager.hpp"
#include "gui/RobotControlWindow.hpp"
#include "gui/EventVisualizerWindow.hpp"

namespace nst { namespace gui {

MainWindow::
MainWindow(QWidget *parent)
: QMainWindow(parent)
{
	// initialize UI
	_mnuFile = menuBar()->addMenu("File");

	// build actions
	QIcon stopIcon = QIcon::fromTheme("process-stop");
	_actEmergencyShutdown = new QAction("Emergency Shutdown", this);
	_actEmergencyShutdown->setIcon(stopIcon);

	_actAddRobotControl = new QAction("Add Robot Control", this);
	_actClose = new QAction("Quit", this);

	connect(_actEmergencyShutdown, &QAction::triggered, this, &MainWindow::onEmergencyShutdown);
	connect(_actAddRobotControl, &QAction::triggered, this, &MainWindow::addRobotControl);
	connect(_actClose, &QAction::triggered, this, &QApplication::quit);

	// build toolbar
	_toolbar = addToolBar("Robot Control Toolbar");
	_toolbar->addAction(_actEmergencyShutdown);

	// build main menu
	_mnuFile->addAction(_actAddRobotControl);
	_mnuFile->addSeparator();
	_mnuFile->addAction(_actClose);


	_mdi = new QMdiArea(this);
	setCentralWidget(_mdi);
	// always start with at least one robot control window open
	addRobotControl();

	statusBar();
}


MainWindow::
~MainWindow()
{
	// TODO: check if this causes a segfault!
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


void MainWindow::
addRobotControl()
{
	auto rc = new RobotControlWindow(_mdi);
	rc->resize(250, 350);
	connect(rc, &RobotControlWindow::closing, this, &MainWindow::onSubwindowClosing);
	_wins.push_back(rc);
	rc->show();
}


void MainWindow::
onEmergencyShutdown()
{
	rcman_emergency_shutdown();
}


}} // nst::gui::
