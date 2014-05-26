#ifndef POLYKEYS_H_
#define POLYKEYS_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"
class PolyKeys : public PatchModule {
public:
    static const CreatorImpl<PolyKeys> creator;
    PolyKeys(int maxPoly, int bufferSize);
    ~PolyKeys() {};
    void FillBuffers(int voice, int bufferSize);
    void ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal);
    ModuleTypes getParameterTypes();
protected:
    int* gate_;
    int* note_;
    int* velocity_;
};
#endif /* POLYKEYS_H_ */
