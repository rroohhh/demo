#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

#include "../simulation/Simtypes.h"

class Integrator {
public:
	virtual Simtypes::FLOAT step(Simtypes::FLOAT simTime) = 0;
	virtual ~Integrator() {};
};

#endif