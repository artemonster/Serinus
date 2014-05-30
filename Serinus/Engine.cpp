#include "Engine.h"
#include "PatchModule.h"
#include "SubPatch.h"
#include "DirectDCO.h"
#include <list>
#include <iostream>
#include <fstream>

std::map<std::string, int> Engine::eventLookUp = {
    { "NoteOff", 0 },
    { "NoteOn", 1 },
    { "Aftertouch", 2 },
    { "ControlChange", 3 },
    { "PatchChange", 4 },
    { "ChannelPressure", 5 },
    { "PitchWheel", 6 },
    { "SysEx", 7 }};

Engine::Engine() {
    //these initalizers are here just for fun
    voices = 5;
    /*
        TODO register all available hardware (buttons, pots, lcd, io, etc) and assign proper handlers to it.
        TODO initialize filesystem and load a patch from it.
        ^^^^ this all will be supplied by the OS as EngineParameters in the constructor.
        */

    //maybe convert this all to static, since engine will be singleton anyways....?
    MidiHandler midiLookUp[] = { 
        &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd,
        &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd, &Engine::HandleRunningCmd,
        &Engine::NoteOff, &Engine::NoteOn, &Engine::Aftertouch, &Engine::ControlChange,
        &Engine::PatchChange, &Engine::ChannelPressure, &Engine::PitchWheel, &Engine::Sysex };
    //I don't trust std::memcpy :)
    for (int i = 0; i < 16; i++) {
        midiHndlTable[i] = midiLookUp[i];
    }
    //Initialize midi note table - may be obsolete, who knows.
    float tune = 440.0;
    for (int x = 0; x <= 127; ++x) {
        midiNotes[x] = tune * (float)pow(2, ( ( (float)x - 69 ) / 12 ));
    }

    rapidxml::xml_document<> doc;
    std::ifstream file("test_patch.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);
    //TODO make a bank system and load a map of <Patch name="super cool lead"> for future loading system.
    rapidxml::xml_node<>* patch_node = doc.first_node("Patch");

    if (patch_node != NULL) {
        this->LoadPatch(this->loadedPatch, patch_node);
    } else {
        std::cout << "Couldn't find <Patch> tag!\n";
    }

    //TODO: when using VST, map inputs and outputs on hardware properly!
    outputSamples = new Sample[kBufferSize];
    inSample = NULL;

    PatchModule* exitModule = loadedPatch.back();

    outputSamples = exitModule->getOutput(0, 0, 0); //last module can't be polyphonic! link only to the 1st sample
    //Initialize voices
    for (int i = 0; i < voices; ++i) {
        availableVoices.push_back(Voice { i, 0 });       
    } 
}

void Engine::LoadPatch(std::vector<PatchModule*> &container, rapidxml::xml_node<>* patch_node) {
    using namespace rapidxml;
    std::map<std::string, int> indexIdentifiers;
    for (xml_node<>* patchModule = patch_node->first_node(); patchModule; patchModule = patchModule->next_sibling()) {
        std::string moduleName = patchModule->name();
        if (moduleName == "SubPatch") {
            SubPatch* subPatch = new SubPatch(voices);
            //TODO fetch patch from existing ones or use local one
            xml_node<>* patch_node = patchModule->first_node("Patch");
            if (patch_node != NULL) {
                this->LoadPatch(subPatch->getContainer(), patch_node);
            } else {
                std::cout << "Couldn't find <Patch> tag inside a SubPatch module!\n";
            }
            // TODO link IO, don't forget IDs
        } else {    
            PatchModule* currentModule = Factory::create(moduleName, voices);
            if (currentModule != NULL) {
                std::string id = patchModule->first_attribute("id")->value();
                container.push_back(currentModule);          
                if (indexIdentifiers.find(id) == end(indexIdentifiers)) {
                    indexIdentifiers.insert(std::make_pair(id, container.size()-1));
                    currentModule->setId(id);
                } else {
                    std::cout << "ID should be unique: " << id << "\n";
                }
                //Parse config, if available
                xml_node<>* configs = patchModule->first_node("Config");
                if (configs != NULL) {
                    for (xml_node<>* configNode = configs->first_node(); configNode; 
                                                                             configNode = configNode->next_sibling()) {
                        std::string prop = configNode->name();
                        std::string value = configNode->value();
                        if (currentModule->setParameter(prop, value) != 0) {
                            std::cout << "Could not set property named: " << prop << " with value: " << value << "\n";
                        }
                    }
                }
                //Parse connections, if available
                xml_node<>* connections = patchModule->first_node("Connections");
                if (connections != NULL) {
                    for (xml_node<>* connection = connections->first_node("Connection"); connection; 
                                                                            connection = connection->next_sibling()) {
                        std::string dest = connection->first_attribute("to")->value();
                        std::string src = connection->first_attribute("from")->value();
                        std::string of = connection->first_attribute("of")->value();
                        PatchModule* source;
                        auto moduleToFind = indexIdentifiers.find(of);
                        if (moduleToFind != indexIdentifiers.end()) {
                            source = container.at(moduleToFind->second);
                        } else {
                            source = NULL;
                            std::cout << "Could not find source module by ID: " << of << "\n";
                        }
                        int outIndex = source->getOutputIndexByName(src);
                        if (outIndex < 0) {
                            std::cout << "Could not find output by name: " << src << " of " << of << "\n";
                        }
                        int inIndex = currentModule->getInputIndexByName(dest);  
                        if (inIndex < 0) {
                            std::cout << "Could not find input by name: " << dest << " of " << id << "\n";
                        }
                        currentModule->setLink(source, inIndex, outIndex);
                    }
                }
                //Parse event sources, if available
                xml_node<>* events = patchModule->first_node("Events");
                if (events != NULL) {
                    for (xml_node<>* event = events->first_node("Event"); event; event = event->next_sibling()) {
                        std::string eventType = event->value();
                        std::map<std::string, int>::iterator toFind = eventLookUp.find(eventType);
                        int eventIndex;
                        if (toFind != eventLookUp.end()) {
                            eventIndex = toFind->second;
                        } else {
                            std::cout << "Couldn't register event called " << eventType << "\n";
                        }
                        eventRegistry[eventIndex].push_back(currentModule);
                    }
                }
            } else {
                std::cout << "Couldn't instantiate PatchModule called " << moduleName << "\n";
            }
        }
    }
}

void Engine::FillAudioBuffers() {
    for (auto module : loadedPatch) {
        module->FillBuffers();
    }
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

        //Faulty commands are a result of really nasty synchronization bug
        //when you hit a lot of keys simultaneously.
        //this will only be fixed when introducing VST
        if (command.size() == 0) {
            std::cout << "Faulty command!\n";
            //TODO raiseerror
            goto pop;
        }
        unsigned char comamndByte = command.front();
        unsigned char lowNibble = comamndByte & 0x0F;
        unsigned char frontNibble = comamndByte >> 4;
        Engine::MidiHandler handler = midiHndlTable[frontNibble];
        ( this->*( handler ) )( lowNibble, command );
pop:        cmds.pop(); //remove handled command from queue.
    }
}

void Engine::PushCommand(MidiCmd cmd) {
    cmds.push(cmd);
}

void Engine::NoteOff(unsigned char voice, MidiCmd cmd) {
    runningStatus = 0x80 | voice;
    unsigned char note = cmd.at(1);
    unsigned char velocity = cmd.at(2);
    auto voiceIt = std::find(activeVoices.begin(), activeVoices.end(), Voice { 99, note });
    if (voiceIt != activeVoices.end()) {
        availableVoices.push_front(*voiceIt);
        for (auto receiver : eventRegistry[NOTEOFF]) {
            receiver->ProcessCommand( ModuleCMD::NOTEOFF, (*voiceIt).voiceNum, cmd, retCode );
        }
        activeVoices.erase(voiceIt);
    } else {
         //Played note not found?
         //it might have been replaced if more keys were pressed than voices available.
    }
};

void Engine::NoteOn(unsigned char voice, MidiCmd cmd) {
    runningStatus = 0x90 | voice;
    unsigned char note = cmd.at(1);
    unsigned char velocity = cmd.at(2);
    auto voiceIt = std::find(activeVoices.begin(), activeVoices.end(), Voice { 99, note });
    if (velocity == 0) {
        //handle like NoteOff, but do not set runningStatus
        if (voiceIt != activeVoices.end()) {
            availableVoices.push_front(*voiceIt);  
            for (auto receiver : eventRegistry[NOTEOFF]) {
                receiver->ProcessCommand( ModuleCMD::NOTEOFF, (*voiceIt).voiceNum, cmd, retCode );
            }
            activeVoices.erase(voiceIt);
        } else {
            //Played note not found?
            //it might have been replaced if more keys were pressed than voices available.
        }
    } else {
        if (availableVoices.size() != 0) {
            //TODO search for already triggered note and make this option tunable.
            Voice newVoice = availableVoices.back(); //just allocate a new one
            newVoice.notePlayed = note;
            for (auto receiver : eventRegistry[NOTEON]) {
                receiver->ProcessCommand( ModuleCMD::NOTEON, newVoice.voiceNum, cmd, retCode );
            }
            activeVoices.push_front(newVoice);
            availableVoices.pop_back();
        } else {
            //override the oldest note!
            Voice newVoice = activeVoices.back();
            newVoice.notePlayed = note;
            for (auto receiver : eventRegistry[NOTEON]) {
                receiver->ProcessCommand( ModuleCMD::NOTEON, newVoice.voiceNum, cmd, retCode );
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