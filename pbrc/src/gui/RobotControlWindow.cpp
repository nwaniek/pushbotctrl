#include "gui/RobotControlWindow.hpp"

#include <iostream>

#include <QCloseEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QPushButton>
#include <QCheckBox>
#include <QMdiArea>
#include <QMoveEvent>

#include "utils.hpp"
#include "RobotControl.hpp"
#include "Commands.hpp"
#include "UserFunction.hpp"
#include "gui/NavigationWidget.hpp"
#include "gui/EventVisualizerWindow.hpp"

namespace nst { namespace gui {


RobotControlWindow::
RobotControlWindow(QWidget *parent, Qt::WindowFlags flags)
: QMdiSubWindow(parent, flags)
{
	// TODO: just a check here
	std::cout << LENGTH(user_functions) << " user functions found" << std::endl;

	// create control and hook into signals
	_control = std::make_shared<RobotControl>();
	connect(_control.get(), &RobotControl::connected, this, &RobotControlWindow::onControlConnected);
	connect(_control.get(), &RobotControl::disconnected, this, &RobotControlWindow::onControlDisconnected);

	// create GUI
	_centralWidget = new QFrame(this);
	layout()->addWidget(_centralWidget);

	QGridLayout *layout = new QGridLayout();

	layout->addWidget(new QLabel("IP: ", _centralWidget));

	// connectivity
	_edtIP = new QLineEdit("10.162.242.xxx", _centralWidget);
	layout->addWidget(_edtIP, 0, 1);

	_btnConnect = new QPushButton("connect", _centralWidget);
	layout->addWidget(_btnConnect, 0, 2);
	connect(_btnConnect, &QPushButton::clicked, this, &RobotControlWindow::onBtnConnectClicked);

	// navigation widget
	_wdgtNav = new NavigationWidget(_centralWidget);
	layout->addWidget(_wdgtNav, 1, 0, 1, 3);
	connect(_wdgtNav, &NavigationWidget::navigationUpdate, this, &RobotControlWindow::onNavigationUpdate);

	// options
	_cbShowEvents = new QCheckBox("visualize DVS events", _centralWidget);
	_cbShowEvents->setCheckState(Qt::Checked);
	connect(_cbShowEvents, &QCheckBox::stateChanged, this, &RobotControlWindow::onShowEventsStateChanged);
	layout->addWidget(_cbShowEvents, 2, 0, 1, 3);

	_cbMagnetWindows = new QCheckBox("magnetic windows", _centralWidget);
	_cbMagnetWindows->setCheckState(Qt::Checked);
	layout->addWidget(_cbMagnetWindows, 3, 0, 1, 3);

	this->setWindowTitle("Robot Control " + QString::number(_control->id()));

	_centralWidget->setLayout(layout);
	setAttribute(Qt::WA_DeleteOnClose);
}


RobotControlWindow::
~RobotControlWindow()
{
	// close all related windows. do not delete, they have the DeleteOnClose
	// flag set so will tidy up themselves
	if (_winEventVisualizer) {
		_winEventVisualizer->close();
		_winEventVisualizer = nullptr;
	}

	// debug output
	std::cout << "~RobotControlWindow" << std::endl;
}


void RobotControlWindow::
closeEvent(QCloseEvent *ev)
{
	emit closing(this);
	QMdiSubWindow::closeEvent(ev);
}


void RobotControlWindow::
onBtnConnectClicked()
{
	if (!_control->isConnected())
		_control->connectRobot(_edtIP->text());
	else
		_control->disconnectRobot();
}


void RobotControlWindow::
openEventVisualizerWindow()
{
	// create event visualizer and move relative to control window
	const QRect geom = frameGeometry();

	_winEventVisualizer = new EventVisualizerWindow(_control, mdiArea());
	_winEventVisualizer->resize(250, 250);
	_winEventVisualizer->show();
	_winEventVisualizer->move(geom.topRight().x(), geom.topRight().y());
	_winEventVisualizer->setWindowTitle("Events: " + _edtIP->text());

	connect(_winEventVisualizer, &EventVisualizerWindow::closing, this, &RobotControlWindow::onEventVisualizerClosing);
}


void RobotControlWindow::
closeEventVisualizerWindow()
{
	if (_winEventVisualizer) _winEventVisualizer->close();
	_winEventVisualizer = nullptr;
}


void RobotControlWindow::
onControlConnected()
{
	_btnConnect->setText("disconnect");
	_edtIP->setReadOnly(true);
	_edtIP->setEnabled(false);
	std::cout << "control connected" << std::endl;

	// set up an event visualizer
	// TODO: check the checkbox if we really need to
	if (_winEventVisualizer) {
		std::cout << "RobotControlWindow: something went wrong, _winEventVisualizer not nullptr" << std::endl;
		_winEventVisualizer->close();
	}
	if (_cbShowEvents->checkState() == Qt::Checked)
		openEventVisualizerWindow();


	// TODO: make GUI component to change user function. just set the 0 user
	// function here for demo purposes
	_control->setUserFunction(&user_functions[0]);
}


void RobotControlWindow::
onControlDisconnected()
{
	closeEventVisualizerWindow();
	_edtIP->setReadOnly(false);
	_edtIP->setEnabled(true);
	_btnConnect->setText("connect");
	std::cout << "control disconnected" << std::endl;
}


void RobotControlWindow::
onNavigationUpdate(const QPointF pos)
{
	if (!_control->isConnected()) return;
	_control->drive(pos.x(), pos.y());
}


void RobotControlWindow::
onShowEventsStateChanged(int state)
{
	if (!_control->isConnected()) return;

	if (state == Qt::Checked)
		openEventVisualizerWindow();
	else
		closeEventVisualizerWindow();
}


void RobotControlWindow::
onEventVisualizerClosing()
{
	// remove the pointer to the window to make sure that we do not
	// accidentally access it -> segfault
	_winEventVisualizer = nullptr;
}


void RobotControlWindow::
moveEvent(QMoveEvent *ev)
{
	// if we have magnet windows, move all 'sub' windows, too
	if (_cbMagnetWindows->checkState() == Qt::Checked) {
		int dx = ev->pos().x() - ev->oldPos().x();
		int dy = ev->pos().y() - ev->oldPos().y();

		if (_winEventVisualizer) {
			int x = _winEventVisualizer->pos().x();
			int y = _winEventVisualizer->pos().y();
			_winEventVisualizer->move(x + dx, y + dy);
		}
	}
	ev->accept();
}



}} // nst::gui
