#ifndef KNOB_H_
#define KNOB_H_
#include "PatchModule.h"
/**
This is a most basic knob, which holds a sample value and sends it out each tick.

Authored: AK
Last revision: 13.05.2014
*/
class Knob : public PatchModule {
public:
    static const CreatorImpl<Knob> creator;
    Knob(int maxPoly, int bufferSize);
    ~Knob() {};
    void FillBuffers();
    void ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal) {};
    ParameterTypes getParameterInfo() { return parameterInfo_; }
    PortNames getOutputsInfo() { return outputInfo_; }
    PortNames getInputsInfo() { return inputInfo_; }
private:
    static const ParameterTypes parameterInfo_;
    static const PortNames outputInfo_;
    static const PortNames inputInfo_;
    enum O { VALUE, OMAX };
    Sample value_;
};

#endif /* KNOB_H_ */