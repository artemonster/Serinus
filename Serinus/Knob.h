#ifndef KNOB_H_
#define KNOB_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"
/**
This is a most basic knob, which holds a sample value and sends it out each tick.
TODO: think on scaling, and event processing (eventual hw mapping)

Authored: AK
Last revision: 13.05.2014
*/
class Knob : public PatchModule {
public:
    static const CreatorImpl<Knob> creator;
    Knob(int maxPoly, int bufferSize);
    ~Knob() {};
    void FillBuffers(int voice, int bufferSize) {};
    inline void Tick(int voice, int bufIndex);
    ModuleTypes getParameterTypes();
    void ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal) {};
private:
    Sample value_;
};

#endif /* KNOB_H_ */