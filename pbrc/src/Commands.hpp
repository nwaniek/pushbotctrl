#ifndef __COMMANDS_HPP__DE8555E9_1B8E_47A6_BF34_8AE88A27C9BE
#define __COMMANDS_HPP__DE8555E9_1B8E_47A6_BF34_8AE88A27C9BE

#include <iostream>
#include <string>
#include <sstream>
#include <QTcpSocket>

namespace nst {
namespace commands {


/**
 * Command - Abstract command interface
 */
struct Command
{
	virtual ~Command() {}
	virtual const std::string toString() const = 0;
};


/**
 * MotorDriver - Enable or Disable the Motor Driver of the Pushbots
 */
struct MotorDriver : Command
{
	MotorDriver(bool enabled = true) : _enabled(enabled) {}

	const std::string toString() const override
	{
		if (this->_enabled)
			return std::string("!M+\n");
		else
			return std::string("!M-\n");
	}

	void enable() { this->_enabled = true; }
	void disable() { this->_enabled = false; }

private:
	bool _enabled = true;
};


/**
 * DVS - Enable or disable event streaming from the retina
 */
struct DVS : Command
{
	DVS(bool enabled = true) : _enabled(enabled) {}

	const std::string toString() const override
	{
		if (this->_enabled)
			return std::string("E+\n");
		else
			return std::string("E-\n");
	}

	void enable() { this->_enabled = true; }
	void disable() { this->_enabled = false; }

private:
	bool _enabled = true;
};


/**
 * IMU - Enable or disable imu data streaming from on-board sensors
 */
struct IMU : Command
{
	IMU(bool enabled = true) : _enabled(enabled) {}

	const std::string toString() const override
	{
		if(this->_enabled)
			// enable streaming for 3D gyro, acc, mag (bitmask 7168) @ 125Hz
			return std::string("!S+7168,8\n");
		else
			return std::string("!S-\n");
	}
	void enable()  {this->_enabled = true; }
	void disable() {this->_enabled = false; }

private:
	bool _enabled = true;
};


/*
 * base class for everything that operates on frequencies, like buzzer, laser
 * pointer, or LEDS
 */
template <const char P>
struct freq_base_t : Command
{
	const std::string toString() const override
	{
		std::stringstream ss;
		ss << "!P" << P << "=" << std::to_string(_base_freq) << "\n";
		ss << "!P" << P << std::to_string(_id) << "=" << std::to_string(_absolute_freq) << "\n";
		return ss.str();
	}

	void setFrequency(unsigned base_freq, float relative_freq)
	{
		if (base_freq == 0u) {
			_relative_freq = 0.0;
			_base_freq = 0u;
			_absolute_freq = 0u;
		}
		else {
			// turn relative into absolute
			_relative_freq = std::abs(relative_freq);
			_base_freq = 1000000u / base_freq;
			_absolute_freq = std::floor(static_cast<float>(_base_freq) * _relative_freq);
		}
	}

	unsigned baseFrequency() const { return _base_freq / 1000000u; }
	unsigned absoluteFrequency() const { return _absolute_freq / 1000000u; }
	float relativeFrequency() const { return _relative_freq; }

protected:
	unsigned _id;
	float _relative_freq;
	unsigned _base_freq;
	unsigned _absolute_freq;
};


/*
 * Laser Pointer
 */
struct LaserPointer : freq_base_t<'A'>
{
	LaserPointer(unsigned base_freq = 0u, float relative_freq = 0.0f)
	{ _id = 0; setFrequency(base_freq, relative_freq); }
};


/*
 * Buzzer
 */
struct Buzzer : freq_base_t<'B'>
{
	Buzzer(unsigned base_freq = 0u, float relative_freq = 0.0f)
	{ _id = 0; setFrequency(base_freq, relative_freq); }
};


/*
 * LED - command to enable/disable the LEDs on top of the PushBot.
 *
 * All frequencies are expressed in Hz.
 */
struct LED : freq_base_t<'C'>
{
	typedef enum {
		Back = 0,
		Front = 1,
	} led_identifier_t;

	/*
	 * construct a new LED command. The LED will have a relative frequency
	 * w.r.t. the base frequency supplied as arguments. express base_freq in
	 * terms of Hz, and up to 1Mhz (pass in 1000000 as argument)
	 */
	LED(led_identifier_t id, unsigned base_freq = 0u, float relative_freq = 0.0f)
	{ _id= id; setFrequency(base_freq, relative_freq); }
};


/*
 * BoardLED - command to enable or disable the LED which is on the DVS Board
 */
struct BoardLED : Command
{
	typedef enum {
		LED_MODE_ON,
		LED_MODE_OFF,
		LED_MODE_BLINKING
	} boardled_mode_t;

	BoardLED(boardled_mode_t mode = LED_MODE_OFF) : _mode(mode) {}

	const std::string toString() const override
	{
		switch (_mode) {
		case LED_MODE_OFF:
			return std::string("!L0\n");
		case LED_MODE_ON:
			return std::string("!L1\n");
		case LED_MODE_BLINKING:
			return std::string("!L2\n");
		}
	}

	boardled_mode_t mode() { return _mode; }
	void set_mode(boardled_mode_t mode) { _mode = mode; }

private:
	boardled_mode_t _mode;
};


/**
 * mdw_base_t - Base class for all motor duty width / velocity commands
 *
 * TODO: check if the widths are really correct for DW
 */
template <unsigned MOTORID>
struct md_base_t
{

	md_base_t(int width) : _width(width) { validate(); }
	void setWidth(int width) { _width = width; validate(); }

protected:
	const unsigned _id = MOTORID;
	int _width;

private:
	void validate()
	{
		if (_width >  100) _width =  100;
		if (_width < -100) _width = -100;
	}
};


/**
 * Macro to define any kind of motor command for duty width and velocity
 */
#define MAKE_MOTOR_COMMAND(TYPE, ID, CMD) \
	struct M ## TYPE ## ID: Command, md_base_t<ID> \
	{ \
		M ## TYPE ## ID(unsigned width) : md_base_t(width) {} \
		\
		const std::string toString() const override \
		{ \
			std::stringstream ss; \
			ss << "!" << (CMD) << std::to_string(_id) << "=" << std::to_string(_width) << "\n"; \
			return ss.str(); \
		} \
	};

/*
 * define all motorcommands for motors 0 and 1
 *	DW : Duty Width
 *	DWD: Duty Width with Decay
 *	V  : Velocity
 *	VD : Velicity with Decay
 */

// commands::MDW0
MAKE_MOTOR_COMMAND(DW,  0, "M")

// commands::MDW1
MAKE_MOTOR_COMMAND(DW,  1, "M")

// commands::MDWD0
MAKE_MOTOR_COMMAND(DWD, 0, "MD")

// commands::MDWD1
MAKE_MOTOR_COMMAND(DWD, 1, "MD")

// commands::MV0
MAKE_MOTOR_COMMAND(V,   0, "MV")

// commands::MV1
MAKE_MOTOR_COMMAND(V,   1, "MV")

// commands::MVD0
MAKE_MOTOR_COMMAND(VD,  0, "MVD")

// commands::MVD1
MAKE_MOTOR_COMMAND(VD,  1, "MVD")

// finalize
#undef MAKE_MOTOR_COMMAND


/*
 * Empty - The empty command is represented by a simple newline.
 *
 * The Empty command is useful when connecting to the robot. The internal state
 * of the robot may be such that it does not properly accept commands. Sending a
 * newline will terminate the PushBots parsing engine for whatever it currently
 * reads from the input.
 */
struct Empty : Command
{
	const std::string toString() const override
	{
		return "\n";
	}
};

}} // nst::commands



inline
QTcpSocket* operator<< (QTcpSocket* sock, const nst::commands::Command &cmd)
{
	sock->write(cmd.toString().c_str());
	return sock;
}


#endif /* __COMMANDS_HPP__DE8555E9_1B8E_47A6_BF34_8AE88A27C9BE */

