/*
 * Engine.cpp
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#include "Engine.h"
#include "Modules/PatchModule.h"
#include <list>

Engine::Engine() { 
    /*
        TODO register all available hardware (buttons, pots, lcd, io, etc) and assign proper handlers to it.
        TODO initialize filesystem and load a patch from it.
        ^^^^ this all will be supplied by the OS as EngineParameters in the constructor.
    */
    MidiHandler midiLookUp[] = { &Engine::HandleUnknownCmd, &Engine::HandleUnknownCmd, &Engine::HandleUnknownCmd, &Engine::HandleUnknownCmd,
        &Engine::HandleUnknownCmd, &Engine::HandleUnknownCmd, &Engine::HandleUnknownCmd, &Engine::HandleUnknownCmd,
        &Engine::NoteOff, &Engine::NoteOn, &Engine::Aftertouch, &Engine::ControlChange,
        &Engine::PatchChange, &Engine::ChannelPressure, &Engine::PitchWheel, &Engine::Sysex };
    //I don't trust std::memcpy :)
    for (int i = 0; i < 16; i++) {
        midiHndlTable[i] = midiLookUp[i];
    }

    for (int x = 0; x < 127; ++x) {
        midiNotes[x] = (440 / 32) * (2 ^ ((x - 9) / 12));
    }

    //this is a test data, ofc
    std::list<std::string> modulesInApatch = { "SawDCO", "LowPass" };
    int routingMatrix[2] = { 0, 0 };


    //LOG.debug("Instantiating modules for a patch...");
    std::list<std::string>::iterator iterator;
    for (iterator = modulesInApatch.begin(); iterator != modulesInApatch.end(); ++iterator) {
        currentPatch.push_back(Factory::create(*iterator));
    }

	/*
	 * Link all modules together. The input of module_i points to the output of the module_(i-x).
	 * This way we can use one output sample of some module at multiple places at once.
	 * TODO how modulation will be handled? Maybe some multiple input ports should be defined?
     * TODO how hardware inputs and outputs will be handled? they should be routed also together somehow (maybe a special index, or better: enum)

     * Also, because we are in the loop for all modules, load up their configurations.

	 * Alternative way to this idea is to call Tick(inSample) in a recursive way (pointed), so that every output lands on the stack
     * (Can be problematic when multiple sinks are registered for the same source!)
	 */
    std::vector<PatchModule*>::size_type i;
    int matrixIndex;
    for (i = 0, matrixIndex = 0; i != currentPatch.size(); i++, matrixIndex++) {
        PatchModule* source = currentPatch[routingMatrix[matrixIndex]]; //will be an array of multiple sources
        //TODO: currentPatch[i]->Configure(configs[i]) //something like that
        currentPatch[i]->inSample = &(source->outSample);
    }
    //Map inputs on hardware properly (see previous comment on linking)
	PatchModule* exitModule = this->currentPatch.back();
	lastSample=&(exitModule->outSample);
    inSample = NULL;
}

Sample Engine::Tick() {
	//for all active allocated voices
	//for all modules
	std::vector<PatchModule*>::iterator mod;
	for (mod=this->currentPatch.begin(); mod != this->currentPatch.end(); ++mod) {
		(*mod)->Tick();
	}
    //look up if a voice needs to be freed
	return *lastSample;
}

void Engine::HandleCommandQueue() {
	while (!cmds.empty()) {
        std::vector<unsigned char> command = cmds.front();

        //TODO make this a debug output
        unsigned int nBytes = command.size();
        for (unsigned int i = 0; i < nBytes; i++) {
            std::cout << "Byte " << i << " = " << (int)command.at(i) << ", ";
        }      
        std::cout << std::endl;

        unsigned char comamndByte = command.front();
        unsigned char lowNibble = comamndByte & 0x0F;
        unsigned char frontNibble = comamndByte >> 4;
        Engine::MidiHandler handler = midiHndlTable[frontNibble];
        (this->*(handler))(lowNibble, command);

        cmds.pop(); //remove handled command from queue.
	}
}

void Engine::pushCommand(std::vector<unsigned char> cmd) {
	cmds.push(cmd);
}

void Engine::NoteOff(unsigned char voice, std::vector<unsigned char> cmd) {

};

void Engine::NoteOn(unsigned char voice, std::vector<unsigned char> cmd) {
    runningStatus = 0x90 | voice;
    unsigned char note = cmd.at(1);
    unsigned char velocity = cmd.at(2);
    if (velocity == 0) {
        //handle like NoteOff, but do not set runningStatus
    }
    //for all registered trigger destinations
};

void Engine::HandleUnknownCmd(unsigned char wtf, std::vector<unsigned char> cmd) {
    if (runningStatus != 0) {
        Engine::MidiHandler handler = midiHndlTable[runningStatus];
        (this->*(handler))(wtf, cmd);
    } else {
        //WTF that can be?!
    }
};

void Engine::Sysex(unsigned char type, std::vector<unsigned char> cmd) {
    if (type >= 0 && type <= 7) {
        //System Common Category
        runningStatus = 0;
    } else {
        //RealTime Category
        //cmd.length()==1
    }
};