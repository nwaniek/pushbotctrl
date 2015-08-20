#ifndef __EVENTVISUALIZERWINDOW_HPP__A93CCD32_800E_47B5_B2ED_BFE5EB16A55E
#define __EVENTVISUALIZERWINDOW_HPP__A93CCD32_800E_47B5_B2ED_BFE5EB16A55E

#include <memory>
#include <QMdiSubWindow>

// forward declarations
class QFrame;
class QResizeEvent;
class QCloseEvent;

namespace nst {

// forward declarations
class RobotControl;
struct DVSEvent;

namespace gui {

class DVSEventWidget;

class EventVisualizerWindow : public QMdiSubWindow
{
	Q_OBJECT
public:
	EventVisualizerWindow(const RobotControl * const control, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~EventVisualizerWindow();

	void resizeEvent(QResizeEvent *ev);

public slots:
	void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;

signals:
	void closing();


private:
	const RobotControl * const _control;
	DVSEventWidget *_wdgtEvents = nullptr;
};


}} // nst::gui::

#endif /* __EVENTVISUALIZERWINDOW_HPP__A93CCD32_800E_47B5_B2ED_BFE5EB16A55E */

