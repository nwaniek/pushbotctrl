#ifndef __DIALOG_HPP__F6563F73_2A6D_435D_B63D_EDCEA5D13311
#define __DIALOG_HPP__F6563F73_2A6D_435D_B63D_EDCEA5D13311

#include <QDialog>

class QPushButton;
class QThread;

namespace nst {

class PushbotConnection;

class Dialog : public QDialog
{
	Q_OBJECT
public:
	Dialog(QWidget *parent = 0);

private slots:
	void btnCreateClicked();

private:
	QPushButton *buttonCreate;
	QPushButton *buttonConnect;
	QPushButton *buttonDisconnect;

	QThread *_con_thread;
	PushbotConnection *_con;

	int state = 0;
};

}

#endif /* __DIALOG_HPP__F6563F73_2A6D_435D_B63D_EDCEA5D13311 */

