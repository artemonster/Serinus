#ifndef VCA_H_
#define VCA_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"
/**
This is a basic VCA module, which can operate in both linear and logarithmic modes.
Gain input is a standard sample, which is then normalized to -1...1 value
TODO: check the output sample spikes when using log mode and ramp DCO

Authored: AK
Last revision: 13.05.2014
*/
class VCA : public PatchModule {
public:
    static const CreatorImpl<VCA> creator;
    VCA();
    ~VCA() {};
    void Tick();
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
    ModuleTypes getParameterTypes();
private:
    bool isLinear_;
};
#endif /* VCA_H_ */