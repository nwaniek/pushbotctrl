#ifndef __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82
#define __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82

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
	void onBtnConnectClicked();
	void onNavigationUpdate(const QPointF pos);
	void onControlConnected();
	void onControlDisconnected();

private:
	RobotControl *_control;

	QFrame *_centralWidget;
	QLineEdit *_edtIP;
	QPushButton *_btnConnect;
	NavigationWidget *_wdgtNav;
	QCheckBox *_cbShowEvents;
};


}} // nst::gui::

#endif /* __ROBOTCONTROLWIDGET_HPP__1F654E45_7026_4F8E_ACB3_933E32B4ED82 */

