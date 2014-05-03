#ifndef KNOB_H_
#define KNOB_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"

class Knob : public PatchModule {
public:
    static const CreatorImpl<Knob> creator;
    inline Knob() {
        value = 1.0;
        output = new Sample[O_Knob::MAX + 1];
        input = NULL;
    };

    ~Knob() {};

    inline void Tick() {
        output[O_Knob::VALUE] = value;
    };

    inline void setValue(InternalVal val) {
        value = val;
    }

    void ProcessCommand(int commandType, int commandIndex, int inValue) {};
    void FoldInputsToInternals() {};
private:
    InternalVal value;
};

const CreatorImpl<Knob> Knob::creator("Knob");
#endif /* KNOB_H_ */