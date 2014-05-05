#include "SawDCO.h"

const CreatorImpl<SawDCO> SawDCO::creator("SawDCO");

SawDCO::SawDCO() {

    output = new Sample[O_SawDCO::MAX];
    output[O_SawDCO::SAMPLE] = 0;
    input = new Sample*[I_SawDCO::MAX];
	parameters = new void*[P_SawDCO::MAX];
	parameters[0] = &tuneFreq;
	parameters[1] = &detune;
	parameters[2] = &phasor;
	parameters[3] = &reTrigger;
	parameters[4] = &waveform;
}

void SawDCO::Tick() {
    output[O_SawDCO::SAMPLE] = (*input[I_SawDCO::AMP] / UPSCALE) * phase;
    phase += *input[I_SawDCO::FREQ] * 2 / SAMPLE_RATE;
    if (phase >= 1 * UPSCALE) phase -= 2 * UPSCALE;
}

ModuleTypes SawDCO::getParameterTypes() {
	ModuleTypes map{
		std::make_pair(P_SawDCO::TUNE, Types::FLOAT),
		std::make_pair(P_SawDCO::DETUNE, Types::FLOAT),
		std::make_pair(P_SawDCO::PHASOR, Types::FLOAT),
		std::make_pair(P_SawDCO::TRIG, Types::BOOL),
		std::make_pair(P_SawDCO::WF, Types::INT),
	};
	return map;
};