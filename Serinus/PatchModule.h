#ifndef PATCHMODULE_H_
#define PATCHMODULE_H_

#include "Serinus.h"
#include "Factory.h"

class PatchModule {
public:
    //Virtual desctructor, so we can delete derived by deleting base class.
	virtual ~PatchModule() {}; 
    //Pointer to an array of output samples.
	Sample *output; 
    //Pointer to an array of pointers to output samples of input modules.
	Sample **input;
    //This method updates all outputs of the module by internal algorithm.
    virtual void Tick() = 0;
    //This method updates all outputs and internal states of the module by an external command.
    virtual void ProcessCommand(int commandType, int commandIndex, int inValue) = 0;
    //This method folds all unrouted input NULL pointers to their respective internal defaults.
    virtual void FoldInputsToInternals() = 0;
    //virtual void Modulate(int targetIndex, Sample inValue) = 0;
};

#endif /* PATCHMODULE_H_ */
