#include "PolyMixer.h"

const CreatorImpl<PolyMixer> PolyMixer::creator("PolyMixer");
const ParameterTypes PolyMixer::parameterInfo_ = {
            { "clamped", { 0,Types::BOOL } } };
const PortNames PolyMixer::outputInfo_ = {"sample"};
const PortNames PolyMixer::inputInfo_ = {"in"};

PolyMixer::PolyMixer(int maxPoly) : PatchModule (maxPoly) {
    ItilializeVoices(O::OMAX, I::IMAX);
    outputSample_ = new float[kMaxPoly];
    for (int i = 0; i < kMaxPoly; ++i) {
        outputSample_[i] = 0;
    } 
    voiceCount_ = maxPoly;
    parameters_ = new void*[parameterInfo_.size()];
    parameters_[0] = &clampedMix_;
    clampedMix_ = true;
}

void PolyMixer::FillBuffers() {
    for (int bufIndex = 0; bufIndex < kBufferSize; ++bufIndex) {
        Sample outSample = 0;
        if (clampedMix_) {
            for (int voice = 0; voice < voiceCount_; ++voice) {
                outSample += *input_[voice][I::POLYSAMPLE][bufIndex];
            }
            outSample *= 0.707946f; //-3dB
            if (outSample >= 1.0f) outSample = 1.0f;
            if (outSample <= -1.0f) outSample = -1.0f;
        } else {
            Sample scaleFactor = static_cast<Sample>( 1.0/voiceCount_ );
            for (int voice = 0; voice < voiceCount_; ++voice) {
                outSample += *input_[voice][I::POLYSAMPLE][bufIndex]*scaleFactor;
            }
        }
        output_[0][O::SAMPLE][bufIndex] = outSample;
    }
}