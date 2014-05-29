#include "SubPatch.h"

const CreatorImpl<SubPatch> SubPatch::creator("SubPatch");
const ParameterTypes SubPatch::parameterInfo_ = {};
const PortNames SubPatch::outputInfo_ = {"output"};
const PortNames SubPatch::inputInfo_ = {"input"};

SubPatch::SubPatch(int maxPoly, int bufferSize) : PatchModule(maxPoly, bufferSize) {
    ItilializeVoices(1, 1);
}

void SubPatch::FillBuffers() {
    for (auto module : loadedPatch_) {
        module->FillBuffers();
    }
}