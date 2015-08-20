#include "SensorsProcessor.hpp"
#include "utils.hpp"

#include <QRegularExpression>

namespace nst{

SensorsProcessor::
SensorsProcessor(QObject *parent)
: QObject(parent)
{
	_rpyEstimate = new RPYEvent;
	fa = new double[3]();
	fm = new double[3]();
}


SensorsProcessor::
~SensorsProcessor()
{
	delete[] fm;
	delete[] fa;
        delete _rpyEstimate;
}


void SensorsProcessor::
parseString(const QString *str)
{
	// parse the response into corresponding structs
        // one response per sensor
        short axisIdx = 0;
        IMUEvent *se = new IMUEvent;

        // check which sensor we have (S10 - gyro, S11 - acc, S12 - mag)
        short seIdx = str->contains("-S10", Qt::CaseInsensitive)?0:(str->contains("-S11", Qt::CaseInsensitive)?1:2);

	// extract the content of the robot stream
        QStringList rawData = str->split(QRegularExpression("-S1\\d\\s"), QString::SkipEmptyParts);
        foreach(const QString &sensorEntry, rawData){
                QStringList axesVals = sensorEntry.split(" ");
                axisIdx = 0;
                foreach(const QString axisEntry, axesVals){
                        switch(seIdx){
                                case 0:
                                        se->g[axisIdx]= decodeSensorVal(axisEntry);
                                        break;
                                case 1:
                                        se->a[axisIdx]= decodeSensorVal(axisEntry);
                                        break;
                                case 2:
                                        se->m[axisIdx]= decodeSensorVal(axisEntry);
                                        break;
                        }
                       if((axisIdx++)==2) break;
                }
        }

        // ... and send the packed struct to update RPY
        processSample(se);
	delete se;
}


void SensorsProcessor::
processSample(const IMUEvent *se)
{
	// process incoming sensory sample
	// gyroscope data (raw data in deg/s)
	// sampling @ 125Hz
	const double dt = 0.008f;
	_rpyEstimate->roll[IMUEvent::GYROSCOPE]  += se->g[IMUEvent::XAXIS]*dt;
	_rpyEstimate->pitch[IMUEvent::GYROSCOPE] += se->g[IMUEvent::YAXIS]*dt;
	_rpyEstimate->yaw[IMUEvent::GYROSCOPE]   += se->g[IMUEvent::ZAXIS]*dt;

	// accelerometer data (raw data in g)
	// LPF
	fa[IMUEvent::XAXIS] = se->a[IMUEvent::XAXIS]*IN_WEIGHT + fa[IMUEvent::XAXIS]*(1.0f - IN_WEIGHT);
	fa[IMUEvent::YAXIS] = se->a[IMUEvent::YAXIS]*IN_WEIGHT + fa[IMUEvent::YAXIS]*(1.0f - IN_WEIGHT);
	fa[IMUEvent::ZAXIS] = se->a[IMUEvent::ZAXIS]*IN_WEIGHT + fa[IMUEvent::ZAXIS]*(1.0f - IN_WEIGHT);

	// fill in the RPY struct
	// [-90...90] deg range
	_rpyEstimate->pitch[IMUEvent::ACCELEROMETER]  = TO_DEG(atan2(sqrt(fa[IMUEvent::XAXIS]*fa[IMUEvent::XAXIS] + fa[IMUEvent::ZAXIS]*fa[IMUEvent::ZAXIS]),fa[IMUEvent::YAXIS])) - 90.0;
	_rpyEstimate->roll[IMUEvent::ACCELEROMETER] = TO_DEG(atan2(sqrt(fa[IMUEvent::YAXIS]*fa[IMUEvent::YAXIS] + fa[IMUEvent::ZAXIS]*fa[IMUEvent::ZAXIS]),fa[IMUEvent::XAXIS])) - 90.0;
	_rpyEstimate->yaw[IMUEvent::ACCELEROMETER]   = 0.0f; // for yaw motion we are perpedicular to the gravity vector, so no contribution

	// magneto data (raw data in uT)
	// LPF
	fm[IMUEvent::XAXIS] = se->m[IMUEvent::XAXIS]*IN_WEIGHT + fm[IMUEvent::XAXIS]*(1.0f - IN_WEIGHT);
        fm[IMUEvent::YAXIS] = se->m[IMUEvent::YAXIS]*IN_WEIGHT + fm[IMUEvent::YAXIS]*(1.0f - IN_WEIGHT);
        fm[IMUEvent::ZAXIS] = se->m[IMUEvent::ZAXIS]*IN_WEIGHT + fm[IMUEvent::ZAXIS]*(1.0f - IN_WEIGHT);

	double magNorm = sqrt((fm[IMUEvent::XAXIS]*fm[IMUEvent::XAXIS]) + (fm[IMUEvent::YAXIS]*fm[IMUEvent::YAXIS]) + (fm[IMUEvent::ZAXIS]*fm[IMUEvent::ZAXIS]));

	fm[IMUEvent::XAXIS] /= magNorm;
	fm[IMUEvent::YAXIS] /= magNorm;
	fm[IMUEvent::ZAXIS] /= magNorm;

        _rpyEstimate->roll[IMUEvent::MAGNETOMETER]   = 0.0f; // no contribution for roll
        _rpyEstimate->pitch[IMUEvent::MAGNETOMETER]  = 0.0f; // no contribution for pitch
	// TODO check this equations for compensated / uncompensated yaw
         _rpyEstimate->yaw[IMUEvent::MAGNETOMETER]   = TO_DEG(atan2((fm[IMUEvent::YAXIS]*cos(_rpyEstimate->roll[IMUEvent::ACCELEROMETER]))-
									 (fm[IMUEvent::ZAXIS]*sin(_rpyEstimate->roll[IMUEvent::ACCELEROMETER])),
									 (fm[IMUEvent::XAXIS]*cos(_rpyEstimate->pitch[IMUEvent::ACCELEROMETER]))+
									 (fm[IMUEvent::YAXIS]*sin(_rpyEstimate->roll[IMUEvent::ACCELEROMETER])*sin(_rpyEstimate->pitch[IMUEvent::ACCELEROMETER]))-
									 (fm[IMUEvent::ZAXIS]*cos(_rpyEstimate->roll[IMUEvent::ACCELEROMETER])*sin(_rpyEstimate->pitch[IMUEvent::ACCELEROMETER])))) - 90.0;
	 //printf("RPYg = [%f %f %f]\n", _rpyEstimate->roll[IMUEvent::GYROSCOPE],  _rpyEstimate->pitch[IMUEvent::GYROSCOPE], _rpyEstimate->yaw[IMUEvent::GYROSCOPE]);
	 //printf("RPYa = [%f %f %f]\n", _rpyEstimate->roll[IMUEvent::ACCELEROMETER], _rpyEstimate->pitch[IMUEvent::ACCELEROMETER], _rpyEstimate->yaw[IMUEvent::ACCELEROMETER]);
	//printf("RPYm = [%f %f %f]\n", _rpyEstimate->roll[IMUEvent::MAGNETOMETER], _rpyEstimate->pitch[IMUEvent::MAGNETOMETER] , _rpyEstimate->yaw[IMUEvent::MAGNETOMETER]);


	auto ev = std::make_shared<SensorEvent>();
	ev->imu = *se;
	ev->rpy = *_rpyEstimate;
	emit sensorEvent(ev);
}

}
