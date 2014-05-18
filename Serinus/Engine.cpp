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
    ModuleValues KnobConfig1 {
        std::make_pair(P_Knob::VALUE, "220")
    };

    ModuleValues KnobConfig2 {
        std::make_pair(P_Knob::VALUE, "2")
    };

    ModuleValues KnobConfig3 {
        std::make_pair(P_Knob::VALUE, "2147483647")
    };

    ModuleValues DirectDCOConfig {
        std::make_pair(P_DirectDCO::WF, "2")
    };

    ModuleValues DirectDCOConfig2 {
        std::make_pair(P_DirectDCO::WF, "0")
    };

    ModuleValues VCAconfig {
        std::make_pair(P_VCA::MODE, "true")
    };

    std::list<Module> patch = {
        { "Knob", KnobConfig1, { NO_INPUT } },//Frequency setting for OSC
        { "Knob", KnobConfig2, { NO_INPUT } },//LFO freq
        { "Knob", KnobConfig3, { NO_INPUT } },//LFO depth
        //{ "DirectDCO", DirectDCOConfig, { { I_DirectDCO::PITCH, 1, O_Knob::VALUE }, 
        //                                  { I_DirectDCO::AMP, 2, O_Knob::VALUE } } }, //LFO
        //{ "DirectDCO", DirectDCOConfig2, { { I_DirectDCO::PITCH, 0, O_Knob::VALUE }, 
        //                                   { I_DirectDCO::AMP, 3, O_DirectDCO::SAMPLE } } }
        { "DirectDCO", DirectDCOConfig, { { I_DirectDCO::PITCH, 1, O_Knob::VALUE },
                                            { I_DirectDCO::AMP, 2, O_Knob::VALUE } } }, //LFO
        { "DirectDCO", DirectDCOConfig2, { { I_DirectDCO::PITCH, 0, O_Knob::VALUE },
                                            { I_DirectDCO::AMP, 2, O_Knob::VALUE } } },
        { "VCA", VCAconfig, { { I_VCA::INPUT, 4, O_DirectDCO::SAMPLE },
                                            { I_VCA::GAIN, 3, O_DirectDCO::SAMPLE } } }
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
        PatchModule* currentModule = Factory::create(moduleIt->name);
        currentPatch.push_back(currentModule);
        currentModule->LoadConfiguration(currentModule->getParameterTypes(), moduleIt->config);
        ModuleInputs moduleConnections = moduleIt->connections;
        ModuleInputs::iterator inputsConfigIt;
        for (inputsConfigIt = moduleConnections.begin(); inputsConfigIt != moduleConnections.end(); ++inputsConfigIt) {
            //TODO: check for inputsConfigIt->first (0:NULL, 1024: HW, 1...999 patchModules)
            PatchModule* source = currentPatch[inputsConfigIt->sourceModule];
            if (!currentModule->hasNoInputs()) { //check, if module even has inputs :)
                //TODO: check for inputsConfigIt->second (0:NULL ....)
                currentModule->setLink(inputsConfigIt->inputIndex, source->getOutput(inputsConfigIt->outputIndex));
            }
        }
        //this->registerReceiver(currentModule);      
    }
    //Map inputs on hardware properly (see previous comment on linking)
    PatchModule* exitModule = currentPatch.back();

    lastSample = exitModule->getOutput(0);
    inSample = NULL;
}

Sample Engine::Tick() {
    //for all active allocated voices
    //for all modules
    std::vector<PatchModule*>::iterator mod;
    for (mod = currentPatch.begin(); mod != currentPatch.end(); ++mod) {
        ( *mod )->Tick();
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
        ( this->*( handler ) )( lowNibble, command );

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