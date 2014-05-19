#include "ADSR.h"

const CreatorImpl<ADSR> ADSR::creator("ADSR");

ADSR::ADSR() {
    output_ = new Sample[O_ADSR::MAX];
    output_[O_ADSR::SAMPLE] = 0;
    state_ = IDLE;
    isLinear_ = true;
    input_ = new Sample*[I_ADSR::MAX];
    parameters_ = new void*[P_ADSR::MAX];
    parameters_[P_ADSR::MODE] = &isLinear_;
    parameters_[P_ADSR::ATTACK] = &attack_;
    parameters_[P_ADSR::DECAY] = &decay_;
    parameters_[P_ADSR::SUSTAIN] = &sustain_;
    parameters_[P_ADSR::RELEASE] = &release_;
}

void ADSR::Tick() {
    if (*input_[I_ADSR::GATE] >= 0.5*UPSCALE) {
        switch (state_) {
            case ATTACK:
            case DECAY:
            case SUSTAIN:
                state_ = RELEASE;
                break;
            case RELEASE:
            case IDLE:
                state_ = ATTACK;
                break;
            default: break;
        }
    } else {
        switch (state_) {
            case ATTACK:
                //check if reached sustain level
            case DECAY:
            case SUSTAIN:
                state_ = RELEASE;
                break;
            case RELEASE:
                break;
            case IDLE:
                break;
            default: break;
        }
    }
}

ModuleTypes ADSR::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_ADSR::MODE, Types::BOOL),
        std::make_pair(P_ADSR::ATTACK, Types::FLOAT),
        std::make_pair(P_ADSR::DECAY, Types::FLOAT),
        std::make_pair(P_ADSR::SUSTAIN, Types::FLOAT),
        std::make_pair(P_ADSR::RELEASE, Types::FLOAT)
    };
    return map;
}
