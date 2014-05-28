#include "PolyKeys.h"

const CreatorImpl<PolyKeys> PolyKeys::creator("PolyKeys");
const ParameterTypes PolyKeys::parameterInfo_ = {};
const PortNames PolyKeys::outputInfo_ = {"cv","gate","velo"};
const PortNames PolyKeys::inputInfo_ = {};
PolyKeys::PolyKeys(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O::OMAX, 0);
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
        Sample* gateOut = &output_[voice][O::GATE][i];
        Sample* cvOut   = &output_[voice][O::CV][i];
        Sample* veloOut = &output_[voice][O::VELO][i];
        if (gate_[voice] == 1) {
            *gateOut = 1.0f;
        } else {
            *gateOut = 0.0f;
        }
        *cvOut = static_cast<float>((note_[voice]-60))/12.0f; //Centered around C4
        *veloOut = static_cast<float>(velocity_[voice])/127.0f; //Normalized to 0...1
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
            note_[polyVoiceNr] = inValue.at(1); //unnecessary, actually
            velocity_[polyVoiceNr] = inValue.at(2);
            break;
        default: break;
    }
}