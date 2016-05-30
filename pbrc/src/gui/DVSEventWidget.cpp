#include "utils.hpp"
#include "DVSEventWidget.hpp"
#include <QRect>
#include <QPainter>
#include "Datatypes.hpp"

namespace nst { namespace gui {


DVSEventWidget::
DVSEventWidget(QWidget *parent) : QWidget(parent), _track_x(-1), _track_y(-1)
{
	_image = new QImage(128, 128, QImage::Format_RGB32);
	_decay_factor = 0.80;

	connect(&_timer, &QTimer::timeout, this, &DVSEventWidget::decayImage);
	_timer.setInterval(20);
	_timer.start();
}


DVSEventWidget::
~DVSEventWidget()
{
	delete _image;
}


void DVSEventWidget::
setDecayFactor(float decay_factor)
{
	_decay_factor = decay_factor;
}


void DVSEventWidget::
decayPixel(unsigned *p)
{
	*p = qRgb(qRed(*p) * _decay_factor,
		  qGreen(*p) * _decay_factor,
		  qBlue(*p) * _decay_factor);
}


void DVSEventWidget::
decayImage()
{
	unsigned *bits = (unsigned*)_image->bits();
	unsigned N = _image->width() * _image->height();
	for (unsigned i = 0; i < N; i++)
		decayPixel(bits++);
	update();
}


void DVSEventWidget::
paintEvent(QPaintEvent * /*event*/)
{
	QPainter p(this);
	QRect rect(0, 0, geometry().width(), geometry().height());
	p.drawImage(rect, *_image);
}


void DVSEventWidget::
newEvent(std::shared_ptr<DVSEvent> ev)
{
	constexpr QRgb COLOR_ON = qRgb(0, 0, 255);
	constexpr QRgb COLOR_OFF = qRgb(255, 0, 0);
	constexpr QRgb COLOR_TRACK = qRgb(0, 255, 0);

	// TODO: check if this function is called too often. If so,
	// accumulate events to stop updating the image too frequently
	_image->setPixel(ev->y, ev->x, ev->p ? COLOR_ON : COLOR_OFF);

	// tracking information
	if (_track_x >= 0 && _track_y >= 0) {
		for (int x = 0; x < 128; ++x)
			_image->setPixel(x, _track_y, COLOR_TRACK);
		for (int y = 0; y < 128; ++y)
			_image->setPixel(_track_x, y, COLOR_TRACK);
	}
}


void DVSEventWidget::
setTrackingLine(int x, int y)
{
	_track_x = x;
	_track_y = y;
}


}} // nst::gui::
