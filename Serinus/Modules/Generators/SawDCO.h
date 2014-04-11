/*
 * SawDCO.h
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#ifndef SAWDCO_H_
#define SAWDCO_H_
#include "..\PatchModule.h"

class SawDCO : public PatchModule  {
public:
	SawDCO();
	~SawDCO() {};
	void Tick();
	void setFrequency(float inFreq);
private:
	float phase;
	float frequency;
};

#endif /* SAWDCO_H_ */
