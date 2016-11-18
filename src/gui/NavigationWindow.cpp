#include "NavigationWindow.hpp"

#include <iostream>

#include <QFrame>
#include <QLayout>
#include <QSizePolicy>
#include <QResizeEvent>
#include <QCloseEvent>

#include "utils.hpp"
#include "Datatypes.hpp"
#include "RobotControl.hpp"
#include "gui/NavigationWidget.hpp"


namespace nst { namespace gui {


NavigationWindow::
NavigationWindow(RobotControl *control, QWidget *parent, Qt::WindowFlags flags)
: QMdiSubWindow(parent, flags), _control(control)
{
	_wdgtNav = new NavigationWidget(20, this);
	layout()->addWidget(_wdgtNav);

	// connect the visualizer to the robotcontrol
	connect(_wdgtNav, &NavigationWidget::navigationUpdate, this, &NavigationWindow::onNavigationUpdate);
	connect(_control, &RobotControl::disconnected, _wdgtNav, &NavigationWidget::reset);

	setAttribute(Qt::WA_DeleteOnClose);
}


NavigationWindow::
~NavigationWindow()
{
	disconnect(_control, &RobotControl::disconnected, _wdgtNav, &NavigationWidget::reset);
}


void NavigationWindow::
resizeEvent(QResizeEvent *ev)
{
	// make sure to keep a fixed/square aspect when resizing
	int max = ev->size().width();
	if (ev->size().height() > max) max = ev->size().height();

	resize(max, max);
	QMdiSubWindow::resizeEvent(ev);
}


void NavigationWindow::
closeEvent(QCloseEvent *ev)
{
	emit closing();
	ev->accept();
}


void NavigationWindow::
onNavigationUpdate(const QPointF pos)
{
	if (!_control->isConnected()) return;

	// progressive increase of speed -> simply take the square
	if (_linear_control_mode)
		_control->drive(pos.x(), pos.y());
	else
		_control->drive(SGNF(pos.x()) * pos.x() * pos.x(), SGNF(pos.y()) * pos.y() * pos.y());

	// std::cout << pos.x() << ", " << pos.y() << std::endl;
}


}} // nst::gui::
