#include "ADSR.h"

const CreatorImpl<ADSR> ADSR::creator("ADSR");
const ParameterTypes ADSR::parameterInfo_ = {
            { "mode", { 0,Types::BOOL } },
            { "attack", { 1,Types::FLOAT } },
            { "decay", { 2,Types::FLOAT } },
            { "sustain", { 3,Types::FLOAT } },
            { "release", { 4,Types::FLOAT } } };
const PortNames ADSR::outputInfo_ = {"sample"};
const PortNames ADSR::inputInfo_ = {"gate"};

ADSR::ADSR(int maxPoly, int bufferSize) : PatchModule (maxPoly, bufferSize) {
    ItilializeVoices(O::OMAX, I::IMAX);
    outputSample_ = new float[maxPoly];
    state_ = new State[maxPoly];
    keyPressed_ = new bool[maxPoly];
    for (int i = 0; i < maxPoly; ++i) {
        outputSample_[i] = 0;
        state_[i] = IDLE;
        keyPressed_[i] = false;
    } 
    parameters_ = new void*[parameterInfo_.size()];
    parameters_[0] = &isLinear_;
    parameters_[1] = &attack_;
    parameters_[2] = &decay_;
    parameters_[3] = &sustain_;
    parameters_[4] = &release_;
    
    isLinear_ = true;
}

void ADSR::FillBuffers(int voice, int bufferSize) {
    Sample* gatebuf = input_[voice][I::GATE];
    for (int i = 0; i < bufferSize; ++i) {
        Sample gate = *(gatebuf + i);
        if (gate >= 0.5f && keyPressed_[voice]==false) {
            keyPressed_[voice] = true;
            state_[voice]      = ATTACK;
            sustainLevel_      = sustain_;
            attackRate_        = 1.0f / (kSampleRate * (attack_ / 1000));
            decayRate_         = (1.0f - sustainLevel_ ) / (kSampleRate * (decay_ / 1000));
            releaseRate_       = sustainLevel_ / (kSampleRate * (release_ / 1000));
        } 
        if (gate <= 0.5f && keyPressed_[voice]==true) {
            keyPressed_[voice] = false;
            state_[voice]      = RELEASE;
            releaseRate_       = outputSample_[voice] / (kSampleRate * (release_ / 1000));
        }

        switch (state_[voice]) {
            case ATTACK:
                outputSample_[voice] += attackRate_;
                if (outputSample_[voice] >= 1.0f) {
                    outputSample_[voice] = 1.0f; //prevent overshoot
                    state_[voice] = DECAY;
                }
                break;
            case DECAY:
                outputSample_[voice] -= decayRate_;
                if (outputSample_[voice] <= sustainLevel_) {
                    outputSample_[voice] = sustainLevel_;
                    state_[voice] = SUSTAIN;
                }
                break;
            case SUSTAIN:
                outputSample_[voice] = sustainLevel_;
                break;
            case RELEASE:
                outputSample_[voice] -= releaseRate_;
                if (outputSample_[voice] <= 0.0f) {
                    outputSample_[voice] = 0.0f; //prevent undershoot
                    state_[voice] = IDLE;
                }
                break;
            case IDLE:
                outputSample_[voice] = 0.0f;
                break;
            default: break;
        }
        output_[voice][O::SAMPLE][i] = outputSample_[voice];  
    }
}