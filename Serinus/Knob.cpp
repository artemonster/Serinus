#include "Knob.h"

const CreatorImpl<Knob> Knob::creator("Knob");

Knob::Knob() {
    value = 0;
    output = new Sample[O_Knob::MAX];
    input = NULL;
    parameters = new void*[P_Knob::MAX];
    parameters[0] = &value;
}

inline void Knob::Tick() {
    output[O_Knob::VALUE] = value;
};

inline void Knob::setValue(Sample val) {
    value = val;
}