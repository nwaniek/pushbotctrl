#ifndef __MAINWINDOW_HPP__7C3D1822_632F_4DE0_AF8F_F66E3E2AB551
#define __MAINWINDOW_HPP__7C3D1822_632F_4DE0_AF8F_F66E3E2AB551

#include <QMainWindow>
#include <vector>

class QMdiArea;
class QMdiSubWindow;
class QAction;
class QMenu;

namespace nst { namespace gui {

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();


public slots:
	void addRobotControl();
	void onSubwindowClosing(QMdiSubWindow *win);

private:
	QMdiArea *_mdi = nullptr;
	std::vector<QMdiSubWindow*> _wins;

	QMenu *_mnuFile = nullptr;
	QAction *_actAddRobotControl = nullptr;
	QAction *_actClose = nullptr;
};


}} // nst::gui::

#endif /* __MAINWINDOW_HPP__7C3D1822_632F_4DE0_AF8F_F66E3E2AB551 */

