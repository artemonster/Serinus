#include "DirectDCO.h"
#include <cmath>
const CreatorImpl<DirectDCO> DirectDCO::creator("DirectDCO");

DirectDCO::DirectDCO() {
    output_ = new Sample[O_DirectDCO::MAX];
    output_[O_DirectDCO::SAMPLE] = 0;
    input_ = new Sample*[I_DirectDCO::MAX];
    parameters_ = new void*[P_DirectDCO::MAX];
    parameters_[0] = &tuneFreq_;
    parameters_[1] = &detune_;
    parameters_[2] = &phasor_;
    parameters_[3] = &reTrigger_;
    parameters_[4] = &waveform_;
    parameters_[5] = &mode_;
    mode_ = 0;
}

void DirectDCO::Tick() {
    float amplitude = static_cast<float>( *input_[I_DirectDCO::AMP] );
    //float pwm = static_cast<float>(*input[I_DirectDCO::PWM]) / UPSCALE;
    float pwm = 0;
    switch (waveform_) {
        case SAW:
            output_[O_DirectDCO::SAMPLE] = phasor_ * amplitude;
            break;
        case TRI:
            if (phasor_ <= 0) {
                output_[O_DirectDCO::SAMPLE] = ( 2 * phasor_ + 1 ) * amplitude;
            } else {
                output_[O_DirectDCO::SAMPLE] = ( -2 * phasor_ + 1 ) * amplitude;
            }
            break;
        case SIN:
            output_[O_DirectDCO::SAMPLE] = sin((phasor_+1)*3.14159265)*UPSCALE;
            break;
        case SQR:
            if (phasor_ >= pwm) {
                output_[O_DirectDCO::SAMPLE] = amplitude;
            } else {
                output_[O_DirectDCO::SAMPLE] = -amplitude;
            }
            break;
        default:
            break;
    }
    if (mode_ == 0) {    //Hz
        phasor_ += static_cast<float>( *input_[I_DirectDCO::PITCH] ) * 2 / SAMPLE_RATE;
    } else {            //CV
        phasor_ += tuneFreq_ * pow(2,static_cast<float>(*input_[I_DirectDCO::PITCH])) * 2 / SAMPLE_RATE;
    }  
    if (phasor_ >= 1) phasor_ -= 2;
}

ModuleTypes DirectDCO::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_DirectDCO::TUNE, Types::FLOAT),
        std::make_pair(P_DirectDCO::DETUNE, Types::FLOAT),
        std::make_pair(P_DirectDCO::PHASOR, Types::FLOAT),
        std::make_pair(P_DirectDCO::TRIG, Types::BOOL),
        std::make_pair(P_DirectDCO::WF, Types::INT),
        std::make_pair(P_DirectDCO::MODE, Types::INT)
    };
    return map;
};
