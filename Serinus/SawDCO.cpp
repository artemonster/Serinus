#include "SawDCO.h"

const CreatorImpl<SawDCO> SawDCO::creator("SawDCO");

SawDCO::SawDCO() {
    phase = 0;
    amplitude = 1;
    freq = 220;

    output = new Sample[O_SawDCO::MAX + 1];
    output[O_SawDCO::SAMPLE] = 0;
    input = new Sample*[I_SawDCO::MAX + 1];
    //fold everything to internals
    input[I_SawDCO::PHASE] = &this->phase;
    input[I_SawDCO::AMP] = &this->amplitude;
    input[I_SawDCO::FREQ] = &this->freq;
}

void SawDCO::Tick() {
    output[O_SawDCO::SAMPLE] = (*input[I_SawDCO::AMP] / UPSCALE) * phase;
    phase += *input[I_SawDCO::FREQ] * 2 / SAMPLE_RATE;
    if (phase >= 1 * UPSCALE) phase -= 2 * UPSCALE;
}