#include "ADSR.h"

const CreatorImpl<ADSR> ADSR::creator("ADSR");

ADSR::ADSR() {
    output = new Sample[O_ADSR::MAX];
    output[O_ADSR::SAMPLE] = 0;
    input = new Sample*[I_ADSR::MAX];
    parameters = new void*[P_ADSR::MAX];
    parameters[P_ADSR::MODE] = &isLinear;
}

void ADSR::Tick() {

}

ModuleTypes ADSR::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_ADSR::MODE, Types::BOOL),
    };
    return map;
}
