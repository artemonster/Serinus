/*
 * LowPass.h
 *
 *  Created on: 9 Apr 2014
 *      Author: akoso_000
 */

#ifndef LOWPASS_H_
#define LOWPASS_H_
#include "PatchModule.h"

class LowPass : public PatchModule {
public:
    static const CreatorImpl<LowPass> creator;
	LowPass();
	~LowPass() {};
	void Tick();
    void Modulate(int targetIndex, Sample inValue) {};
    void ProcessCommand(int commandIndex, int inValue) {};
private:
	Sample inT_1;
	Sample inT_2;
	int K;
};

#endif /* LOWPASS_H_ */
