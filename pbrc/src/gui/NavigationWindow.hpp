#ifndef __NAVIGATIONWINDOW_HPP__9C508A24_10FB_4C92_8555_D64EDA1FACAB
#define __NAVIGATIONWINDOW_HPP__9C508A24_10FB_4C92_8555_D64EDA1FACAB

#include <memory>
#include <QMdiSubWindow>

// forward declarations
class QFrame;
class QResizeEvent;
class QCloseEvent;

namespace nst {

// forward declarations
class RobotControl;

namespace gui {

class NavigationWidget;

class NavigationWindow : public QMdiSubWindow
{
	Q_OBJECT
public:
	NavigationWindow(RobotControl * control, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~NavigationWindow();

	void resizeEvent(QResizeEvent *ev);

public slots:
	void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;

signals:
	void closing();

private slots:
	void onNavigationUpdate(const QPointF pos);

private:
	RobotControl *_control;
	NavigationWidget *_wdgtNav = nullptr;

	// take the NavigationWidget data directly, or transform. see function
	// onNavigationUpdate for details
	bool _linear_control_mode = false;
};


}} // nst::gui::

#endif /* __NAVIGATIONWINDOW_HPP__9C508A24_10FB_4C92_8555_D64EDA1FACAB */

