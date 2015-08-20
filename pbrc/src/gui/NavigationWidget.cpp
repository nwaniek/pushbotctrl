#include "NavigationWidget.hpp"
#include <cmath>
#include <QRect>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <algorithm>
#include <iostream>
#include "utils.hpp"

namespace nst { namespace gui {


NavigationWidget::
NavigationWidget(int update_interval, QWidget *parent)
: QWidget(parent)
, _update_interval(update_interval)
, _cmd_radius(5)
, _radius(0)
, _center(0, 0)
, _cmd_pos(0, 0)
, _cmd_visual_pos(0, 0)
{
	_timer = new QTimer();
	connect(_timer, &QTimer::timeout, this, &NavigationWidget::timerTimeout);
	_timer->setInterval(_update_interval);
	_timer->start();
}


NavigationWidget::
~NavigationWidget()
{}


void NavigationWidget::
timerTimeout()
{
	emit navigationUpdate(QPointF(_cmd_pos));
}


void NavigationWidget::
resizeEvent(QResizeEvent *ev)
{
	this->_center.setX(ev->size().width() / 2);
	this->_center.setY(ev->size().height() / 2);
	this->_radius = std::min(_center.x(), _center.y()) - this->_cmd_radius;

	cmdToVisualPos();
}


void NavigationWidget::
reset()
{
	_cmd_pos.setX(0);
	_cmd_pos.setY(0);
	cmdToVisualPos();
	emit navigationUpdate(QPointF(_cmd_pos));
	update();
}


void NavigationWidget::
cmdToVisualPos()
{
	_cmd_visual_pos.setX( _cmd_pos.x() * _radius + _center.x());
	_cmd_visual_pos.setY(-_cmd_pos.y() * _radius + _center.y());
}


void NavigationWidget::
mouseMoveEvent(QMouseEvent *ev)
{
	QPointF old_pos(this->_cmd_pos);
	float x = ev->pos().x() - _center.x();
	float y = ev->pos().y() - _center.y();

	float snapdistance = 5.0;
	float norm = sqrtf(x*x + y*y);
	if (norm <= snapdistance) x = y = 0.0;


	if (norm >= _radius) {
		x = floor(x * _radius / norm);
		y = floor(y * _radius / norm);
	}

	_cmd_pos.setX( x / _radius);
	_cmd_pos.setY(-y / _radius);
	cmdToVisualPos();

	if (old_pos != _cmd_pos)
		emit navigationUpdate(QPointF(_cmd_pos));
	this->update();
}


void NavigationWidget::
paintEvent(QPaintEvent * /*ev*/)
{
	// setup painter
	QPainter p;
	p.begin(this);
	p.setRenderHint(QPainter::Antialiasing);

	// white background, black circle
	p.setBrush(Qt::white);
	p.drawEllipse(_center, (int)_radius, (int)_radius);
	p.setBrush(QColor(0, 0, 0, 0));

	// coordinate axis
	QPen pen(QColor(150, 150, 150, 150));
	pen.setStyle(Qt::DashLine);
	p.setPen(pen);
	p.drawLine(_center.x() - 20, _center.y(), _center.x() + 20, _center.y());
	p.drawLine(_center.x(), _center.y() - 20, _center.x(), _center.y() + 20);

	// draw user speed setting
	p.setPen(QColor(255, 0, 0));
	p.drawEllipse(_cmd_visual_pos, _cmd_radius, _cmd_radius);
	p.setPen(Qt::red);

	p.end();
}



}} // nst::gui::
