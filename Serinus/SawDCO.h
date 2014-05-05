#ifndef SAWDCO_H_
#define SAWDCO_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"

class SawDCO : public PatchModule  {
public:
    static const CreatorImpl<SawDCO> creator;
	
	SawDCO(); 
	~SawDCO() {};
	void Tick();
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
	ModuleTypes getParameterTypes();
private: 
	//Modulation inputs
	Sample pitch;
	Sample pwm;
	Sample phase;
	Sample pan;

	//Internal params
	float tuneFreq;
	float detune;
	float phasor;
	bool reTrigger;
	int waveform;
};

#endif /* SAWDCO_H_ */
