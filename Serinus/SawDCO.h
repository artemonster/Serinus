/*
 * SawDCO.h
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#ifndef SAWDCO_H_
#define SAWDCO_H_
#include "PatchModule.h"

class SawDCO : public PatchModule  {
public:
    static const CreatorImpl<SawDCO> creator;
	SawDCO(); 
	~SawDCO() {};
	void Tick();
	void setFrequency(float inFreq);
    void Modulate(int targetIndex, Sample inValue);
    void ProcessCommand(int commandIndex, int inValue) {};
    //internals! cannot be private because of damn thing...
	float phase;
	float frequency;
    float amplitude;
};

typedef float SawDCO::*mem_ptr;
#endif /* SAWDCO_H_ */
