#ifndef DIRECTDCO_H_
#define DIRECTDCO_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"
/**
This is a basic oscillator. It can provide 4 waveforms (saw, triangle, sine and square wave).
Many features are undecided yet.
PWM is in UPSCALE*percentage
TODO: decide, whether to provide 4 outputs simulatenously or use a switch-case.
Introduce noise, maybe? 
How detune will be handled? Phase modulation?

Authored: AK
Last revision: 13.05.2014
*/
class DirectDCO : public PatchModule {
public:
    static const CreatorImpl<DirectDCO> creator;

    DirectDCO(int maxPoly, int bufferSize);
    ~DirectDCO() {};
    inline void Tick(int voice, int bufIndex);
    void FillBuffers(int voice, int bufferSize);
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
    ModuleTypes getParameterTypes();
private:
    enum WaveForm { SAW, TRI, SIN, SQR };
    float tuneFreq_;
    float detune_;
    float *phasor_;
    bool reTrigger_;
    int waveform_;
    int mode_;
};

#endif /* DIRECTDCO_H_ */
