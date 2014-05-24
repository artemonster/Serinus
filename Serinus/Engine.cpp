#include "Engine.h"
#include "PatchModule.h"
#include "DirectDCO.h"
#include "PatchModuleConfigs.h"
#include <list>

Engine::Engine() {
    //these initalizers are here just for fun
    maxPoly = 5;
    bufferSize = BUFFER_SIZE;
    /*
        TODO register all available hardware (buttons, pots, lcd, io, etc) and assign proper handlers to it.
        TODO initialize filesystem and load a patch from it.
        ^^^^ this all will be supplied by the OS as EngineParameters in the constructor.
        */
    MidiHandler midiLookUp[] = { 
        &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd,
        &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd,
        &Engine::NoteOff, &Engine::NoteOn, &Engine::Aftertouch, &Engine::ControlChange,
        &Engine::PatchChange, &Engine::ChannelPressure, &Engine::PitchWheel, &Engine::Sysex };
    //I don't trust std::memcpy :)
    for (int i = 0; i < 16; i++) {
        midiHndlTable[i] = midiLookUp[i];
    }
    //Initialize midi note table
    float tune = 440.0;
    for (int x = 0; x <= 127; ++x) {
        midiNotes[x] = tune * (float)pow(2, ( ( (float)x - 69 ) / 12 ));
    }

    //<---------------------------------- TEST DATA ---------------------------------->
    ModuleValues LFOFreq {
        std::make_pair(P_Knob::VALUE, "8") // 8 Hz
    };

    ModuleValues AMP {
        std::make_pair(P_Knob::VALUE, "2147483647")
    };

    ModuleValues TrigFreq {
        std::make_pair(P_Knob::VALUE, "1") //CV
    };

    ModuleValues PWM {
        std::make_pair(P_Knob::VALUE, "1813265919") //PWM 25%
    };

    ModuleValues LFO {
        std::make_pair(P_DirectDCO::WF, "2"),
        std::make_pair(P_DirectDCO::TUNE, "213.63"),
        std::make_pair(P_DirectDCO::MODE, "0") //hz control
    };

    ModuleValues MainOsc {
        std::make_pair(P_DirectDCO::WF, "1"),
        std::make_pair(P_DirectDCO::TUNE, "213.6"),
        std::make_pair(P_DirectDCO::MODE, "1") //CV control
    };

    ModuleValues ADSRConf {
        std::make_pair(P_ADSR::ATTACK, "250"),
        std::make_pair(P_ADSR::DECAY, "90"),
        std::make_pair(P_ADSR::SUSTAIN, "50"),
        std::make_pair(P_ADSR::RELEASE, "1000")
    };

    std::list<Module> patch = {
        { "Knob", LFOFreq, { NO_INPUT }, NO_CMDS },
        { "Knob", AMP, { NO_INPUT }, NO_CMDS },
        { "Knob", TrigFreq, { NO_INPUT }, NO_CMDS },
        { "Knob", PWM, { NO_INPUT }, NO_CMDS },
        { "PolyKeys", NO_CONF, { NO_INPUT }, { ModuleCMD::NOTEOFF,ModuleCMD::NOTEON } }, //4
        { "DirectDCO", LFO, { { I_DirectDCO::PITCH, 0, O_Knob::VALUE }, //5
                              { I_DirectDCO::PWM, 3, O_Knob::VALUE },
                              { I_DirectDCO::AMP, 1, O_Knob::VALUE } }, NO_CMDS },
        { "DirectDCO", MainOsc, { { I_DirectDCO::PITCH, 4, O_PolyKeys::CV }, //6
                                  { I_DirectDCO::PWM, 3, O_Knob::VALUE },
                                  { I_DirectDCO::AMP, 5, O_DirectDCO::SAMPLE } }, NO_CMDS },
        { "ADSR", ADSRConf, { { I_ADSR::GATE, 4, O_PolyKeys::GATE } }, NO_CMDS },//7
        { "VCA", NO_CONF, { { I_VCA::INPUT, 6, O_DirectDCO::SAMPLE },
                                { I_VCA::GAIN, 7, O_ADSR::SAMPLE } }, NO_CMDS }
    };
    //<---------------------------------- TEST DATA END ---------------------------------->

    //LOG.debug("Instantiating modules for a patch...");
    std::list<Module>::iterator moduleIt;
    /*
    * Dynamically insantiate the module and apply configuration.
    * Then, link all modules together. All inputs of module_i point to the output of the module_(i-x).
    * This way we can use one output sample of some module at multiple places at once.
    * Q: how hardware inputs and outputs will be handled?
    * A: Special input and output PatchModules will be instantiated at the beginning and end of the configuration
    * TODO register module as special message receiver
    *
    * Alternative way to this idea is to call Tick(inSample) in a recursive way (pointed), so that every output 
    * lands on the stack (Can be problematic when multiple sinks are registered for the same source!)
    */
    for (moduleIt = patch.begin(); moduleIt != patch.end(); ++moduleIt) {
        PatchModule* currentModule = Factory::create(moduleIt->name, maxPoly, bufferSize);
        currentPatch.push_back(currentModule);
        currentModule->setId(currentPatch.size());
        currentModule->LoadConfiguration(currentModule->getParameterTypes(), moduleIt->config);
         
        ModuleInputs moduleConnections = moduleIt->connections;
        ModuleInputs::iterator inputsConfigIt;
        for (inputsConfigIt = moduleConnections.begin(); inputsConfigIt != moduleConnections.end(); ++inputsConfigIt) {
            //TODO: check for inputsConfigIt->first (0:NULL, 1024: HW, 1...999 patchModules)
            PatchModule* source = currentPatch[inputsConfigIt->sourceModule];
            if (!currentModule->hasNoInputs()) { //check, if module even has inputs :)
                //TODO: check for inputsConfigIt->second (0:NULL ....)
                for (int j = 0; j < maxPoly; ++j) {
                    currentModule->setLink(j, inputsConfigIt->inputIndex, 
                                           source->getOutput(j, inputsConfigIt->outputIndex));
                }
                
            }
        }   
        RegisterTo commandSources = moduleIt->commands;
        RegisterTo::iterator cmdIt;
        for (cmdIt = commandSources.begin(); cmdIt != commandSources.end(); ++cmdIt) {
            eventRegistry[( *cmdIt )].push_back(currentModule);
        }        
    }

    //Map inputs on hardware properly (see previous comment on linking)
    //Initialize voices
    PatchModule* exitModule = currentPatch.back();
    outputSamples = new Sample*[maxPoly];
    inSample = NULL;
    for (int i = 0; i < maxPoly; i++) {
        availableVoices.push_back(new Voice { i, 0 });
        outputSamples[i] = exitModule->getOutput(i,0);
    }  
}

Sample Engine::Tick(int bufIndex) {
    //for all active allocated voices
    //for all modules
    std::vector<PatchModule*>::iterator mod;
    for (mod = currentPatch.begin(); mod != currentPatch.end(); ++mod) {
        for (int i = 0; i < maxPoly; ++i) {
            ( *mod )->Tick(i,bufIndex);
        }
    }
    //mix all buffer samples properly!
    //http://www.vttoth.com/CMS/index.php/technical-notes/68
    // z= a + b - ab, or Z = 2 (a+b) - 2ab -1
    Sample outSample = 0;
    for (int i = 0; i < maxPoly; ++i) {
        outSample += *( outputSamples[i] + bufIndex ) / maxPoly;
    }
    return outSample;
}

void Engine::HandleCommandQueue() {
    while (!cmds.empty()) {
        MidiCmd command = cmds.front();
#ifdef SRS_DEBUG
        unsigned int nBytes = command.size();
        for (unsigned int i = 0; i < nBytes; i++) {
            std::cout << "Byte " << i << " = " << (int)command.at(i) << ", ";
        }
        std::cout << std::endl;
#endif // SRS_DEBUG
        unsigned char comamndByte = command.front();
        unsigned char lowNibble = comamndByte & 0x0F;
        unsigned char frontNibble = comamndByte >> 4;
        Engine::MidiHandler handler = midiHndlTable[frontNibble];
        ( this->*( handler ) )( lowNibble, command );
        cmds.pop(); //remove handled command from queue.
    }
}

void Engine::PushCommand(MidiCmd cmd) {
    cmds.push(cmd);
}

void Engine::NoteOff(unsigned char voice, MidiCmd cmd) {
    runningStatus = 0x80 | voice;
    unsigned char note = cmd.at(1);
    unsigned char velocity = cmd.at(2);
    std::list<Voice*>::iterator voiceIt;
    Voice* toFind = new Voice { 99, note };
    voiceIt = std::find(activeVoices.begin(), activeVoices.end(), toFind);
    if (voiceIt != activeVoices.end()) {
        availableVoices.push_front(*voiceIt);
        for (receiverIt = eventRegistry[NOTEOFF].begin(); receiverIt != eventRegistry[NOTEOFF].end(); ++receiverIt) {
            (*receiverIt)->ProcessCommand( ModuleCMD::NOTEOFF, (*voiceIt)->voiceNum, cmd, retCode );
        }
        activeVoices.erase(voiceIt);
    } else {
         //Played note not found? WTF? raise error! 
    }
};

void Engine::NoteOn(unsigned char voice, MidiCmd cmd) {
    runningStatus = 0x90 | voice;
    unsigned char note = cmd.at(1);
    unsigned char velocity = cmd.at(2);
    std::list<Voice*>::iterator voiceIt;
    Voice* toFind = new Voice { 99, note };
    voiceIt = std::find(activeVoices.begin(), activeVoices.end(), toFind);
    if (velocity == 0) {
        //handle like NoteOff, but do not set runningStatus
        if (voiceIt != activeVoices.end()) {
            availableVoices.push_front(*voiceIt);       
            for (receiverIt = eventRegistry[NOTEOFF].begin(); 
                 receiverIt != eventRegistry[NOTEOFF].end(); ++receiverIt) {
                (*receiverIt)->ProcessCommand( ModuleCMD::NOTEOFF, (*voiceIt)->voiceNum, cmd, retCode );
            }
            activeVoices.erase(voiceIt);
        } else {
            //Played note not found? WTF? raise error! 
        }
    } else {
        // here we have the possibility either to re-trigger same decaying note, or just allocate new one
        if (availableVoices.size() != 0) {
            Voice* newVoice = availableVoices.back();
            newVoice->notePlayed = note;
            for (receiverIt = eventRegistry[NOTEON].begin(); 
                 receiverIt != eventRegistry[NOTEON].end(); ++receiverIt) {
                PatchModule* registeredReceiver = ( *receiverIt );
                int voice = newVoice->voiceNum;
                registeredReceiver->ProcessCommand( ModuleCMD::NOTEON, voice, cmd, retCode );
            }
            activeVoices.push_front(newVoice);
            availableVoices.pop_back();
        } else {
            //override the oldest note!
            Voice* newVoice = activeVoices.back();
            newVoice->notePlayed = note;
            for (receiverIt = eventRegistry[NOTEON].begin(); 
                 receiverIt != eventRegistry[NOTEON].end(); ++receiverIt) {
                PatchModule* registeredReceiver = ( *receiverIt );
                int voice = newVoice->voiceNum;
                registeredReceiver->ProcessCommand( ModuleCMD::NOTEON, voice, cmd, retCode );
            }
            activeVoices.pop_back();
            activeVoices.push_front(newVoice);
        }
    }
};

void Engine::HandleRunningCmd(unsigned char wtf, MidiCmd cmd) {
    if (runningStatus != 0) {
        Engine::MidiHandler handler = midiHndlTable[runningStatus];
        ( this->*( handler ) )( wtf, cmd );
    } else {
        //WTF that can be?!
    }
};

void Engine::Sysex(unsigned char type, MidiCmd cmd) {
    if (type >= 0 && type <= 7) {
        //System Common Category
        runningStatus = 0;
    } else {
        //RealTime Category
        //cmd.length()==1
    }
};