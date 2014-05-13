#include "VCA.h"
#include <math.h> 
const CreatorImpl<VCA> VCA::creator("VCA");
VCA::VCA() {
    output = new Sample[O_VCA::MAX];
    output[O_VCA::SAMPLE] = 0;
    input = new Sample*[I_VCA::MAX];
    parameters = new void*[P_VCA::MAX];
    parameters[P_VCA::MODE] = &isLinear;
}

void VCA::Tick() {
    float gain = static_cast<float>( *input[I_VCA::GAIN] )/ UPSCALE;
    Sample in = *input[I_VCA::INPUT];
    if (isLinear) {
        output[O_VCA::SAMPLE] = in*gain;
    } else {
        //TODO: fix log with RAMP input (produces some nasty clicks)
        output[O_VCA::SAMPLE] = in*log(gain);
    }     
}

ModuleTypes VCA::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_VCA::MODE, Types::BOOL),
    };
    return map;
}