#ifndef __SENSORSPROCESSOR_HPP__50486FA1_C215_4455_A737_ADAFD76CA620
#define __SENSORSPROCESSOR_HPP__50486FA1_C215_4455_A737_ADAFD76CA620

#include <QObject>
#include <cmath>
#include <iostream>
#include <memory>
#include "Datatypes.hpp"

namespace nst {

#define IN_WEIGHT 0.5
#define TO_DEG(X) (double)X*180.0/M_PI

// forward declarations
struct IMUEvent;

/**
 * SensorProcessor - Use single sensory samples to compute robot RPY.
 */
class SensorsProcessor : public QObject
{
        Q_OBJECT

public:
         SensorsProcessor(QObject *parent = 0);
        ~SensorsProcessor();

	bool parseString(const QString *str);

signals:
	void sensorEvent(std::shared_ptr<SensorEvent> ev);

public:
        void processSample(const IMUEvent *ev);

private:
	RPYEvent * _rpyEstimate; // estimates
	double *fa, *fm;	 // filtered data
};


/**
 * decode incoming sensory values from IMU (encoding is Q16)
 */
inline
double decodeSensorVal(QString str)
{
    // get the 32 bit binary representation of the decoded decimal in the Q16 string
    std::bitset<32> bStr(str.toUInt(NULL, 16));
    // extract the correct signed value from 2's complement representation
    return  ((bStr.test(31))?(((double)(bStr.flip(31).to_ulong()) - pow(2, 31))/pow(2,16)):((double)(bStr.to_ulong())/pow(2,16)));
}

} // nst::

#endif /* __SENSORSPROCESSOR_HPP__50486FA1_C215_4455_A737_ADAFD76CA620 */

