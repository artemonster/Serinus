#include "PolyKeys.h"

const CreatorImpl<PolyKeys> PolyKeys::creator("PolyKeys");

PolyKeys::PolyKeys(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O_PolyKeys::MAX, 0);
    gate_ = new int[maxPoly];
    note_ = new int[maxPoly];
    velocity_ = new int[maxPoly];
    for (int i = 0; i < maxPoly; ++i) {
        gate_[i] = velocity_[i] = 0;
        note_[i] = 60;
    }
}

void PolyKeys::FillBuffers(int voice, int bufferSize) {
    for (int i = 0; i < bufferSize; ++i) {
        PolyKeys::Tick(voice, i);
    }
}

inline void PolyKeys::Tick(int voice, int bufIndex) {
    Sample* gateOut = &output_[voice][O_PolyKeys::GATE][bufIndex];
    Sample* cvOut = &output_[voice][O_PolyKeys::CV][bufIndex];
    Sample* veloOut = &output_[voice][O_PolyKeys::VELO][bufIndex];
    if (gate_[voice] == 1) {
        *gateOut = static_cast<int>( UPSCALE - 100 );
    } else {
        *gateOut = 0;
    }
    *cvOut = static_cast<float>((note_[voice]-60)*100000)/12.0; //Centered around C4
    *veloOut = 0; //TODO fix this!
}

void PolyKeys::ProcessCommand(const int &cmdType, int polyVoiceNr, const MidiCmd &inValue, int &retVal) {
    switch (cmdType) {
        case NOTEON:
            gate_[polyVoiceNr] = 1;
            note_[polyVoiceNr] = inValue.at(1);
            velocity_[polyVoiceNr] = inValue.at(2);
            break;
        case NOTEOFF:
            gate_[polyVoiceNr] = 0;
            velocity_[polyVoiceNr] = inValue.at(2);
            break;
        default: break;
    }
}

ModuleTypes PolyKeys::getParameterTypes() {
    ModuleTypes map;
    return map;
};