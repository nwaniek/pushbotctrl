#include "CommandInterface.hpp"
#include "Commands.hpp"
#include "RobotControl.hpp"
#include <QFrame>
#include <QLayout>
#include <QSizePolicy>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QLineEdit>
#include <QGridLayout>
#include <QTextEdit>
#include <QFont>
#include <QFontDatabase>


namespace nst { namespace gui {

CommandInterface::CommandInterface(RobotControl *control, QWidget *parent, Qt::WindowFlags flags)
: QMdiSubWindow(parent, flags), _control(control)
{
	const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

	_centralWidget = new QFrame(this);
	layout()->addWidget(_centralWidget);

	_layout = new QGridLayout();
	_edtResponse = new QTextEdit(_centralWidget);
	_edtCommand = new QLineEdit(_centralWidget);

	_edtResponse->setFont(fixedFont);
	_edtCommand->setFont(fixedFont);

	_layout->addWidget(_edtResponse, 0, 0);
	_layout->addWidget(_edtCommand, 1, 0);

	_edtResponse->setReadOnly(true);
	_centralWidget->setLayout(_layout);

	// connect stuff
	connect(_control, &RobotControl::responseReceived, this, &CommandInterface::newResponse);
	connect(_edtCommand, &QLineEdit::returnPressed, this, &CommandInterface::onEdtCommandReturnPressed);
}


CommandInterface::~CommandInterface()
{
	disconnect(_control, &RobotControl::responseReceived, this, &CommandInterface::newResponse);
}


void CommandInterface::
closeEvent(QCloseEvent *ev)
{
	emit closing();
	ev->accept();
}


void CommandInterface::
newResponse(std::shared_ptr<QString> str)
{
	QString _str(*str);
	_edtResponse->append(_str);
}


void CommandInterface::
onEdtCommandReturnPressed()
{
	std::string s = _edtCommand->text().toUtf8().constData();
	s.append("\n");
	_control->sendCommand(s);
	_edtCommand->setText("");
}



}} // nst::gui
