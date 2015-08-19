#include "gui/RobotControlWindow.hpp"

#include <iostream>

#include <QCloseEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QPushButton>
#include <QCheckBox>

#include "RobotControl.hpp"
#include "Commands.hpp"
#include "gui/NavigationWidget.hpp"

namespace nst { namespace gui {


RobotControlWindow::
RobotControlWindow(QWidget *parent, Qt::WindowFlags flags)
: QMdiSubWindow(parent, flags)
{
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
	_edtIP = new QLineEdit("10.162.177.xxx", _centralWidget);
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
	layout->addWidget(_cbShowEvents, 2, 0, 1, 3);

	_centralWidget->setLayout(layout);

	setAttribute(Qt::WA_DeleteOnClose);
}


RobotControlWindow::
~RobotControlWindow()
{
	// TODO: segfault here! wait for all threads to finish
	delete _control;

	// TODO: delete all other objects!
	std::cout << "destroying" << std::endl;
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
onControlConnected()
{
	_btnConnect->setText("disconnect");
	std::cout << "control connected" << std::endl;
}


void RobotControlWindow::
onControlDisconnected()
{
	_btnConnect->setText("connect");
	std::cout << "control disconnected" << std::endl;
}

#define sgnf(a) (((a) < 0.0) ? -1.0 : 1.0)

void RobotControlWindow::
onNavigationUpdate(const QPointF pos)
{
	if (!_control->isConnected()) return;
	// translate position from ([-1,1], [-1,1]) to a motor command

	// compute speeds
	float max_speed = 100;
	float norm = sqrt(pos.x() * pos.x() + pos.y() * pos.y());
	float m0speed = max_speed * norm * sgnf(pos.y());
	float m1speed = max_speed * norm * sgnf(pos.y());

	// compute angle
	float angle = atan2(pos.y(), pos.x());
	float aangle = fabsf(angle);

	float m0mul = 1.0f;
	float m1mul = 1.0f;

	if (aangle < M_PI / 2.0)
		m1mul = aangle / (M_PI / 2.0);
	else
		m0mul = (M_PI - aangle) / (M_PI / 2.0);

	m0speed *= m0mul;
	m1speed *= m1mul;

	_control->sendCommand(new commands::MV0(static_cast<int>(m0speed)));
	_control->sendCommand(new commands::MV1(static_cast<int>(m1speed)));
}



}} // nst::gui
