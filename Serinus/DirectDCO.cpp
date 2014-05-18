#include "DirectDCO.h"
#include <cmath>
const CreatorImpl<DirectDCO> DirectDCO::creator("DirectDCO");

DirectDCO::DirectDCO() {

    output = new Sample[O_DirectDCO::MAX];
    output[O_DirectDCO::SAMPLE] = 0;
    input = new Sample*[I_DirectDCO::MAX];
    parameters = new void*[P_DirectDCO::MAX];
    parameters[0] = &tuneFreq;
    parameters[1] = &detune;
    parameters[2] = &phasor;
    parameters[3] = &reTrigger;
    parameters[4] = &waveform;
}

void DirectDCO::Tick() {
    float amplitude = static_cast<float>( *input[I_DirectDCO::AMP] );
    //float pwm = static_cast<float>(*input[I_DirectDCO::PWM]) / UPSCALE;
    float pwm = 0;
    switch (waveform) {
        case SAW:
            output[O_DirectDCO::SAMPLE] = phasor * amplitude;
            break;
        case TRI:
            if (phasor <= 0) {
                output[O_DirectDCO::SAMPLE] = ( 2 * phasor + 1 ) * amplitude;
            } else {
                output[O_DirectDCO::SAMPLE] = ( -2 * phasor + 1 ) * amplitude;
            }
            break;
        case SIN:
            output[O_DirectDCO::SAMPLE] = sin((phasor+1)*3.14159265)*UPSCALE;
            break;
        case SQR:
            if (phasor >= pwm) {
                output[O_DirectDCO::SAMPLE] = amplitude;
            } else {
                output[O_DirectDCO::SAMPLE] = -amplitude;
            }
            break;
        default:
            break;
    }
    phasor += static_cast<float>( *input[I_DirectDCO::PITCH] ) * 2 / SAMPLE_RATE;
    if (phasor >= 1) phasor -= 2;
}

ModuleTypes DirectDCO::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_DirectDCO::TUNE, Types::FLOAT),
        std::make_pair(P_DirectDCO::DETUNE, Types::FLOAT),
        std::make_pair(P_DirectDCO::PHASOR, Types::FLOAT),
        std::make_pair(P_DirectDCO::TRIG, Types::BOOL),
        std::make_pair(P_DirectDCO::WF, Types::INT),
    };
    return map;
};
