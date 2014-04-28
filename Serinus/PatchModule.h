#ifndef PATCHMODULE_H_
#define PATCHMODULE_H_

#include "Serinus.h"
#include "Factory.h"

class PatchModule {
public:
	virtual ~PatchModule() {};

	Sample outSample;
	Sample *inSample;
    Sample **modIn;
    virtual void Tick() = 0;
    virtual void ProcessCommand(int commandIndex, int inValue) = 0;
    virtual void Modulate(int targetIndex, Sample inValue) = 0;
};

#endif /* PATCHMODULE_H_ */
