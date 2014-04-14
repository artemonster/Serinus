/*
 * PatchModule.h
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#ifndef PATCHMODULE_H_
#define PATCHMODULE_H_
#include <map>
#include "../Serinus.h"

class PatchModule {
public:
	virtual void Tick() = 0;
	//virtual void ProcessMessage() = 0;
	virtual ~PatchModule() {};
	Sample outSample;
	Sample *inSample;
};

template<typename T> PatchModule* createInstance() { return new T; }
typedef std::map<std::string, PatchModule*(*)()> ModulesMap;
#endif /* PATCHMODULE_H_ */
