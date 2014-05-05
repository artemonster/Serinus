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


*/

typedef int Sample; 					//this type is used for samples

typedef std::map<int, int> ModuleTypes;
typedef std::map<int, std::string> ModuleValues;
namespace Types { enum TypeMapping { INT, FLOAT, BOOL }; }
struct InputConfig {
    int inputIndex;
    int sourceModule;
    int outputIndex;
};
const InputConfig NO_INPUT = { 0, 0, 0 };
typedef std::vector<InputConfig> ModuleInputs;

const unsigned int SAMPLE_RATE = 44100;				//won't change (I guess)
const unsigned int BUFFER_SIZE = 1024; 				//specify min max
const unsigned int MAX_VOICES = 16; 				//just for fun
const unsigned int SINT32_UPSCALE = 2147483647LL;	//scaling factor to transform normalised float -1...+1 to SInt32
const unsigned int SINT16_UPSCALE = 32767;

const unsigned int UPSCALE = SINT32_UPSCALE;

#endif /* SERINUS_H_ */
