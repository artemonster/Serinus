#ifndef POLYKEYS_H_
#define POLYKEYS_H_
#include "PatchModule.h"
/**
PolyKeys module receives midi NoteOn and NoteOff events and translates them to control voltage, gate and velocity
samples.

Authored: AK
Last revision: 29.05.2014
*/
class PolyKeys : public PatchModule {
public:
    static const CreatorImpl<PolyKeys> creator;
    PolyKeys(int maxPoly, int bufferSize);
    ~PolyKeys() {};
    void FillBuffers();
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
