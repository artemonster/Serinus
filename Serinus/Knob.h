#ifndef KNOB_H_
#define KNOB_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"

class Knob : public PatchModule {
public:
    static const CreatorImpl<Knob> creator;
    Knob();
    ~Knob() {};
    inline void Tick();
    inline void setValue(InternalVal val);
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
private:
    InternalVal value;
};

#endif /* KNOB_H_ */