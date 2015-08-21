#ifndef __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82
#define __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82

#include <memory>
#include <QMdiSubWindow>

class QFrame;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QMoveEvent;
class QComboBox;

namespace nst {

// forward declarations
class RobotControl;

namespace gui {

// forward declarations
class NavigationWindow;
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

	void closeEvent(QCloseEvent *ev) override;
	void moveEvent(QMoveEvent *ev) override;

signals:
	void closing(QMdiSubWindow *win);

private slots:
	// ui slots
	void onBtnConnectClicked();
	void onShowEventsStateChanged(int state);
	void onCbUserFunctionStateChanged(int state);
	void onCmbUserFunctionIndexChanged(int index);
	void onCbManualControlStateChanged(int state);

	// 'sub'-window notifications
	void onEventVisualizerClosing();
	void onNavigationClosing();

	// control slots

	void onControlConnected();
	void onControlDisconnected();

private:
	void openEventVisualizerWindow();
	void closeEventVisualizerWindow();
	void openNavigationWindow();
	void closeNavigationWindow();
	void setUserFunction(unsigned index);
	void unsetUserFunction();

	RobotControl *_control;

	QFrame *_centralWidget = nullptr;
	QLineEdit *_edtIP = nullptr;
	QPushButton *_btnConnect = nullptr;

	QCheckBox *_cbManualControl = nullptr;
	QCheckBox *_cbShowEvents = nullptr;
	QCheckBox *_cbMagnetWindows = nullptr;
	QCheckBox *_cbUserFunction = nullptr;

	QComboBox *_cmbUserFunction = nullptr;

	// 'sub'-windows
	EventVisualizerWindow *_winEventVisualizer = nullptr;
	NavigationWindow *_winNavigation = nullptr;
};


}} // nst::gui::

#endif /* __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82 */

