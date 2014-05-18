#ifndef DIRECTDCO_H_
#define DIRECTDCO_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"
/**
This is a basic oscillator. It can provide 4 waveforms (saw, triangle, sine and square wave).
Many features are undecided yet.
TODO: decide, whether to provide 4 outputs simulatenously or use a switch-case.
Introduce noise, maybe? 
How detune will be handled? Phase modulation?

Authored: AK
Last revision: 13.05.2014
*/
class DirectDCO : public PatchModule {
public:
    static const CreatorImpl<DirectDCO> creator;

    DirectDCO();
    ~DirectDCO() {};
    void Tick();
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
    ModuleTypes getParameterTypes();
private:
    enum WaveForm { SAW, TRI, SIN, SQR };
    //Internal params
    float tuneFreq;
    float detune;
    float phasor;
    bool reTrigger;
    int waveform;
};

#endif /* DIRECTDCO_H_ */