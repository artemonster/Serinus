#ifndef ENGINE_H_
#define ENGINE_H_

#include <vector>
#include <queue>
#include <list>
#include <string>
#include "PatchModule.h"
/**
 * This is the engine class, which handles everything in the system.
 * Currently, only midi handling and patchmodule loading/configuration/connectivity is managed.
 * Additionally, there will be a hardware IO registry, filesystem handling and UI.
 * After bootloader is done checking/updating firmware, the main program will take over.
 * Main program will consist only of the engine instantiation, which will take over from this point, 
 * managing everything.
 *
 * Mini ToDo:
 *      add initializer method, which will load up default engine config from text/xml
 *      add patch-reinitializer, so that patch can be reloaded by a command
 *      
 * Authored: AK
 * Last revision: 20.05.2014
 */
class Engine {
public:
    Engine();
    ~Engine() {};
    
    typedef void( Engine::*MidiHandler )( unsigned char, MidiCmd );
    
    void PushCommand(MidiCmd cmd);
    void HandleCommandQueue();
    void FillAudioBuffers();

    inline Sample MixAllVoices(int bufIndex) {
        Sample outSample = 0;
        if (clampMix) {
            for (int i = 0; i < maxPoly; ++i) {
                outSample += *( outputSamples[i] + bufIndex );
            }
            outSample *= 0.707946f; //-3dB
            if (outSample >= 1.0) outSample = 1.0;
            if (outSample <= -1.0) outSample = -1.0;
        } else {
            Sample scaleFactor = static_cast<Sample>( 1.0/maxPoly );
            for (int i = 0; i < maxPoly; ++i) {
                outSample += *( outputSamples[i] + bufIndex )*scaleFactor;
            }
        }
        return outSample;
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
    bool clampMix = true;
    // Midi stuff:
    void registerReceiver(PatchModule* toRegister);
    enum MidiEvent {NOTEOFF,NOTEON,AFTERT,CC,PATCH,CPRESS,PITCHW,SYSEX};
    float midiNotes[128];
    unsigned char runningStatus = 0;
    int retCode;
    MidiHandler midiHndlTable[16];
    std::vector<PatchModule*> eventRegistry[8];
    std::vector<PatchModule*>::iterator receiverIt;
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
    std::list<Voice>::iterator voiceIt;
    // Patch
    std::vector<PatchModule*> currentPatch;
    int maxPoly;
    int bufferSize;
    std::queue<MidiCmd> cmds;
    Sample *inSample;
    Sample **outputSamples;
    
    struct Module {
        std::string name;
        ModuleValues config;
        ModuleInputs connections;
        RegisterTo commands;
    };
};

#endif /* ENGINE_H_ */
