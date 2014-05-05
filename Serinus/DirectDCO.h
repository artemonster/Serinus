#ifndef DIRECTDCO_H_
#define DIRECTDCO_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"

class DirectDCO : public PatchModule  {
public:
    static const CreatorImpl<DirectDCO> creator;
	
	DirectDCO(); 
	~DirectDCO() {};
	void Tick();
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
	ModuleTypes getParameterTypes();
private: 
	enum WaveForm { SAW, TRI, SIN, SQR };
	//Modulation inputs
	Sample pitch;
	Sample pwm;
	Sample phase;
	Sample pan;
	Sample amp;
	//Internal params
	float tuneFreq;
	float detune;
	float phasor;
	bool reTrigger;
	int waveform;
};

#endif /* DIRECTDCO_H_ */
