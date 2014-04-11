/*
 * PatchModule.h
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#ifndef PATCHMODULE_H_
#define PATCHMODULE_H_

#include "../Serinus.h"

class PatchModule {
public:
	virtual void Tick() = 0;
	//virtual void ProcessMessage() = 0;
	virtual ~PatchModule() {};
	Sample outSample;
	Sample *inSample;
};

#endif /* PATCHMODULE_H_ */
