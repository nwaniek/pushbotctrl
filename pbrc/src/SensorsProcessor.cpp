#include "SensorsProcessor.hpp"


namespace nst{

SensorsProcessor::
SensorsProcessor(QObject *parent) 
: QObject(parent) 
{
	_rpyEstimate = new RPYEvent; 
	fa = (double*)calloc(3, sizeof(double));
	fm = (double*)calloc(3, sizeof(double));
}


SensorsProcessor::
~SensorsProcessor()
{
        delete _rpyEstimate;
	free(fa);
	free(fm);
}

void 
SensorsProcessor::
newSample(const SensorsEvent *se)
{
	// process incoming sensory sample
	// gyroscope data (raw data in deg/s)
	// sampling @ 125Hz
	const double dt = 0.008f;
	_rpyEstimate->roll[SensorsEvent::GYROSCOPE]  += se->g[SensorsEvent::XAXIS]*dt;
	_rpyEstimate->pitch[SensorsEvent::GYROSCOPE] += se->g[SensorsEvent::YAXIS]*dt;
	_rpyEstimate->yaw[SensorsEvent::GYROSCOPE]   += se->g[SensorsEvent::ZAXIS]*dt;

	// accelerometer data (raw data in g)
	// LPF
	fa[SensorsEvent::XAXIS] = se->a[SensorsEvent::XAXIS]*IN_WEIGHT + fa[SensorsEvent::XAXIS]*(1.0f - IN_WEIGHT);
	fa[SensorsEvent::YAXIS] = se->a[SensorsEvent::YAXIS]*IN_WEIGHT + fa[SensorsEvent::YAXIS]*(1.0f - IN_WEIGHT);
	fa[SensorsEvent::ZAXIS] = se->a[SensorsEvent::ZAXIS]*IN_WEIGHT + fa[SensorsEvent::ZAXIS]*(1.0f - IN_WEIGHT);

	// fill in the RPY struct
	// [-90...90] deg range
	_rpyEstimate->pitch[SensorsEvent::ACCELEROMETER]  = TO_DEG(atan2(sqrt(fa[SensorsEvent::XAXIS]*fa[SensorsEvent::XAXIS] + fa[SensorsEvent::ZAXIS]*fa[SensorsEvent::ZAXIS]),fa[SensorsEvent::YAXIS])) - 90.0;
	_rpyEstimate->roll[SensorsEvent::ACCELEROMETER] = TO_DEG(atan2(sqrt(fa[SensorsEvent::YAXIS]*fa[SensorsEvent::YAXIS] + fa[SensorsEvent::ZAXIS]*fa[SensorsEvent::ZAXIS]),fa[SensorsEvent::XAXIS])) - 90.0;
	_rpyEstimate->yaw[SensorsEvent::ACCELEROMETER]   = 0.0f; // for yaw motion we are perpedicular to the gravity vector, so no contribution
	
	// magneto data (raw data in uT)
	// LPF
	fm[SensorsEvent::XAXIS] = se->m[SensorsEvent::XAXIS]*IN_WEIGHT + fm[SensorsEvent::XAXIS]*(1.0f - IN_WEIGHT);
        fm[SensorsEvent::YAXIS] = se->m[SensorsEvent::YAXIS]*IN_WEIGHT + fm[SensorsEvent::YAXIS]*(1.0f - IN_WEIGHT);
        fm[SensorsEvent::ZAXIS] = se->m[SensorsEvent::ZAXIS]*IN_WEIGHT + fm[SensorsEvent::ZAXIS]*(1.0f - IN_WEIGHT);

	double magNorm = sqrt((fm[SensorsEvent::XAXIS]*fm[SensorsEvent::XAXIS]) + (fm[SensorsEvent::YAXIS]*fm[SensorsEvent::YAXIS]) + (fm[SensorsEvent::ZAXIS]*fm[SensorsEvent::ZAXIS]));

	fm[SensorsEvent::XAXIS] /= magNorm;
	fm[SensorsEvent::YAXIS] /= magNorm;
	fm[SensorsEvent::ZAXIS] /= magNorm;

        _rpyEstimate->roll[SensorsEvent::MAGNETOMETER]   = 0.0f; // no contribution for roll
        _rpyEstimate->pitch[SensorsEvent::MAGNETOMETER]  = 0.0f; // no contribution for pitch
	// TODO check this equations for compensated / uncompensated yaw
         _rpyEstimate->yaw[SensorsEvent::MAGNETOMETER]   = TO_DEG(atan2((fm[SensorsEvent::YAXIS]*cos(_rpyEstimate->roll[SensorsEvent::ACCELEROMETER]))-
									 (fm[SensorsEvent::ZAXIS]*sin(_rpyEstimate->roll[SensorsEvent::ACCELEROMETER])),
									 (fm[SensorsEvent::XAXIS]*cos(_rpyEstimate->pitch[SensorsEvent::ACCELEROMETER]))+
									 (fm[SensorsEvent::YAXIS]*sin(_rpyEstimate->roll[SensorsEvent::ACCELEROMETER])*sin(_rpyEstimate->pitch[SensorsEvent::ACCELEROMETER]))-
									 (fm[SensorsEvent::ZAXIS]*cos(_rpyEstimate->roll[SensorsEvent::ACCELEROMETER])*sin(_rpyEstimate->pitch[SensorsEvent::ACCELEROMETER])))) - 90.0;
	 //printf("RPYg = [%f %f %f]\n", _rpyEstimate->roll[SensorsEvent::GYROSCOPE],  _rpyEstimate->pitch[SensorsEvent::GYROSCOPE], _rpyEstimate->yaw[SensorsEvent::GYROSCOPE]);
	 //printf("RPYa = [%f %f %f]\n", _rpyEstimate->roll[SensorsEvent::ACCELEROMETER], _rpyEstimate->pitch[SensorsEvent::ACCELEROMETER], _rpyEstimate->yaw[SensorsEvent::ACCELEROMETER]);
	//printf("RPYm = [%f %f %f]\n", _rpyEstimate->roll[SensorsEvent::MAGNETOMETER], _rpyEstimate->pitch[SensorsEvent::MAGNETOMETER] , _rpyEstimate->yaw[SensorsEvent::MAGNETOMETER]);
}

}
