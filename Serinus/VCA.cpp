#include "VCA.h"
#include <math.h> 

const CreatorImpl<VCA> VCA::creator("VCA");

VCA::VCA(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O_VCA::MAX, I_VCA::MAX);
    parameters_ = new void*[P_VCA::MAX];
    parameters_[P_VCA::MODE] = &isLinear_;
    isLinear_ = true;
}

void VCA::FillBuffers(int voice, int bufferSize) {
    Sample* gainbuf = input_[voice][I_VCA::GAIN][0];
    Sample* inbuf = input_[voice][I_VCA::INPUT][0];
    for (int i = 0; i < bufferSize; ++i) {
        Sample  gain    = *(gainbuf+i);
        Sample  input   = *(inbuf+i);
        Sample* output  = &output_[voice][O_VCA::SAMPLE][i];
        if (isLinear_) {
            *output = input*gain;
        } else {
            //TODO: fix log with RAMP input (produces some nasty clicks)
            *output = input*log(gain);
        }   
    }
}

ModuleTypes VCA::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_VCA::MODE, Types::BOOL),
    };
    return map;
}