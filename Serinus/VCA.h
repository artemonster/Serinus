#ifndef VCA_H_
#define VCA_H_
#include "PatchModule.h"
/**
This is a basic VCA module, which can operate in both linear and logarithmic modes.
Gain input should be inside 0...1 boundaries (negative gain is clipped)

TODO: fix log. mode

Authored: AK
Last revision: 29.05.2014
*/
class VCA : public PatchModule {
public:
    static const CreatorImpl<VCA> creator;
    VCA(int maxPoly);
    ~VCA() {};
    void FillBuffers();
    void ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal) {};
    ParameterTypes getParameterInfo() { return parameterInfo_; }
    PortNames getOutputsInfo() { return outputInfo_; }
    PortNames getInputsInfo() { return inputInfo_; }
private:
    static const ParameterTypes parameterInfo_;
    static const PortNames outputInfo_;
    static const PortNames inputInfo_;
    enum I { INPUT, GAIN, IMAX };
    enum O { SAMPLE, OMAX };
    bool isLinear_;
};
#endif /* VCA_H_ */