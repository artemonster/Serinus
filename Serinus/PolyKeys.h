#ifndef POLYKEYS_H_
#define POLYKEYS_H_
#include "PatchModule.h"
class PolyKeys : public PatchModule {
public:
    static const CreatorImpl<PolyKeys> creator;
    PolyKeys(int maxPoly, int bufferSize);
    ~PolyKeys() {};
    void FillBuffers(int voice, int bufferSize);
    void ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal);
    ParameterTypes getParameterInfo() { return parameterInfo_; }
    PortNames getOutputsInfo() { return outputInfo_; }
    PortNames getInputsInfo() { return inputInfo_; }
private:
    static const ParameterTypes parameterInfo_;
    static const PortNames outputInfo_;
    static const PortNames inputInfo_;
    enum O { CV, GATE, VELO, OMAX };
    int* gate_;
    int* note_;
    int* velocity_;
};
#endif /* POLYKEYS_H_ */
