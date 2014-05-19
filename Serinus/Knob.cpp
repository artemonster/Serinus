#include "Knob.h"

const CreatorImpl<Knob> Knob::creator("Knob");

Knob::Knob() {
    value_ = 0;
    output_ = new Sample[O_Knob::MAX];
    input_ = NULL;
    parameters_ = new void*[P_Knob::MAX];
    parameters_[P_Knob::VALUE] = &value_;
}

inline void Knob::Tick() {
    output_[O_Knob::VALUE] = value_;
};

ModuleTypes Knob::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_Knob::VALUE, Types::INT),
    };
    return map;
};