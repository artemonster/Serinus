#ifndef SUBPATCH_H_
#define SUBPATCH_H_
#include "PatchModule.h"
/**
This is a subpatch module implementation.

Authored: AK
Last revision: 29.05.2014
*/

class SubPatch : public PatchModule {
public:
    static const CreatorImpl<SubPatch> creator;    
    SubPatch(int maxPoly, int bufferSize);
    ~SubPatch() {};
    void FillBuffers();
    void ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal) {};
    ParameterTypes getParameterInfo() { return parameterInfo_; }
    PortNames getOutputsInfo() { return outputInfo_; }
    PortNames getInputsInfo() { return inputInfo_; }
    std::vector<PatchModule*> getContainer() { return loadedPatch_; }
private:
    static const ParameterTypes parameterInfo_;
    static const PortNames outputInfo_;
    static const PortNames inputInfo_;
    std::vector<PatchModule*> loadedPatch_;
};
#endif /* SUBPATCH_H_ */

