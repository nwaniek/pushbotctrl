#ifndef __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82
#define __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82

#include <memory>
#include <QMdiSubWindow>

class QFrame;
class QCheckBox;
class QLineEdit;
class QPushButton;

namespace nst {

// forward declarations
class RobotControl;

namespace gui {

// forward declarations
class NavigationWidget;
class EventVisualizerWindow;

/**
 * RobotControlWindow - Widget to connect to and control a robot.
 */
class RobotControlWindow : public QMdiSubWindow
{
	Q_OBJECT
public:
	RobotControlWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~RobotControlWindow();

	void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;

signals:
	void closing(QMdiSubWindow *win);

private slots:
	// ui slots
	void onBtnConnectClicked();
	void onShowEventsStateChanged(int state);

	// 'sub'-window notifications
	void onEventVisualizerClosing();

	// control slots
	void onNavigationUpdate(const QPointF pos);
	void onControlConnected();
	void onControlDisconnected();

private:
	void openEventVisualizerWindow();
	void closeEventVisualizerWindow();

	std::shared_ptr<RobotControl> _control;

	QFrame *_centralWidget = nullptr;
	QLineEdit *_edtIP = nullptr;
	QPushButton *_btnConnect = nullptr;
	NavigationWidget *_wdgtNav = nullptr;
	QCheckBox *_cbShowEvents = nullptr;

	// 'sub'-windows
	EventVisualizerWindow *_winEventVisualizer = nullptr;
};


}} // nst::gui::

#endif /* __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82 */

