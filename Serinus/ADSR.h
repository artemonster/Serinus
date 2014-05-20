#ifndef ADSR_H_
#define ADSR_H_
#include "PatchModule.h"
#include "PatchModuleConfigs.h"
/**
This is a basic ADSR module, which can operate in both linear and exponential modes.
TODO: decide on gate and trigger inputs (whether or not they should be controlled via event or patch samples)
Float parameters are in milliseconds, Release in percent (0-100)
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
    enum State { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE };
    float outputSample_;
    State state_;
    bool isLinear_;
    float attack_;
    float decay_;
    float sustain_;
    float release_;

    bool keyPressed;
    float attackRate_;
    float sustainLevel_;
    float decayRate_;
    float releaseRate_;
};
#endif /* ADSR_H_ */