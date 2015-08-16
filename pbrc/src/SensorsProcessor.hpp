#include <QObject>
#include <cmath>
#include <iostream>
#include "Datatypes.hpp"

namespace nst {

#define IN_WEIGHT 0.5

// forward declarations
struct SensorsEvent;

/**
 * SensorProcessor - Use single sensory samples to compute robot RPY.
 */
class SensorsProcessor : public QObject
{
        Q_OBJECT

public:
         SensorsProcessor(QObject *parent = 0);
        ~SensorsProcessor();

public slots:
        void newSample(const SensorsEvent *ev);

private:
	RPYEvent * _rpyEstimate; // estimates
	double *fa, *fm;	 // filtered data
};


} // nst::
