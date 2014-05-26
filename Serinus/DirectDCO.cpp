#include "DirectDCO.h"
#include <cmath>
const CreatorImpl<DirectDCO> DirectDCO::creator("DirectDCO");

DirectDCO::DirectDCO(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O_DirectDCO::MAX, I_DirectDCO::MAX);
    phasor_ = new float[maxPoly];
    for (int i = 0; i < maxPoly; ++i) {
        phasor_[i] = 0.0;
    }
    parameters_ = new void*[P_DirectDCO::MAX];
    parameters_[0] = &tuneFreq_;
    parameters_[1] = &detune_;
    parameters_[2] = &reTrigger_;
    parameters_[3] = &waveform_;
    parameters_[4] = &mode_;
    mode_ = 0;
}

void DirectDCO::FillBuffers(int voice, int bufferSize) {
    Sample* ampbuf   = input_[voice][I_DirectDCO::AMP][0];
    Sample* pwmbuf   = input_[voice][I_DirectDCO::PWM][0];
    Sample* pitchbuf = input_[voice][I_DirectDCO::PITCH][0];
    for (int i = 0; i < bufferSize; ++i) {
        Sample  amplitude = *(ampbuf+i);
        Sample  pwm       = *(pwmbuf+i);
        Sample  pitch     = *(pitchbuf+i);
        Sample* output    = &output_[voice][O_DirectDCO::SAMPLE][i];
        switch (waveform_) {
            case SAW:
                *output = phasor_[voice] * amplitude;
                break;
            case TRI:
                if (phasor_[voice] <= 0.0f) {
                    *output = ( 2 * phasor_[voice] + 1.0f ) * amplitude;
                } else {
                    *output = ( -2 * phasor_[voice] + 1.0f ) * amplitude;
                }
                break;
            case SIN:
                *output = sin((phasor_[voice])*3.14159265f)*amplitude;
                break;
            case SQR:
                if ((phasor_[voice] + 1.0f) >= pwm*2) {
                    *output = amplitude;
                } else {
                    *output = 0;
                }
                break;
            default:
                break;
        }
        if (mode_ == 0) {   //Hz
            phasor_[voice] += pitch * 2.0f / SAMPLE_RATE;
        } else {            //CV
            phasor_[voice] += tuneFreq_ * static_cast<float>(pow(2, pitch)) * 2.0f / SAMPLE_RATE;
        }  
        if (phasor_[voice] >= 1.0f) phasor_[voice] -= 2.0f;       
    }
}

ModuleTypes DirectDCO::getParameterTypes() {
    ModuleTypes map {
        std::make_pair(P_DirectDCO::TUNE, Types::FLOAT),
        std::make_pair(P_DirectDCO::DETUNE, Types::FLOAT),
        std::make_pair(P_DirectDCO::TRIG, Types::BOOL),
        std::make_pair(P_DirectDCO::WF, Types::INT),
        std::make_pair(P_DirectDCO::MODE, Types::INT)
    };
    return map;
};
