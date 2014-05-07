#ifndef WAVETABLEOSC_H_
#define WAVETABLEOSC_H_

#include "PatchModule.h"

class WaveTableOsc : public PatchModule {
public:
    static const CreatorImpl<WaveTableOsc> creator;
    WaveTableOsc();
    ~WaveTableOsc() {};
    void Tick();
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
    ModuleTypes getParameterTypes() {
        ModuleTypes map {
            std::make_pair(0, Types::FLOAT),
        };
        return map;
    };
    void setFrequency(float inc);
    void setPhaseOffset(float offset);
    void loadWaveTable(int tableLen, float *input);

protected:
    int tableLength;
    float* sampleTable;

    bool willInterpolate;
    float phasor;
    float phasorInc;
};

inline void WaveTableOsc::setFrequency(float inFreq) {
    phasorInc = inFreq / SAMPLE_RATE;
}


#endif
