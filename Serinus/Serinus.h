/*
 * Sreinus.h
 *
 *  Created on: 9 Apr 2014
 *      Author: akoso_000
 */

#ifndef SREINUS_H_
#define SREINUS_H_

#include <iostream>
#include <math.h>
#include <cstdio>
#include "signal.h"
#include "HAL\RtAudio.h"
#include "HAL\RtMidi.h"

typedef int Sample; 					//this type is used for samples
typedef float InternalVal;              //this is what used inside each module to store parameters!
const unsigned int SAMPLE_RATE = 44100;			//won't change (I guess)
const unsigned int BUFFER_SIZE = 1024; 			//specify min max
const unsigned int MAX_VOICES = 16; 				//just for fun
const unsigned int SINT32_UPSCALE = 2147483647LL; //scaling factor to transform normalised float -1...+1 to SInt32
const unsigned int SINT16_UPSCALE = 32767;

const unsigned int UPSCALE = SINT32_UPSCALE;

#endif /* SREINUS_H_ */
