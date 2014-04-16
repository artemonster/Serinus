/*
 * SawDCO.cpp
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#include "SawDCO.h"

const CreatorImpl<SawDCO> SawDCO::creator("SawDCO");
static mem_ptr members[3] = { &SawDCO::phase, &SawDCO::frequency, &SawDCO::amplitude };

SawDCO::SawDCO() {
	frequency=220;
	phase=0.0;
    amplitude = 1.0;
}

void SawDCO::Tick() {
    outSample = amplitude*phase*UPSCALE;
	phase += frequency*2/SAMPLE_RATE;
    if(phase >= 1.0f) phase -= 2.0f;
}

void SawDCO::setFrequency(float inFreq) {
	frequency=inFreq;
}

void SawDCO::modulate(int memberIndex, Sample inValue) {
    this->*(members[memberIndex]) *= inValue;
}
