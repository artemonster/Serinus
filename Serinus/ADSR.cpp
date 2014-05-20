#include "ADSR.h"

const CreatorImpl<ADSR> ADSR::creator("ADSR");

ADSR::ADSR() {
    output_ = new Sample[O_ADSR::MAX];
    output_[O_ADSR::SAMPLE] = 0;
    outputSample_ = 0;
    state_ = IDLE;
    keyPressed = false;
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
    if (*input_[I_ADSR::GATE] >= 0.5*UPSCALE && keyPressed==false) {
        keyPressed = true;
        state_ = ATTACK;
        sustainLevel_ = sustain_ / 100;
        attackRate_ = 1.0 / (SAMPLE_RATE * (attack_ / 1000));
        decayRate_ =  (1.0 - sustainLevel_ ) / (SAMPLE_RATE * (decay_ / 1000));
        releaseRate_ =  sustainLevel_ / (SAMPLE_RATE * (release_ / 1000));
    } 
    if (*input_[I_ADSR::GATE] <= 0.5*UPSCALE && keyPressed==true) {
        keyPressed = false;
        state_ = RELEASE;
        releaseRate_ =  outputSample_ / (SAMPLE_RATE * (release_ / 1000));
    }

    switch (state_) {
        case ATTACK:
            outputSample_ += attackRate_;
            if (outputSample_ >= 1.0) {
                outputSample_ = 1.0; //prevent overshoot
                state_ = DECAY;
            }
            break;
        case DECAY:
            outputSample_ -= decayRate_;
            if (outputSample_ <= sustainLevel_) {
                outputSample_ = sustainLevel_;
                state_ = SUSTAIN;
            }
            break;
        case SUSTAIN:
            outputSample_ = sustainLevel_;
            break;
        case RELEASE:
            outputSample_ -= releaseRate_;
            if (outputSample_ <= 0) {
                outputSample_ = 0; //prevent undershoot
                state_ = IDLE;
            }
            break;
        case IDLE:
            outputSample_ = 0;
            break;
        default: break;
    }

    output_[O_ADSR::SAMPLE] = static_cast<int>(outputSample_*(UPSCALE-100));  
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
