#include "Knob.h"

const CreatorImpl<Knob> Knob::creator("Knob");
const ParameterTypes Knob::parameterInfo_ = { { "value", { 0, Types::SAMPLE } } };
const PortNames Knob::outputInfo_ = {"value"};
const PortNames Knob::inputInfo_ = {};
Knob::Knob(int maxPoly) : PatchModule (maxPoly) {
    ItilializeVoices(O::OMAX, 0);
    value_ = 0;
    isBuffered_ = false;
    parameters_ = new void*[parameterInfo_.size()];
    parameters_[0] = &value_;
}

void Knob::FillBuffers() {
    output_[0][O::VALUE][0] = value_; //Only 1 voice, 1 sample.
}