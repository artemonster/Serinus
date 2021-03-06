#ifndef ENGINE_H_
#define ENGINE_H_

#include <vector>
#include <queue>
#include <list>
#include <string>
#include "PatchModule.h"
#include "Util/rapidxml.hpp"
/**
 * This is the engine class, which handles everything in the system.
 * Currently, only midi handling and patchmodule loading/configuration/connectivity is managed.
 *  
 * Authored: AK
 * Last revision: 29.05.2014
 */
class Engine {
public:
    Engine();
    ~Engine() {};
    
    typedef void( Engine::*MidiHandler )( unsigned char, MidiCmd );
    
    void PushCommand(MidiCmd cmd);
    void HandleCommandQueue();
    void FillAudioBuffers();
    void LoadPatch(std::vector<PatchModule*> &container, rapidxml::xml_node<>* patch_node);

    inline Sample getSample(int bufIndex) {
        return *( outputSamples + bufIndex );
    }

    //MIDI stuff
    void NoteOff(unsigned char voice, MidiCmd cmd);
    void NoteOn(unsigned char voice, MidiCmd cmd);
    void Aftertouch(unsigned char voice, MidiCmd cmd) {};
    void ControlChange(unsigned char voice, MidiCmd cmd) {};
    void PatchChange(unsigned char voice, MidiCmd cmd) {};
    void ChannelPressure(unsigned char voice, MidiCmd cmd) {};
    void PitchWheel(unsigned char voice, MidiCmd cmd) {};
    void Sysex(unsigned char type, MidiCmd cmd);
    void HandleRunningCmd(unsigned char wtf, MidiCmd cmd);
private:
    int retCode;
    
    // MIDI
    std::queue<MidiCmd> cmds;
    enum MidiEvent {NOTEOFF,NOTEON,AFTERT,CC,PATCH,CPRESS,PITCHW,SYSEX,MAX};
    static std::map<std::string, int> eventLookUp; //same as MidiEvent, but string->index map
    float midiNotes[128];
    unsigned char runningStatus = 0;
    MidiHandler midiHndlTable[16];

    // Polyphony
    struct Voice {
        int voiceNum;
        int notePlayed;
        bool operator ==(Engine::Voice rhs) {
            if (notePlayed == rhs.notePlayed) return true;
            else return false;
        }
    };
    std::list<Voice> activeVoices;
    std::list<Voice> availableVoices;
    
    // Patch
    std::vector<PatchModule*> loadedPatch;
    std::vector<PatchModule*> eventRegistry[MidiEvent::MAX];
    void registerReceiver(PatchModule* toRegister);
   
    // Settings
    bool clampMix = true;
    int voices;
    Sample *inSample;
    Sample *outputSamples;
};

#endif /* ENGINE_H_ */
