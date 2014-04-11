/*
 * Engine.cpp
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#include "Engine.h"

Engine::Engine() {
	inSample=NULL;

	/*
	 * Instantiate all modules according to the loaded patch
	 * and push them into the patch vector.
	 * TODO is vector a good container for this?
	 * TODO patch loading + module initial configuration in constructor
	 */
	PatchModule* dco = new SawDCO();
	currentPatch.push_back(dco);
	PatchModule* vcf = new LowPass();
	currentPatch.push_back(vcf);

	/**
	 * Link all modules together. The input of module_i points to the output of the module_(i-1).
	 * This way we can use one output sample of some module at multiple places at once.
	 * TODO automatic linking according to routing matrix.
	 *
	 * Alternative way is to call Tick(inSample) in a recursive way (pointed), so that everything is on the stack
	 * this is just here so I won't forget.
	 */
	vcf->inSample = &(dco->outSample);

	//Link the output of last module to the output sample of the engine.
	PatchModule* exitModule = this->currentPatch.back();
	lastSample=&(exitModule->outSample);
}

Sample Engine::Tick() {
	//for all allocated voices
	//for all modules
	std::vector<PatchModule*>::iterator mod;
	for (mod=this->currentPatch.begin(); mod != this->currentPatch.end(); ++mod) {
		(*mod)->Tick();
	}
	return *lastSample;
}

void Engine::HandleCommandQueue() {
	while (!cmds.empty()) {
		//TODO add new class: command and add handling here
		//std::string command = cmds.pop();
		//....
	}
}

void Engine::pushCommand(std::string cmd) {
	cmds.push(cmd);
}
