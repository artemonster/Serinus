#include "Knob.h"

const CreatorImpl<Knob> Knob::creator("Knob");

Knob::Knob(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O_Knob::MAX, 0);
    value_ = 0;
    parameters_ = new void*[P_Knob::MAX];
    parameters_[P_Knob::VALUE] = &value_;
}

inline void Knob::Tick(int voice, int bufIndex) {
    output_[voice][O_Knob::VALUE][bufIndex] = value_;
};

ModuleTypes Knob::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_Knob::VALUE, Types::INT),
    };
    return map;
};