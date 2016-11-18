#ifndef __COMMANDINTERFACE_HPP__17D6142D_6221_41F1_990B_AB0C1E9A4388
#define __COMMANDINTERFACE_HPP__17D6142D_6221_41F1_990B_AB0C1E9A4388

#include <memory>
#include <QMdiSubWindow>

class QTextEdit;
class QLineEdit;
class QGridLayout;
class QFrame;

namespace nst {

class RobotControl;

namespace gui {


class CommandInterface: public QMdiSubWindow
{
	Q_OBJECT
public:
	CommandInterface(RobotControl *control, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CommandInterface();

public slots:
	void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;
	void newResponse(std::shared_ptr<QString> str);
	void onEdtCommandReturnPressed();

signals:
	void closing();

private:
	RobotControl * _control;
	QFrame *_centralWidget;
	QGridLayout *_layout;
	QTextEdit *_edtResponse;
	QLineEdit *_edtCommand;
};


}} // nst::gui

#endif /* __COMMANDINTERFACE_HPP__17D6142D_6221_41F1_990B_AB0C1E9A4388 */

