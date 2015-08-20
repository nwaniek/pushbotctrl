#ifndef __NAVIGATIONWIDGET_HPP__58B99C95_EEFF_48D6_84A4_457243568660
#define __NAVIGATIONWIDGET_HPP__58B99C95_EEFF_48D6_84A4_457243568660

#include <QWidget>

class QTimer;

namespace nst { namespace gui {

class NavigationWidget : public QWidget
{
	Q_OBJECT
public:
	/*
	 * create a new navigation widget. the update interval in ms will
	 * tell how often navigationUpdate is called (if no user update was
	 * there). this is necessary for the decay commands to work properly.
	 */
	NavigationWidget(int update_interval = 20, QWidget *parent = 0);
	virtual ~NavigationWidget();

	void resizeEvent(QResizeEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;
	void paintEvent(QPaintEvent *ev) override;

signals:
	void navigationUpdate(const QPointF pos);

public slots:
	void reset();

private slots:
	void timerTimeout();

private:
	int _update_interval;
	int _cmd_radius;
	float _radius;
	QPoint _center;
	QPointF _cmd_pos;
	QPoint _cmd_visual_pos;

	QTimer *_timer;

	void cmdToVisualPos();
};

}} // nst::gui::

#endif /* __NAVIGATIONWIDGET_HPP__58B99C95_EEFF_48D6_84A4_457243568660 */

