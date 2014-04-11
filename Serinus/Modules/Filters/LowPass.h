/*
 * LowPass.h
 *
 *  Created on: 9 Apr 2014
 *      Author: akoso_000
 */

#ifndef LOWPASS_H_
#define LOWPASS_H_
#include "..\PatchModule.h"

class LowPass : public PatchModule {
public:
	LowPass();
	~LowPass() {};
	void Tick();
private:
	Sample inT_1;
	Sample inT_2;
	int K;
};

#endif /* LOWPASS_H_ */
