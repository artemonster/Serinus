/*
 * SawDCO.h
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#ifndef SAWDCO_H_
#define SAWDCO_H_
#include "PatchModule.h"

class SawDCO : public PatchModule  {
public:
    static const CreatorImpl<SawDCO> creator;
	SawDCO(); 
	~SawDCO() {};
	void Tick();
    void ProcessCommand(int commandType, int commandIndex, int inValue) {};
    void FoldInputsToInternals() {};
private:
    InternalVal phase;
};

#endif /* SAWDCO_H_ */
