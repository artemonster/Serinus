/*
 * SawDCO.cpp
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#include "SawDCO.h"
#include "PatchModuleConfigs.h"
const CreatorImpl<SawDCO> SawDCO::creator("SawDCO");

SawDCO::SawDCO() {
    output = new Sample[O_SawDCO::MAX + 1];
    input = new Sample*[I_SawDCO::MAX + 1];
	phase=0.0;
}

void SawDCO::Tick() {
    output[O_SawDCO::SAMPLE] = (Sample)*input[I_SawDCO::AMP] * phase * UPSCALE;
    phase += *input[I_SawDCO::FREQ] * 2 / SAMPLE_RATE;
    if (phase >= 1.0f) phase -= 2.0f;
}
