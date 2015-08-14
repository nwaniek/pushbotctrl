#ifndef __DVSEVENTWIDGET_HPP__B36E956F_AC52_4740_9A93_3C82F0E75D71
#define __DVSEVENTWIDGET_HPP__B36E956F_AC52_4740_9A93_3C82F0E75D71

#include <QTimer>
#include <QWidget>
#include <QPaintEvent>
#include <QImage>

namespace nst {

// forward declarations
struct DVSEvent;


/**
 * DVSEventWidget - draw events received from a DVS.
 */
class DVSEventWidget : public QWidget
{
	Q_OBJECT

public:
	explicit DVSEventWidget(QWidget *parent=nullptr);
	~DVSEventWidget();

public slots:
	void paintEvent(QPaintEvent *event);
	void decayImage();
	void newEvent(const DVSEvent *ev);
	void setDecayFactor(float decay_factor);

private:
	QTimer  _timer;
	QImage *_image;
	float   _decay_factor;

	void decayPixel(unsigned *p);
};


} // nst::

#endif /* __DVSEVENTWIDGET_HPP__B36E956F_AC52_4740_9A93_3C82F0E75D71 */

