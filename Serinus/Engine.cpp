#include "Engine.h"
#include "PatchModule.h"
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
    ModuleValues DCOFreq {
        std::make_pair(P_Knob::VALUE, "1") // 0 V
    };

    ModuleValues LFOFreq {
        std::make_pair(P_Knob::VALUE, "8") // 2 Hz
    };

    ModuleValues AMP {
        std::make_pair(P_Knob::VALUE, "2147483647")
    };

    ModuleValues TrigFreq {
        std::make_pair(P_Knob::VALUE, "-1") //CV
    };

    ModuleValues PWM {
        std::make_pair(P_Knob::VALUE, "1813265919") //PWM 25%
    };

    ModuleValues LFO {
        std::make_pair(P_DirectDCO::WF, "2"),
        std::make_pair(P_DirectDCO::TUNE, "213.6"),
        std::make_pair(P_DirectDCO::MODE, "0") //hz control
    };

    ModuleValues MainOsc {
        std::make_pair(P_DirectDCO::WF, "1"),
        std::make_pair(P_DirectDCO::TUNE, "213.6"),
        std::make_pair(P_DirectDCO::MODE, "1") //VC control
    };

    ModuleValues TrigOsc {
        std::make_pair(P_DirectDCO::WF, "3"),
        std::make_pair(P_DirectDCO::TUNE, "1"),
        std::make_pair(P_DirectDCO::MODE, "1") //VC control
    };

    ModuleValues ADSRConf {
        std::make_pair(P_ADSR::ATTACK, "250"),
        std::make_pair(P_ADSR::DECAY, "90"),
        std::make_pair(P_ADSR::SUSTAIN, "50"),
        std::make_pair(P_ADSR::RELEASE, "1000")
    };

    ModuleValues VCAconfig {
        std::make_pair(P_VCA::MODE, "true")
    };

    std::list<Module> patch = {
        { "Knob", DCOFreq, { NO_INPUT } },
        { "Knob", LFOFreq, { NO_INPUT } },
        { "Knob", AMP, { NO_INPUT } },
        { "Knob", TrigFreq, { NO_INPUT } },
        { "Knob", PWM, { NO_INPUT } },

        { "DirectDCO", LFO, { { I_DirectDCO::PITCH, 1, O_Knob::VALUE }, //5
                              { I_DirectDCO::PWM, 4, O_Knob::VALUE },
                              { I_DirectDCO::AMP, 2, O_Knob::VALUE } } },
        { "DirectDCO", MainOsc, { { I_DirectDCO::PITCH, 0, O_Knob::VALUE }, //6
                                  { I_DirectDCO::PWM, 4, O_Knob::VALUE },
                                  { I_DirectDCO::AMP, 5, O_DirectDCO::SAMPLE } } },
        { "DirectDCO", TrigOsc, { { I_DirectDCO::PITCH, 3, O_Knob::VALUE },//7
                                  { I_DirectDCO::PWM, 4, O_Knob::VALUE },
                                  { I_DirectDCO::AMP, 2, O_Knob::VALUE } } },

        { "ADSR", ADSRConf, { { I_ADSR::GATE, 7, O_DirectDCO::SAMPLE } } },//8
        { "VCA", VCAconfig, { { I_VCA::INPUT, 6, O_DirectDCO::SAMPLE },
                                { I_VCA::GAIN, 8, O_ADSR::SAMPLE } } }
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
        //this->registerReceiver(currentModule);      
    }

    //allocate voices
    
    //struct Voice {
    //    int notePlayed;
    //    int state;
    //    PatchModule** config_;
    //};
    //int patchSize = patch.size();
    //Voice* voices = new Voice[maxPoly];
    //for (int i = 0; i < maxPoly; i++) {
    //    PatchModule** clone = new PatchModule*[patchSize];

    //    voices[i] = { 0, 0, clone };
    //}
    //Map inputs on hardware properly (see previous comment on linking)
    PatchModule* exitModule = currentPatch.back();

    outputSamples = exitModule->getOutput(0,0);
    inSample = NULL;
}

Sample Engine::Tick(int bufIndex) {
    //for all active allocated voices
    //for all modules
    std::vector<PatchModule*>::iterator mod;
    for (mod = currentPatch.begin(); mod != currentPatch.end(); ++mod) {
        ( *mod )->Tick(0,bufIndex);
    }
    //look up if a voice needs to be freed
    return outputSamples[bufIndex];
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
        ( this->*( handler ) )( lowNibble, command );

        cmds.pop(); //remove handled command from queue.
    }
}

void Engine::PushCommand(std::vector<unsigned char> cmd) {
    cmds.push(cmd);
}

void Engine::NoteOff(unsigned char voice, std::vector<unsigned char> cmd) {
    // remove from active voices
    // notify all receivers
    
};

void Engine::NoteOn(unsigned char voice, std::vector<unsigned char> cmd) {
    runningStatus = 0x90 | voice;
    unsigned char note = cmd.at(1);
    unsigned char velocity = cmd.at(2);
    if (velocity == 0) {
        //handle like NoteOff, but do not set runningStatus
    }
    //std::vector<void*( )> registredHandlers = registry[MidiCmd::NOTEON];
    //if (!registredHandlers.empty()) {
    //    std::vector<void*( )>::iterator handlersIt;
    //    for (handlersIt = registredHandlers.begin; handlersIt != registredHandlers.end(); ++handlersIt) {
    //        (*handlersIt)();
    //    }
    //}
    //for all registered trigger destinations
};

void Engine::HandleRunningCmd(unsigned char wtf, std::vector<unsigned char> cmd) {
    if (runningStatus != 0) {
        Engine::MidiHandler handler = midiHndlTable[runningStatus];
        ( this->*( handler ) )( wtf, cmd );
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