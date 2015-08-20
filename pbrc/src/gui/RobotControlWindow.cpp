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
#include <QComboBox>

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
	_control = new RobotControl();
	connect(_control, &RobotControl::connected, this, &RobotControlWindow::onControlConnected);
	connect(_control, &RobotControl::disconnected, this, &RobotControlWindow::onControlDisconnected);

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
	_wdgtNav = new NavigationWidget(20, _centralWidget);
	layout->addWidget(_wdgtNav, 1, 0, 1, 3);
	connect(_wdgtNav, &NavigationWidget::navigationUpdate, this, &RobotControlWindow::onNavigationUpdate);
	connect(_control, &RobotControl::disconnected, _wdgtNav, &NavigationWidget::reset);


	// visualize events?
	_cbShowEvents = new QCheckBox("visualize DVS events", _centralWidget);
	_cbShowEvents->setCheckState(Qt::Checked);
	layout->addWidget(_cbShowEvents, 2, 0, 1, 3);
	connect(_cbShowEvents, &QCheckBox::stateChanged, this, &RobotControlWindow::onShowEventsStateChanged);

	// magnetic windows?
	_cbMagnetWindows = new QCheckBox("magnetic windows", _centralWidget);
	_cbMagnetWindows->setCheckState(Qt::Checked);
	layout->addWidget(_cbMagnetWindows, 3, 0, 1, 3);

	// user function
	_cbUserFunction = new QCheckBox("", _centralWidget);
	_cbUserFunction->setCheckState(Qt::Unchecked);
	layout->addWidget(_cbUserFunction, 4, 0, 1, 1);
	connect(_cbUserFunction, &QCheckBox::stateChanged, this, &RobotControlWindow::onCbUserFunctionStateChanged);

	_cmbUserFunction = new QComboBox(_centralWidget);
	for (size_t i = 0; i < LENGTH(user_functions); i++)
		_cmbUserFunction->addItem(user_functions[i].name);
	_cmbUserFunction->setEnabled(false);
	layout->addWidget(_cmbUserFunction, 4, 1, 1, 2);
	void(QComboBox::*cmbsignal)(int) = &QComboBox::currentIndexChanged;
	connect(_cmbUserFunction, cmbsignal, this, &RobotControlWindow::onCmbUserFunctionIndexChanged);

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

	// set a user function
	if (_cbUserFunction->checkState() == Qt::Checked) {
		int index = _cmbUserFunction->currentIndex();
		if (index >= 0)
			setUserFunction(static_cast<unsigned>(index));
		else
			unsetUserFunction();
	}
	else
		unsetUserFunction();
}


void RobotControlWindow::
onControlDisconnected()
{
	// remember the old state of the cb
	auto old = _cbShowEvents->checkState();
	closeEventVisualizerWindow();
	_cbShowEvents->setCheckState(old);
	_edtIP->setReadOnly(false);
	_edtIP->setEnabled(true);
	_btnConnect->setText("connect");
	std::cout << "control disconnected" << std::endl;
}


void RobotControlWindow::
onNavigationUpdate(const QPointF pos)
{
	if (!_control->isConnected()) return;

	// progressive increase of speed -> simply take the square
	if (_linear_control_mode)
		_control->drive(pos.x(), pos.y());
	else
		_control->drive(SGNF(pos.x()) * pos.x() * pos.x(), SGNF(pos.y()) * pos.y() * pos.y());
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
	_cbShowEvents->setCheckState(Qt::Unchecked);
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


void RobotControlWindow::
onCbUserFunctionStateChanged(int state)
{
	if (state == Qt::Checked) {
		_cmbUserFunction->setEnabled(true);
		setUserFunction(_cmbUserFunction->currentIndex());
	}
	else {
		_cmbUserFunction->setEnabled(false);
		unsetUserFunction();
	}
}


void RobotControlWindow::
onCmbUserFunctionIndexChanged(int index)
{
	// check the value. if no item has been selected we want to unset the
	// user function
	if (index < 0)
		unsetUserFunction();
	else
		setUserFunction(static_cast<unsigned>(index));
}


void RobotControlWindow::
setUserFunction(unsigned index)
{
	if (index < LENGTH(user_functions))
		_control->setUserFunction(&user_functions[index]);
	else
		std::cerr << "Invalid User Function ID " << index << std::endl;
}

void RobotControlWindow::
unsetUserFunction()
{
	_control->unsetUserFunction();
}


}} // nst::gui
