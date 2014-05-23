#include "VCA.h"
#include <math.h> 
const CreatorImpl<VCA> VCA::creator("VCA");
VCA::VCA(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O_VCA::MAX, I_VCA::MAX);
    parameters_ = new void*[P_VCA::MAX];
    parameters_[P_VCA::MODE] = &isLinear_;
}

void VCA::Tick(int voice, int bufIndex) {
    Sample* gainbuf = input_[voice][I_VCA::GAIN][0];
    Sample* inbuf = input_[voice][I_VCA::INPUT][0];

    float gain = static_cast<float>( *(gainbuf+bufIndex) )/ UPSCALE;
    Sample* output = &output_[voice][O_VCA::SAMPLE][bufIndex];
    if (isLinear_) {
        *output = *(inbuf+bufIndex)*gain;
    } else {
        //TODO: fix log with RAMP input (produces some nasty clicks)
        *output = *(inbuf+bufIndex)*log(gain);
    }     
}

ModuleTypes VCA::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_VCA::MODE, Types::BOOL),
    };
    return map;
}