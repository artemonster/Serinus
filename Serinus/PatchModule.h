#ifndef PATCHMODULE_H_
#define PATCHMODULE_H_

#include "Serinus.h"
#include "Factory.h"
/**
  * This is a base class for every patch module, used in the engine.
  * Each module impementation should implement all virtual methods according to their specifications.
  * Each module has their special enum-namespaces, which should be maintained in PatchModuleConfigs.h
  * These namespaces serve many purposes, but main one is to name IO and parameters of the module.
  * Each module has it's own set of inputs (pointers to other module's outputs) and outputs (Samples)
  * Inputs can be treated as sample inputs, as well as modulating inputs for the parameters.
  * All parameters have type of Sample, so use this to your advantage, and don't forget about scalings/overflow.
  * Do not forget to fold all parameter pointers to internal parameters, because these pointers will be used to
  * load initial configuration for these parameters.
  * 
  * Authored: AK 
  * Last revision: 03.05.2014
  *
*/
class PatchModule {
public:
    /**Virtual desctructor, so we can delete derived by deleting base class.*/
	virtual ~PatchModule() {}; 
    /**Pointer to an array of output samples.*/
	Sample *output; 
    /**Pointer to an array of pointers to output samples of input modules.*/
	Sample **input;
    /**This method updates all outputs of the module by internal algorithm.*/
    virtual void Tick() = 0;
    /**This method updates all outputs and internal states of the module by an external command.*/
    virtual void ProcessCommand(const int &commandType, const int &commandIndex, const int &inValue) = 0;
    /**This method loads up the configuration map (internal state) for the parameters.*/
    inline void LoadConfiguration(const ModuleConfig &config) {
        ModuleConfig::const_iterator iterator;
        for (iterator = config.begin(); iterator != config.end(); iterator++) {
            *input[iterator->first] = iterator->second;
        }
    };
};

#endif /* PATCHMODULE_H_ */
