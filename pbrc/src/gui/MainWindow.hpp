#ifndef __MAINWINDOW_HPP__7C3D1822_632F_4DE0_AF8F_F66E3E2AB551
#define __MAINWINDOW_HPP__7C3D1822_632F_4DE0_AF8F_F66E3E2AB551

#include <QMainWindow>
#include <vector>

class QMdiArea;
class QMdiSubWindow;

namespace nst { namespace gui {

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

public slots:
	void onSubwindowClosing(QMdiSubWindow *win);

private:
	QMdiArea *_mdi;
	std::vector<QMdiSubWindow*> _wins;
};


}} // nst::gui::

#endif /* __MAINWINDOW_HPP__7C3D1822_632F_4DE0_AF8F_F66E3E2AB551 */
