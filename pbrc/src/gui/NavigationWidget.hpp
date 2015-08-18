#ifndef __NAVIGATIONWIDGET_HPP__58B99C95_EEFF_48D6_84A4_457243568660
#define __NAVIGATIONWIDGET_HPP__58B99C95_EEFF_48D6_84A4_457243568660

#include <QWidget>

namespace nst { namespace gui {

class NavigationWidget : public QWidget
{
	Q_OBJECT
public:
	NavigationWidget(QWidget *parent = 0);
	virtual ~NavigationWidget();

	void resizeEvent(QResizeEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;
	void paintEvent(QPaintEvent *ev) override;

signals:
	void navigationUpdate(const QPointF pos);

private:
	int _cmd_radius;
	float _radius;
	QPoint _center;
	QPointF _cmd_pos;
	QPoint _cmd_visual_pos;

	void cmdToVisualPos();
};

}} // nst::gui::

#endif /* __NAVIGATIONWIDGET_HPP__58B99C95_EEFF_48D6_84A4_457243568660 */

