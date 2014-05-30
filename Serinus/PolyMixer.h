#ifndef POLYMIXER_H_
#define POLYMIXER_H_
#include "PatchModule.h"
class PolyMixer : public PatchModule {
public:
    static const CreatorImpl<PolyMixer> creator;    
    PolyMixer(int maxPoly);
    ~PolyMixer() {};
    void FillBuffers();
    bool isMonophonic() { return true; }
    void ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal) {};
    ParameterTypes getParameterInfo() { return parameterInfo_; }
    PortNames getOutputsInfo() { return outputInfo_; }
    PortNames getInputsInfo() { return inputInfo_; }
private:
    static const ParameterTypes parameterInfo_;
    static const PortNames outputInfo_;
    static const PortNames inputInfo_;
    enum I { POLYSAMPLE, IMAX };
    enum O { SAMPLE, OMAX };
    bool clampedMix_;
    Sample* outputSample_;
};
#endif /* POLYMIXER_H_ */
