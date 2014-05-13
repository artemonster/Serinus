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
    Knob();
    ~Knob() {};
    inline void Tick();
    ModuleTypes getParameterTypes();
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
private:
    Sample value;
};

#endif /* KNOB_H_ */