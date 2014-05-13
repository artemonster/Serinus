#ifndef ADSR_H_
#define ADSR_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"
/**
This is a basic ADSR module, which can operate in both linear and exponential modes.
TODO: decide on gate and trigger inputs (whether or not they should be controlled via event or patch samples)

Authored: AK
Last revision: 13.05.2014
*/
class ADSR : public PatchModule {
public:
    static const CreatorImpl<ADSR> creator;
    ADSR();
    ~ADSR() {};
    void Tick();
    void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) {};
    ModuleTypes getParameterTypes();
protected:
    bool isLinear = true;
};
#endif /* ADSR_H_ */