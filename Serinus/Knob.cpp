#include "Knob.h"

const CreatorImpl<Knob> Knob::creator("Knob");

Knob::Knob(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O_Knob::MAX, 0);
    value_ = 0;
    parameters_ = new void*[P_Knob::MAX];
    parameters_[P_Knob::VALUE] = &value_;
    ModuleTypes map {
        std::make_pair(P_Knob::VALUE, Types::FLOAT),
    };
    parameterInfo_ = map;
}

void Knob::FillBuffers(int voice, int bufferSize) {
    for (int i = 0; i < bufferSize; ++i) {
        output_[voice][O_Knob::VALUE][i] = value_;
    }
}