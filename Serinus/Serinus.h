#ifndef SERINUS_H_
#define SERINUS_H_

#include <iostream>
#include <math.h>
#include <cstdio>
#include <cstdint>
#include <map>
#include <vector>
#include "signal.h"
#include "HAL\RtAudio.h"
#include "HAL\RtMidi.h"

/**
================= WORK LOG =================
...prehistoric times:
Legend says, that a very blurry foundation to the project was laid somewhat in between 01.03.14 and 04.05.14

05.05.2014: Creation of the work log.
Today I've finally tweaked the module configuration system, so it will be as generic as possible.
If everything goes wrong, then I always can revert to stupid switchcase implementation for every module.
Also, the one and only SawDCO was renamed to DirectDCO (DDS), which now contains all 4 needed waveforms.

13.05.2014: Little things.
After some pause in the works I finally got back to the project. To get back to it I've started with liitle things,
like hiding PathModule member vars under protected and giving some accesor methods to them.
Also I've added a simple VCA module, which has two modes: linear and logarithmic. Second one is a little funky though,
I need an oscilloscope to see what actually happens there.
ADSR is also there, but I haven't decided yet on CV signals (a part of datapath or events)

19.05.2014: Events and stuff
Once again I've tried to solve this event registry problem, routing internal setters/getters + MIDI commands properly
through all voices, and I stumbled upon many problems. So we wait on that...
Also, ADSR got implemented, but not tested yet. 
DCO got CV mode, so sooner or later I will add proper midi handling.
Now I've started to think about switching back to floats for signal representation, because SINT32 is bothering me with
overflows (multiply two signals, which are "normalized" between -1..+1 will result in temporary long promotion.
I've also renamed member values of patchmodules so that they have trailing underscore.

20.05.2014: Inseparable
I thought that I could implement many things step-by-step, and command handling with voice allocation system will be
no exception to that. Well, I was wrong. They are seriously inseparable, so I guess this system will come in a big
bulk update soon.
Today I've finished ADSR and default ahrd-wired patch is ok

23.05.2014: Buffers and crazy pointers
Today I've introduced polyphony and buffers to the PatchModule, which effectively made output sample buffer
a 3d array :)) Which means, input buffer pointer is a 4-way pointer! Woohoo!
The polyphonic patch is made in such way that only voice-related parameters (ADSR state, phasors of OSC, etc) are
cloned. Everything else (global parameters, settings, the module itself) is not. This greatly saves the memory.
Next time I will make a voice allocator and introduce fixed point.

24.05.2014: Polyphony and MIDI
So, that is it! The synth accepts midi input and produces a polyphonic audio output. It is still buggy and I need to
sort out a lot of things though. I've also addded a little MidiCommander project which will send different midi 
commands to loopMIDI, so that I don't need to attach my keyboard each time I want to test something.
Next stop: bug fixing, clean-up, shiny refactoring and fixed point!

25.05.2014: === A Historical Moment ===
Polyphony works great! All bugs are sorted out, so I can finally start working on "looks" of the code and 
optimizations. Woohoo!
Today I've also reduced bazillion of tick calls by inlined methods to fill buffers. This should speed things up.

26.05.2014: Floats everywhere!!11one
Despite the initial idea of using fixed point arithmetics I've decided to drop it and go with 32 bit floats.
I am not sure whether I will implement this synth in hardware, so there is no need to think about such things ahead 
of the time.
I've also moved ModuleTypes initialization to the module constructor, where it belongs.
Now I am considering how to implement the xml serialization.
Also, I try harder to follow common c++ code style conventions :) soon, major refactoring will occur.
*/

#define SRS_DEBUG                                   //Define this, if you want debug output
typedef float Sample; 				                //this type is used for samples
typedef std::map<int, int> ModuleTypes;             //this type is used to map module types to indeces
typedef std::map<int, std::string> ModuleValues;    //this type is used to load module parameters

typedef std::vector<unsigned char> MidiCmd;
enum ModuleCMD {NOTEOFF,NOTEON,GET,SET};
typedef std::vector<ModuleCMD> RegisterTo;

namespace Types { enum TypeMapping { INT, FLOAT, SAMPLE, BOOL }; }

struct InputConfig {
    int inputIndex;
    int sourceModule;
    int outputIndex;
};

typedef std::vector<InputConfig> ModuleInputs;
const InputConfig NO_INPUT = { 0, 0, 0 };
const ModuleValues NO_CONF;
const RegisterTo NO_CMDS;

const unsigned int kSampleRate = 44100;				//won't change (I guess)
const unsigned int kBufferSize = 512; 				//specify min max
#endif /* SERINUS_H_ */
