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
class CommandInterface;

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
	void onCbShowEventsStateChanged(int state);
	void onCbUserFunctionStateChanged(int state);
	void onCmbUserFunctionIndexChanged(int index);
	void onCbManualControlStateChanged(int state);
	void onCbCommandInterfaceStateChanged(int state);
	void onCbLaserPointerStateChanged(int state);
	void onCbBuzzerStateChanged(int state);
	void onCbLEDsStateChanged(int state);

	// 'sub'-window notifications
	void onEventVisualizerClosing();
	void onNavigationClosing();
	void onCommandInterfaceClosing();

	// control slots
	void onControlConnected();
	void onControlDisconnected();
	void onControlUserFunctionData(int id, int type, void *data);

private:
	void openEventVisualizerWindow();
	void closeEventVisualizerWindow();
	void openNavigationWindow();
	void closeNavigationWindow();
	void setUserFunction(unsigned index);
	void unsetUserFunction();
	void openCommandInterface();
	void closeCommandInterface();

	void laserpointerSettingsChanged();
	void buzzerSettingsChanged();
	void ledSettingsChanged();

	RobotControl *_control;

	QFrame *_centralWidget = nullptr;
	QLineEdit *_edtURI = nullptr;
	QPushButton *_btnConnect = nullptr;

	QCheckBox *_cbCommandInterface = nullptr;
	QCheckBox *_cbManualControl = nullptr;
	QCheckBox *_cbShowEvents = nullptr;
	QCheckBox *_cbMagnetWindows = nullptr;
	QCheckBox *_cbUserFunction = nullptr;
	QCheckBox *_cbLaserPointer = nullptr;
	QCheckBox *_cbBuzzer = nullptr;
	QCheckBox *_cbLEDs = nullptr;

	QLineEdit *_edtLPBaseFreq = nullptr;
	QLineEdit *_edtLPRelative = nullptr;
	QLineEdit *_edtBuzzerBaseFreq = nullptr;
	QLineEdit *_edtBuzzerRelative = nullptr;
	QLineEdit *_edtLEDBaseFreq = nullptr;
	QLineEdit *_edtLEDFrontRelative = nullptr;
	QLineEdit *_edtLEDBackRelative = nullptr;

	QComboBox *_cmbUserFunction = nullptr;

	// 'sub'-windows
	EventVisualizerWindow *_winEventVisualizer = nullptr;
	NavigationWindow *_winNavigation = nullptr;
	CommandInterface *_winCommands = nullptr;
};


}} // nst::gui::

#endif /* __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82 */

