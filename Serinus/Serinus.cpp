#include "Serinus.h"
#include "Engine.h"

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

static void midiMonitor(double deltatime, std::vector<unsigned char> *message, void *userData) {
    Engine* engine = (Engine*)userData;
    engine->PushCommand(*message);
}

static bool isDone=false;
void siginthandler(int param){ isDone=true; }

int main(int argc, char* args[]) {
	std::cout << "Testing..." << std::endl;
	signal(SIGINT, siginthandler);

	Engine engine;
	RtAudio dac;
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 1 ;
	parameters.firstChannel = 0;

    unsigned int bufferFrames = BUFFER_SIZE; //cannot be const, a hack for RtAudio

    dac.openStream(&parameters, NULL, RTAUDIO_SINT32,
        SAMPLE_RATE, &bufferFrames, &audioCallback, (void *)&engine);
	dac.startStream();

	RtMidiIn midiin;
	midiin.openPort(0);
	midiin.ignoreTypes(false, false, false);
    midiin.setCallback(&midiMonitor, &engine);

	while(!isDone) {
		engine.HandleCommandQueue();
    }

	dac.stopStream();
    std::cout << "Finished!";
    return 0;
}
