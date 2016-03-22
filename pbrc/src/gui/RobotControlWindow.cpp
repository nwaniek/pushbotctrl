#include "gui/RobotControlWindow.hpp"

#include <iostream>

#include <QCloseEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QPushButton>
#include <QCheckBox>
#include <QMdiArea>
#include <QMoveEvent>
#include <QComboBox>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QSizePolicy>

#include "utils.hpp"
#include "RobotControl.hpp"
#include "Commands.hpp"
#include "UserFunction.hpp"
#include "gui/NavigationWindow.hpp"
#include "gui/EventVisualizerWindow.hpp"

namespace nst { namespace gui {


RobotControlWindow::
RobotControlWindow(QWidget *parent, Qt::WindowFlags flags)
: QMdiSubWindow(parent, flags)
{
	// create control and hook into signals
	_control = new RobotControl();
	connect(_control, &RobotControl::connected, this, &RobotControlWindow::onControlConnected);
	connect(_control, &RobotControl::disconnected, this, &RobotControlWindow::onControlDisconnected);

	// window frame
	this->setWindowTitle("Robot Control " + QString::number(_control->id()));

	// create GUI
	_centralWidget = new QFrame(this);
	layout()->addWidget(_centralWidget);

	QGridLayout *layout = new QGridLayout();
	layout->addWidget(new QLabel("URI: ", _centralWidget));

	int row = 0;

	// connectivity
	// _edtURI = new QLineEdit("10.162.242.xxx", _centralWidget);
	_edtURI = new QLineEdit("/dev/ttyUSB0?baudrate=12000000", _centralWidget);
	layout->addWidget(_edtURI, row, 1);

	_btnConnect = new QPushButton("connect", _centralWidget);
	layout->addWidget(_btnConnect, row, 2);
	connect(_btnConnect, &QPushButton::clicked, this, &RobotControlWindow::onBtnConnectClicked);

	++row;

	// manual control
	_cbManualControl = new QCheckBox("manual control", _centralWidget);
	layout->addWidget(_cbManualControl, row, 0, 1, 3);
	// _cbManualControl->setCheckState(Qt::Checked);
	connect(_cbManualControl, &QCheckBox::stateChanged, this, &RobotControlWindow::onCbManualControlStateChanged);

	/*
	++row;

	// navigation widget
	_wdgtNav = new NavigationWidget(20, _centralWidget);
	layout->addWidget(_wdgtNav, row, 0, 1, 3);
	connect(_wdgtNav, &NavigationWidget::navigationUpdate, this, &RobotControlWindow::onNavigationUpdate);
	connect(_control, &RobotControl::disconnected, _wdgtNav, &NavigationWidget::reset);
	*/

	++row;

	// visualize events?
	_cbShowEvents = new QCheckBox("visualize DVS events", _centralWidget);
	_cbShowEvents->setCheckState(Qt::Checked);
	layout->addWidget(_cbShowEvents, row, 0, 1, 3);
	connect(_cbShowEvents, &QCheckBox::stateChanged, this, &RobotControlWindow::onCbShowEventsStateChanged);

	++row; {
	auto line = new QFrame(_centralWidget);
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	layout->addWidget(line, row, 0, 1, 3);
	}


	++row;

	// frame for frequency settings pointer
	auto _frameFreq = new QFrame(_centralWidget);
	auto _layoutFreq = new QGridLayout();
	_frameFreq->setLayout(_layoutFreq);
	_frameFreq->setFrameStyle(QFrame::Panel | QFrame::Raised);
	_frameFreq->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
	_frameFreq->setContentsMargins(0, 0, 0, 0);
	_frameFreq->layout()->setContentsMargins(0, 0, 0, 0);

	// laser pointer
	int frame_row = 0;
	_cbLaserPointer = new QCheckBox("Laser Pointer", _frameFreq);
	_cbLaserPointer->setCheckState(Qt::Unchecked);
	_layoutFreq->addWidget(_cbLaserPointer, frame_row, 0);

	_edtLPBaseFreq = new QLineEdit("1", _frameFreq);
	_edtLPBaseFreq->setValidator(new QIntValidator(0, 1000000, this));
	_layoutFreq->addWidget(_edtLPBaseFreq, frame_row, 1);
	_edtLPBaseFreq->setEnabled(false);

	auto _lblLPBaseFreq = new QLabel("Hz");
	_layoutFreq->addWidget(_lblLPBaseFreq, frame_row, 2);

	_edtLPRelative = new QLineEdit("50", _frameFreq);
	_edtLPRelative->setValidator(new QIntValidator(0, 100, this));
	_layoutFreq->addWidget(_edtLPRelative, frame_row, 3);
	_edtLPRelative->setEnabled(false);

	auto _lblLPRelative = new QLabel("%");
	_layoutFreq->addWidget(_lblLPRelative, frame_row, 4);

	++frame_row;

	// Buzzer
	_cbBuzzer = new QCheckBox("Buzzer", _frameFreq);
	_cbBuzzer->setCheckState(Qt::Unchecked);
	_layoutFreq->addWidget(_cbBuzzer, frame_row, 0);

	_edtBuzzerBaseFreq = new QLineEdit("1", _frameFreq);
	_edtBuzzerBaseFreq->setValidator(new QIntValidator(0, 1000000, this));
	_layoutFreq->addWidget(_edtBuzzerBaseFreq, frame_row, 1);
	_edtBuzzerBaseFreq->setEnabled(false);

	auto _lblBuzzerBaseFreq = new QLabel("Hz");
	_layoutFreq->addWidget(_lblBuzzerBaseFreq, frame_row, 2);

	_edtBuzzerRelative = new QLineEdit("50", _frameFreq);
	_edtBuzzerRelative->setValidator(new QIntValidator(0, 100, this));
	_layoutFreq->addWidget(_edtBuzzerRelative, frame_row, 3);
	_edtBuzzerRelative->setEnabled(false);

	auto _lblBuzzerRelative = new QLabel("%");
	_layoutFreq->addWidget(_lblBuzzerRelative, frame_row, 4);

	++frame_row;

	// LED Front
	_cbLEDs = new QCheckBox("LEDs", _frameFreq);
	_cbLEDs->setCheckState(Qt::Unchecked);
	_layoutFreq->addWidget(_cbLEDs, frame_row, 0);

	_edtLEDBaseFreq = new QLineEdit("1", _frameFreq);
	_edtLEDBaseFreq->setValidator(new QIntValidator(0, 1000000, this));
	_layoutFreq->addWidget(_edtLEDBaseFreq, frame_row, 1);
	_edtLEDBaseFreq->setEnabled(false);

	auto _lblLEDFrontBaseFreq = new QLabel("Hz");
	_layoutFreq->addWidget(_lblLEDFrontBaseFreq, frame_row, 2);

	_edtLEDFrontRelative = new QLineEdit("50", _frameFreq);
	_edtLEDFrontRelative->setValidator(new QIntValidator(0, 100, this));
	_layoutFreq->addWidget(_edtLEDFrontRelative, frame_row, 3);
	_edtLEDFrontRelative->setEnabled(false);

	auto _lblLEDFrontRelative = new QLabel("% Front");
	_layoutFreq->addWidget(_lblLEDFrontRelative, frame_row, 4);

	++frame_row;

	// LED Back
	_edtLEDBackRelative = new QLineEdit("50", _frameFreq);
	_edtLEDBackRelative->setValidator(new QIntValidator(0, 100, this));
	_layoutFreq->addWidget(_edtLEDBackRelative, frame_row, 3);
	_edtLEDBackRelative->setEnabled(false);

	auto _lblLEDBackRelative = new QLabel("% Back");
	_layoutFreq->addWidget(_lblLEDBackRelative, frame_row, 4);


	// connect all events
	connect(_cbLaserPointer, &QCheckBox::stateChanged, this, &RobotControlWindow::onCbLaserPointerStateChanged);
	connect(_cbBuzzer, &QCheckBox::stateChanged, this, &RobotControlWindow::onCbBuzzerStateChanged);
	connect(_cbLEDs, &QCheckBox::stateChanged, this, &RobotControlWindow::onCbLEDsStateChanged);

	connect(_edtLPBaseFreq, &QLineEdit::textChanged, this, &RobotControlWindow::laserpointerSettingsChanged);
	connect(_edtLPRelative, &QLineEdit::textChanged, this, &RobotControlWindow::laserpointerSettingsChanged);
	connect(_edtBuzzerBaseFreq, &QLineEdit::textChanged, this, &RobotControlWindow::buzzerSettingsChanged);
	connect(_edtBuzzerRelative, &QLineEdit::textChanged, this, &RobotControlWindow::buzzerSettingsChanged);
	connect(_edtLEDBaseFreq, &QLineEdit::textChanged, this, &RobotControlWindow::ledSettingsChanged);
	connect(_edtLEDFrontRelative, &QLineEdit::textChanged, this, &RobotControlWindow::ledSettingsChanged);
	connect(_edtLEDBackRelative, &QLineEdit::textChanged, this, &RobotControlWindow::ledSettingsChanged);

	layout->addWidget(_frameFreq, row, 0, 1, 3);

	++row;

	// user function
	_cbUserFunction = new QCheckBox("", _centralWidget);
	_cbUserFunction->setCheckState(Qt::Unchecked);
	layout->addWidget(_cbUserFunction, row, 0, 1, 1);
	connect(_cbUserFunction, &QCheckBox::stateChanged, this, &RobotControlWindow::onCbUserFunctionStateChanged);

	_cmbUserFunction = new QComboBox(_centralWidget);
	for (size_t i = 0; i < LENGTH(user_functions); i++)
		_cmbUserFunction->addItem(user_functions[i].name);
	_cmbUserFunction->setEnabled(false);
	layout->addWidget(_cmbUserFunction, row, 1, 1, 2);
	void(QComboBox::*cmbsignal)(int) = &QComboBox::currentIndexChanged;
	connect(_cmbUserFunction, cmbsignal, this, &RobotControlWindow::onCmbUserFunctionIndexChanged);

	++row; {
	auto line = new QFrame(_centralWidget);
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	layout->addWidget(line, row, 0, 1, 3);
	}

	++row;

	// magnetic windows?
	_cbMagnetWindows = new QCheckBox("magnetic windows", _centralWidget);
	_cbMagnetWindows->setCheckState(Qt::Checked);
	layout->addWidget(_cbMagnetWindows, row, 0, 1, 3);

	_centralWidget->setLayout(layout);
	setAttribute(Qt::WA_DeleteOnClose);
}


RobotControlWindow::
~RobotControlWindow()
{
	// close all related windows. do not delete, they have the DeleteOnClose
	// flag set so will tidy up themselves
	if (_winNavigation) closeNavigationWindow();
	if (_winEventVisualizer) closeEventVisualizerWindow();
}


void RobotControlWindow::
closeEvent(QCloseEvent *ev)
{
	emit closing(this);
	QMdiSubWindow::closeEvent(ev);
}


void RobotControlWindow::
onBtnConnectClicked()
{
	if (!_control->isConnected())
		_control->connectRobot(_edtURI->text());
	else
		_control->disconnectRobot();
}


void RobotControlWindow::
openEventVisualizerWindow()
{
	// create event visualizer and move relative to control window
	const QRect geom = frameGeometry();

	_winEventVisualizer = new EventVisualizerWindow(_control, mdiArea());
	_winEventVisualizer->resize(250, 250);
	_winEventVisualizer->show();

	// move it right next to the existing window
	int xoffset = geom.topRight().x();
	int yoffset = geom.topRight().y();
	if (_winNavigation) {
		const QRect geom2 = _winNavigation->frameGeometry();
		xoffset = geom2.topRight().x();
		yoffset = geom2.topRight().y();
	}

	_winEventVisualizer->move(xoffset, yoffset);
	_winEventVisualizer->setWindowTitle("Events: " + _edtURI->text());

	connect(_winEventVisualizer, &EventVisualizerWindow::closing, this, &RobotControlWindow::onEventVisualizerClosing);
}

void RobotControlWindow::
openNavigationWindow()
{
	// create event visualizer and move relative to control window
	const QRect geom = frameGeometry();

	_winNavigation = new NavigationWindow(_control, mdiArea());
	_winNavigation->resize(250, 250);
	_winNavigation->show();

	int xoffset = geom.topRight().x();
	int yoffset = geom.topRight().y();
	if (_winEventVisualizer) {
		const QRect geom2 = _winEventVisualizer->frameGeometry();
		xoffset = geom2.topRight().x();
		yoffset = geom2.topRight().y();
	}

	_winNavigation->move(xoffset, yoffset);
	_winNavigation->setWindowTitle("Navigation " + _edtURI->text());

	connect(_winNavigation, &NavigationWindow::closing, this, &RobotControlWindow::onNavigationClosing);
}

void RobotControlWindow::
closeNavigationWindow()
{
	if (_winNavigation) _winNavigation->close();
	_winNavigation = nullptr;
}


void RobotControlWindow::
closeEventVisualizerWindow()
{
	if (_winEventVisualizer) _winEventVisualizer->close();
	_winEventVisualizer = nullptr;
}


void RobotControlWindow::
onControlConnected()
{
	_btnConnect->setText("disconnect");
	_edtURI->setReadOnly(true);
	_edtURI->setEnabled(false);

	// set up an event visualizer
	// TODO: check the checkbox if we really need to
	if (_winEventVisualizer) {
		std::cerr << "RobotControlWindow: something went wrong, _winEventVisualizer not nullptr" << std::endl;
		_winEventVisualizer->close();
	}
	if (_winNavigation) {
		std::cerr << "RobotControlWindow: something went wrong, _winNavigation not nullptr" << std::endl;
		_winNavigation->close();
	}

	// open windows
	if (_cbManualControl->checkState() == Qt::Checked)
		openNavigationWindow();

	if (_cbShowEvents->checkState() == Qt::Checked)
		openEventVisualizerWindow();

	// check for any specific options
	laserpointerSettingsChanged();
	buzzerSettingsChanged();
	ledSettingsChanged();

	// set a user function
	if (_cbUserFunction->checkState() == Qt::Checked) {
		int index = _cmbUserFunction->currentIndex();
		if (index >= 0)
			setUserFunction(static_cast<unsigned>(index));
		else
			unsetUserFunction();
	}
	else
		unsetUserFunction();
}


void RobotControlWindow::
onControlDisconnected()
{
	// remember the old state of the cb
	auto old = _cbShowEvents->checkState();
	closeEventVisualizerWindow();
	_cbShowEvents->setCheckState(old);

	old = _cbManualControl->checkState();
	closeNavigationWindow();
	_cbManualControl->setCheckState(old);

	_edtURI->setReadOnly(false);
	_edtURI->setEnabled(true);
	_btnConnect->setText("connect");
}


void RobotControlWindow::
onCbShowEventsStateChanged(int state)
{
	if (!_control->isConnected()) return;

	if (state == Qt::Checked)
		openEventVisualizerWindow();
	else
		closeEventVisualizerWindow();
}


void RobotControlWindow::
onEventVisualizerClosing()
{
	// remove the pointer to the window to make sure that we do not
	// accidentally access it -> segfault
	_winEventVisualizer = nullptr;
	_cbShowEvents->setCheckState(Qt::Unchecked);
}


void RobotControlWindow::
moveEvent(QMoveEvent *ev)
{
	// if we have magnet windows, move all 'sub' windows, too
	if (_cbMagnetWindows->checkState() == Qt::Checked) {
		int dx = ev->pos().x() - ev->oldPos().x();
		int dy = ev->pos().y() - ev->oldPos().y();

		if (_winEventVisualizer) {
			int x = _winEventVisualizer->pos().x();
			int y = _winEventVisualizer->pos().y();
			_winEventVisualizer->move(x + dx, y + dy);
		}
		if (_winNavigation) {
			int x = _winNavigation->pos().x();
			int y = _winNavigation->pos().y();
			_winNavigation->move(x + dx, y + dy);
		}
	}
	ev->accept();
}


void RobotControlWindow::
onCbUserFunctionStateChanged(int state)
{
	if (state == Qt::Checked) {
		_cmbUserFunction->setEnabled(true);
		setUserFunction(_cmbUserFunction->currentIndex());
	}
	else {
		_cmbUserFunction->setEnabled(false);
		unsetUserFunction();
	}
}


void RobotControlWindow::
onCmbUserFunctionIndexChanged(int index)
{
	// check the value. if no item has been selected we want to unset the
	// user function
	if (index < 0)
		unsetUserFunction();
	else
		setUserFunction(static_cast<unsigned>(index));
}


void RobotControlWindow::
setUserFunction(unsigned index)
{
	if (index < LENGTH(user_functions))
		_control->setUserFunction(&user_functions[index]);
	else
		std::cerr << "Invalid User Function ID " << index << std::endl;
}


void RobotControlWindow::
unsetUserFunction()
{
	_control->unsetUserFunction();
}


void RobotControlWindow::
onCbManualControlStateChanged(int state)
{
	if (!_control->isConnected()) return;

	if (state == Qt::Checked)
		openNavigationWindow();
	else
		closeNavigationWindow();
}

void RobotControlWindow::
onNavigationClosing()
{
	_winNavigation = nullptr;
	_cbManualControl->setCheckState(Qt::Unchecked);
}

void RobotControlWindow::
onCbLaserPointerStateChanged(int state)
{
	// GUI
	_edtLPBaseFreq->setEnabled(state == Qt::Checked);
	_edtLPRelative->setEnabled(state == Qt::Checked);

	// control
	laserpointerSettingsChanged();
}


void RobotControlWindow::
onCbBuzzerStateChanged(int state)
{
	// GUI
	_edtBuzzerBaseFreq->setEnabled(state == Qt::Checked);
	_edtBuzzerRelative->setEnabled(state == Qt::Checked);

	// control
	buzzerSettingsChanged();
}


void RobotControlWindow::
onCbLEDsStateChanged(int state)
{
	// GUI
	_edtLEDBaseFreq->setEnabled(state == Qt::Checked);
	_edtLEDFrontRelative->setEnabled(state == Qt::Checked);
	_edtLEDBackRelative->setEnabled(state == Qt::Checked);

	// Control
	ledSettingsChanged();
}


void RobotControlWindow::
laserpointerSettingsChanged()
{
	if (!_control->isConnected()) return;
	if (_cbLaserPointer->checkState() == Qt::Checked) {
		int freq  = _edtLPBaseFreq->text().isEmpty() ? 0 : _edtLPBaseFreq->text().toInt();
		float rel = freq == 0 ? 0.0f :
			(_edtLPRelative->text().isEmpty() ? 0.0f : _edtLPRelative->text().toFloat() / 100.0f);

		_control->enableLaserPointer(freq, rel);
	}
	else
		_control->disableLaserPointer();
}


void RobotControlWindow::
buzzerSettingsChanged()
{
	if (!_control->isConnected()) return;
	if (_cbBuzzer->checkState() == Qt::Checked) {
		int freq  = _edtBuzzerBaseFreq->text().isEmpty() ? 0 : _edtBuzzerBaseFreq->text().toInt();
		float rel = freq == 0 ? 0.0f :
			(_edtBuzzerRelative->text().isEmpty() ? 0.0f : _edtBuzzerRelative->text().toFloat() / 100.0f);

		_control->enableBuzzer(freq, rel);
	}
	else
		_control->disableBuzzer();
}


void RobotControlWindow::
ledSettingsChanged()
{
	if (!_control->isConnected()) return;
	if (_cbLEDs->checkState() == Qt::Checked) {
		int freq  = _edtLEDBaseFreq->text().isEmpty() ? 0 : _edtLEDBaseFreq->text().toInt();
		float rel_front = freq == 0 ? 0.0f :
			(_edtLEDFrontRelative->text().isEmpty() ? 0.0f : _edtLEDFrontRelative->text().toFloat() / 100.0f);
		float rel_back = freq == 0 ? 0.0f :
			(_edtLEDBackRelative->text().isEmpty() ? 0.0f : _edtLEDBackRelative->text().toFloat() / 100.0f);


		_control->enableLEDs(freq, rel_front, rel_back);
	}
	else
		_control->disableLEDs();
}


}} // nst::gui
