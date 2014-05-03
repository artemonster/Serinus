/*
 * Engine.cpp
 *
 *  Created on: 8 Apr 2014
 *      Author: akoso_000
 */

#include "Engine.h"
#include "PatchModule.h"
#include "PatchModuleConfigs.h"
#include <list>

Engine::Engine() { 
    /*
        TODO register all available hardware (buttons, pots, lcd, io, etc) and assign proper handlers to it.
        TODO initialize filesystem and load a patch from it.
        ^^^^ this all will be supplied by the OS as EngineParameters in the constructor.
    */
    MidiHandler midiLookUp[] = { &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd,
        &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd,
        &Engine::NoteOff, &Engine::NoteOn, &Engine::Aftertouch, &Engine::ControlChange,
        &Engine::PatchChange, &Engine::ChannelPressure, &Engine::PitchWheel, &Engine::Sysex };
    //I don't trust std::memcpy :)
    for (int i = 0; i < 16; i++) {
        midiHndlTable[i] = midiLookUp[i];
    }

    float tune = 440.0;
    for (int x = 0; x <= 127; ++x) {
        midiNotes[x] = tune * pow(2, (((float)x - 69) / 12));
    }

    //this is a test data, ofc
    //List of string-named classes to instantiate. Index in this list is unique ID
    std::list<std::string> modulesInApatch = { "Knob", "Knob", "Knob", "SawDCO", "WaveTableOsc"};
    //This is a bit tricky: each module has vector of inputs, which can be mapped so some previous element, which has multiple outputs. SO:
    //For element E (vector), for input I of E (vector of inputs), specify source element S with needed output index O (int pair)
    
    std::list<std::vector<std::pair<int, int>>> routingMatrix = { { { 0, 0 } }, { { 0, 0 } }, { { 0, 0 } }, { { 0, 0 }, { 0, 0 } }, { { 2, 0 }, { 3, 0 } } };

    //LOG.debug("Instantiating modules for a patch...");
    std::list<std::string>::iterator iterator;
    std::list<std::vector<std::pair<int, int>>>::iterator routeIt;
    /*
    * Dynamically insantiate the module and apply configuration.
    * Then, link all modules together. All inputs of module_i point to the output of the module_(i-x).
    * This way we can use one output sample of some module at multiple places at once.
    * Q: how hardware inputs and outputs will be handled? 
    * A: Special input and output PatchModules will be instantiated at the beginning and end of the configuration
    * TODO register module as special message receiver
    *
    * Alternative way to this idea is to call Tick(inSample) in a recursive way (pointed), so that every output lands on the stack
    * (Can be problematic when multiple sinks are registered for the same source!)
    */
    for (routeIt = routingMatrix.begin(), iterator = modulesInApatch.begin(); iterator != modulesInApatch.end(); ++iterator, ++routeIt) {
        PatchModule* currentModule = Factory::create(*iterator);
        currentPatch.push_back(currentModule);
        //vector of inputs of currentModule, each of them pointing to the tuple of source module's index ([0]), and it's output index ([1])
        std::vector<std::pair<int, int>> router = *routeIt;
        std::vector<std::pair<int, int>>::iterator inputsIterator;
        for (inputsIterator = router.begin(); inputsIterator != router.end(); ++inputsIterator) {
            PatchModule* source = currentPatch[inputsIterator->first];
            currentModule->input[std::distance(router.begin(), inputsIterator)] = &(source->output[inputsIterator->second]);
            //Fold unrouted inputs to internals, if needed
            currentModule->FoldInputsToInternals();
        }
    }
    //Map inputs on hardware properly (see previous comment on linking)
	PatchModule* exitModule = this->currentPatch.back();
	lastSample=&(exitModule->output[0]);
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

void Engine::PushCommand(std::vector<unsigned char> cmd) {
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

void Engine::HandleRunningCmd(unsigned char wtf, std::vector<unsigned char> cmd) {
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