#include "EventVisualizerWindow.hpp"

#include <iostream>

#include <QFrame>
#include <QLayout>
#include <QSizePolicy>
#include <QResizeEvent>
#include <QCloseEvent>

#include "utils.hpp"
#include "Datatypes.hpp"
#include "RobotControl.hpp"
#include "gui/DVSEventWidget.hpp"


namespace nst { namespace gui {


EventVisualizerWindow::
EventVisualizerWindow(RobotControl *control, QWidget *parent, Qt::WindowFlags flags)
: QMdiSubWindow(parent, flags), _control(control)
{
	_wdgtEvents = new DVSEventWidget(this);
	layout()->addWidget(_wdgtEvents);

	// connect the visualizer to the robotcontrol
	connect(_control, &RobotControl::DVSEventReceived, _wdgtEvents, &DVSEventWidget::newEvent);

	setAttribute(Qt::WA_DeleteOnClose);
}


EventVisualizerWindow::
~EventVisualizerWindow()
{
	disconnect(_control, &RobotControl::DVSEventReceived, _wdgtEvents, &DVSEventWidget::newEvent);
}


void EventVisualizerWindow::
resizeEvent(QResizeEvent *ev)
{
	// make sure to keep a fixed/square aspect when resizing
	int max = ev->size().width();
	if (ev->size().height() > max) max = ev->size().height();

	resize(max, max);
	QMdiSubWindow::resizeEvent(ev);
}


void EventVisualizerWindow::
closeEvent(QCloseEvent *ev)
{
	emit closing();
	ev->accept();
}


}} // nst::gui::
