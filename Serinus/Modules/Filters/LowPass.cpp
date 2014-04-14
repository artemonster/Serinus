/*
 * LowPass.cpp
 *
 *  Created on: 9 Apr 2014
 *      Author: akoso_000
 */

#include "LowPass.h"

const CreatorImpl<LowPass> LowPass::creator("LowPass");

LowPass::LowPass() {
	inT_1=0;
	inT_2=0;
	K=2;
}
// y[k]=(x[k]+2*x[k-1]+x[k-2])/4
void LowPass::Tick() {
    outSample = (*inSample*K + inT_1 + inT_2)/4;
	inT_2=inT_1;
	inT_1=*inSample;
}


