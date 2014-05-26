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
        Sample* gateOut = &output_[voice][O_PolyKeys::GATE][i];
        Sample* cvOut   = &output_[voice][O_PolyKeys::CV][i];
        Sample* veloOut = &output_[voice][O_PolyKeys::VELO][i];
        if (gate_[voice] == 1) {
            *gateOut = 1.0f;
        } else {
            *gateOut = 0.0f;
        }
        *cvOut = static_cast<float>((note_[voice]-60))/12.0f; //Centered around C4
        *veloOut = 0; //TODO fix this!
    }
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