#include "Knob.h"

const CreatorImpl<Knob> Knob::creator("Knob");

Knob::Knob() {
    value = 1.0;
    output = new Sample[O_Knob::MAX + 1];
    input = NULL;
}

inline void Knob::Tick() {
    output[O_Knob::VALUE] = value;
};

inline void Knob::setValue(InternalVal val) {
    value = val;
}