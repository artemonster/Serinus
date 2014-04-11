/*
 * Engine.h
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <vector>
#include <queue>
#include <string>

#include "Modules/PatchModule.h"
#include "Modules/Generators/SawDCO.h"
#include "Modules/Filters/LowPass.h"

class Engine {
public:
	Engine();
	~Engine() {};
	Sample Tick();
	void HandleCommandQueue();
	void pushCommand(std::string cmd);

	//std::queue<int*> voices;123
	//void loadPatch(std::string patchName);
private:
	std::vector<PatchModule*> currentPatch;
	std::queue<std::string> cmds;
	Sample *inSample;
	Sample *lastSample;
};

#endif /* ENGINE_H_ */
