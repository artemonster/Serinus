#include "Knob.h"

const CreatorImpl<Knob> Knob::creator("Knob");
const ParameterTypes Knob::parameterInfo_ = { { "value", { 0, Types::SAMPLE } } };
const PortNames Knob::outputInfo_ = {"value"};
const PortNames Knob::inputInfo_ = {};
Knob::Knob(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O::OMAX, 0);
    value_ = 0;
    parameters_ = new void*[parameterInfo_.size()];
    parameters_[0] = &value_;
}

void Knob::FillBuffers() {
    for (int voice = 0; voice < maxPoly_; ++voice) {
        for (int i = 0; i < bufferSize_; ++i) {
            output_[voice][O::VALUE][i] = value_;
        }
    }
}