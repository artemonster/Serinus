#include <iostream>
#include <math.h>
#include <cstdio>
#include "signal.h"
#include "HAL\RtAudio.h"
#include "HAL\RtMidi.h"
#include "Engine.h"
#define __WINDOWS_MM__
using namespace std;

static int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData  ) {

	Engine *engine = (Engine*)userData; //Do a proper type cast
	int *outBuf = (int*)outputBuffer; //Do a proper type cast
	(void) inputBuffer; // Prevent unused variable warning.
    for(unsigned int i=0; i<nBufferFrames; i++) {
        *outBuf++ = engine->Tick();
    }
    return 0;
}

static bool isDone=false;
void siginthandler(int param){ isDone=true; }
unsigned int bufferFrames = 1024;

int main(int argc, char* args[]) {
	cout << "Testing..." << endl;
	signal(SIGINT, siginthandler);
	Engine engine;
	RtAudio dac;
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 2;
	parameters.firstChannel = 0;
	dac.openStream( &parameters, NULL, RTAUDIO_SINT32,
    SAMPLE_RATE, &bufferFrames, &audioCallback, (void *)&engine);
	dac.startStream();
//	RtMidiIn *midiin = new RtMidiIn();
//	midiin->openPort( 0 );
//	midiin->ignoreTypes( false, false, false );
//	std::vector<unsigned char> message;
//	int nBytes, i;
//	double stamp;

	while(!isDone) {
//		stamp = midiin->getMessage( &message );
//		    nBytes = message.size();
//		    for ( i=0; i<nBytes; i++ )
//		      std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
//		    if ( nBytes > 0 )
//		      std::cout << "stamp = " << stamp << std::endl;
		engine.HandleCommandQueue();
    }

	dac.stopStream();
    cout << "Finished!";
    return 0;
}
