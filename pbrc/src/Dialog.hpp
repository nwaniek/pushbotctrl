#ifndef __DIALOG_HPP__F6563F73_2A6D_435D_B63D_EDCEA5D13311
#define __DIALOG_HPP__F6563F73_2A6D_435D_B63D_EDCEA5D13311

#include <QDialog>
#include "Datatypes.hpp"

class QPushButton;
class QString;
class QThread;


namespace nst {


class PushbotConnection;
class BytestreamParser;


class Dialog : public QDialog
{
	Q_OBJECT
public:
	Dialog(QWidget *parent = 0);

private slots:
	void btnCreateClicked();
	void onDVSEventReceived(const DVSEvent *ev);
	void onResponseReceived(const QString *str);

private:
	QPushButton *buttonCreate;
	QPushButton *buttonConnect;
	QPushButton *buttonDisconnect;

	QThread *_con_thread;
	PushbotConnection *_con;

	QThread *_parser_thread;
	BytestreamParser *_parser;
};

}

#endif /* __DIALOG_HPP__F6563F73_2A6D_435D_B63D_EDCEA5D13311 */

