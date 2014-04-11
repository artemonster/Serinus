/*
 * SawDCO.cpp
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#include "SawDCO.h"

SawDCO::SawDCO() {
	frequency=220;
	phase=0.0;
}

void SawDCO::Tick() {
	outSample = phase*INT32_UPSCALE;
	phase += frequency*2/SAMPLE_RATE;
    if(phase >= 1.0f) phase -= 2.0f;
}

void SawDCO::setFrequency(float inFreq) {
	frequency=inFreq;
}
