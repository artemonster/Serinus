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
#include "PatchModule.h"

class Engine {
public:
	Engine();
	~Engine() {};
    Sample Tick(); 
    void PushCommand(std::vector<unsigned char> cmd);
	void HandleCommandQueue();
    
    //MIDI stuff
    typedef void(Engine::*MidiHandler)(unsigned char, std::vector<unsigned char>);
    void NoteOff(unsigned char voice, std::vector<unsigned char> cmd);
    void NoteOn(unsigned char voice, std::vector<unsigned char> cmd);
    void Aftertouch(unsigned char voice, std::vector<unsigned char> cmd) {};
    void ControlChange(unsigned char voice, std::vector<unsigned char> cmd) {};
    void PatchChange(unsigned char voice, std::vector<unsigned char> cmd) {};
    void ChannelPressure(unsigned char voice, std::vector<unsigned char> cmd) {};
    void PitchWheel(unsigned char voice, std::vector<unsigned char> cmd) {};
    void Sysex(unsigned char type, std::vector<unsigned char> cmd);
    void HandleRunningCmd(unsigned char wtf, std::vector<unsigned char> cmd);
     
private:
    // Midi stuff:
    float midiNotes[128];
    unsigned char runningStatus = 0;
	std::vector<PatchModule*> currentPatch;
    std::queue<std::vector<unsigned char>> cmds;
	Sample *inSample;
    Sample *lastSample;
    MidiHandler midiHndlTable[16];
    struct Module {
        std::string name;
        ModuleConfig config;
        ModuleInputs connections;
    };
};

#endif /* ENGINE_H_ */
