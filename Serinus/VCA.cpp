#include "VCA.h"
#include <math.h> 

const CreatorImpl<VCA> VCA::creator("VCA");
const ParameterTypes VCA::parameterInfo_ = { { "mode", { 0, Types::BOOL } } };
const PortNames VCA::outputInfo_ = {"sample"};
const PortNames VCA::inputInfo_ = {"in","gain"};

VCA::VCA(int maxPoly) : PatchModule (maxPoly) {
    ItilializeVoices(O::OMAX, I::IMAX);
    parameters_ = new void*[parameterInfo_.size()];
    parameters_[0] = &isLinear_;
    isLinear_ = true;
}

void VCA::FillBuffers() {
    for (int voice = 0; voice < voiceCount_; ++voice) {
        for (int i = 0; i < kBufferSize; ++i) {
            Sample  gain    = *input_[voice][I::GAIN][i];
            Sample  input   = *input_[voice][I::INPUT][i];
            Sample* output  = &output_[voice][O::SAMPLE][i];
            if (gain < 0.0f) gain = 0.0f;
            if (isLinear_) {
                *output = input*gain;
            } else {
                //TODO: fix log with RAMP input (produces some nasty clicks)
                *output = input*log(gain);
            }   
        }
    }
}