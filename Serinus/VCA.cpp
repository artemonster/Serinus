#include "VCA.h"
#include <math.h> 
const CreatorImpl<VCA> VCA::creator("VCA");
VCA::VCA() {
    output_ = new Sample[O_VCA::MAX];
    output_[O_VCA::SAMPLE] = 0;
    input_ = new Sample*[I_VCA::MAX];
    parameters_ = new void*[P_VCA::MAX];
    parameters_[P_VCA::MODE] = &isLinear_;
}

void VCA::Tick() {
    float gain = static_cast<float>( *input_[I_VCA::GAIN] )/ UPSCALE;
    Sample in = *input_[I_VCA::INPUT];
    if (isLinear_) {
        output_[O_VCA::SAMPLE] = in*gain;
    } else {
        //TODO: fix log with RAMP input (produces some nasty clicks)
        output_[O_VCA::SAMPLE] = in*log(gain);
    }     
}

ModuleTypes VCA::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_VCA::MODE, Types::BOOL),
    };
    return map;
}