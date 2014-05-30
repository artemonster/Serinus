#ifndef ADSR_H_
#define ADSR_H_
#include "PatchModule.h"
/**
This is a basic ADSR module, which can operate in both linear and exponential modes.
TODO: decide on gate and trigger inputs (whether or not they should be controlled via event or patch samples)
Float parameters are in milliseconds, aside from release (normalized value between 0 and 1).
Authored: AK
Last revision: 29.05.2014
*/

class ADSR : public PatchModule {
public:
    static const CreatorImpl<ADSR> creator;    
    ADSR(int maxPoly);
    ~ADSR() {};
    void FillBuffers();
    void ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal) {};
    ParameterTypes getParameterInfo() { return parameterInfo_; }
    PortNames getOutputsInfo() { return outputInfo_; }
    PortNames getInputsInfo() { return inputInfo_; }
private:
    static const ParameterTypes parameterInfo_;
    static const PortNames outputInfo_;
    static const PortNames inputInfo_;
    enum I { GATE, IMAX };
    enum O { SAMPLE, OMAX };
    enum State { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE };
    float* outputSample_;
    State* state_;
    bool* keyPressed_;

    bool isLinear_;
    float attack_;
    float decay_;
    float sustain_;
    float release_;

    float attackRate_;
    float sustainLevel_;
    float decayRate_;
    float releaseRate_;
};
#endif /* ADSR_H_ */