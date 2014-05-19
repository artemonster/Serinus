#include "WaveTableOsc.h"
#include "PatchModuleConfigs.h"
const CreatorImpl<WaveTableOsc> WaveTableOsc::creator("WaveTableOsc");

WaveTableOsc::WaveTableOsc() {
    phasor = 0.0;
    phasorInc = 0.0;
    parameters_ = new void*[1];
    parameters_[0] = &phasor;
    //todo make these constructor-dependent
    willInterpolate = false;
    tableLength = 4096;
    sampleTable = new float[tableLength];
    output_ = new Sample[O_WaveTableOsc::MAX + 1];
    float inc = 2.0 / tableLength;
    for (int i = 0; i <= tableLength; i++) {
        sampleTable[i] = -1.0 + inc*i;
    }
    this->setFrequency(220.0);
}

void WaveTableOsc::Tick() {
    if (willInterpolate) {
        float properPhase = phasor*tableLength;
        int truncPhase = properPhase;
        float fracPhase = properPhase - truncPhase;
        float s0 = sampleTable[truncPhase++];
        if (truncPhase >= tableLength) truncPhase = 0;
        float s1 = sampleTable[truncPhase];
        output_[O_WaveTableOsc::SAMPLE] = ( s0 + ( s1 - s0 )*fracPhase ) * UPSCALE;
    } else {
        output_[O_WaveTableOsc::SAMPLE] = sampleTable[int(phasor*tableLength)] * UPSCALE;
    }
    //Update phase
    phasor += phasorInc;
    if (phasor >= 1.0)
        phasor -= 1.0;
}