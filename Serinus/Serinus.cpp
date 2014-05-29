#include "Serinus.h"
#include "Engine.h"
#include <chrono>
#include <thread>

static int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                         double streamTime, RtAudioStreamStatus status, void *userData) {

    Engine *engine = static_cast<Engine*>(userData);
    float *outBuf = static_cast<float*>(outputBuffer);
    (void)inputBuffer; // Prevent unused variable warning.
    engine->FillAudioBuffers();
    for (unsigned int i = 0; i < nBufferFrames; i++) {
        *outBuf++ = engine->getSample(i);
    }
    return 0;
}

static void midiMonitor(double deltatime, std::vector<unsigned char> *message, void *userData) {
    Engine* engine = (Engine*)userData;
    engine->PushCommand(*message);
}

static bool isDone = false;
void siginthandler(int param) { isDone = true; }

int main(int argc, char* args[]) {
    std::cout << "Testing... Hit Ctrl+C to quit. \n" << std::endl;
    signal(SIGINT, siginthandler);
    Engine engine;
    RtAudio dac;
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 1;
    parameters.firstChannel = 0;

    //RtAudio::StreamOptions* options = new RtAudio::StreamOptions();
    //options->flags |= RTAUDIO_SCHEDULE_REALTIME | RTAUDIO_MINIMIZE_LATENCY;
    //options->numberOfBuffers = 4;
    //options->priority = 0;

    unsigned int bufferFrames = kBufferSize; //cannot be const, a hack for RtAudio

    dac.openStream(&parameters, NULL, RTAUDIO_FLOAT32,
                   kSampleRate, &bufferFrames, &audioCallback, (void *)&engine);
    dac.startStream();

    RtMidiIn* midiin = new RtMidiIn();

    try {
        midiin->openPort(0);
        midiin->ignoreTypes(false, false, false);
        midiin->setCallback(&midiMonitor, &engine);
    } catch (RtMidiError &error) {
        error.printMessage();
    }

    while (!isDone) {
        engine.HandleCommandQueue();
    }

    midiin->cancelCallback();
    midiin->closePort();  
    dac.stopStream();
    std::cout << "Finished!";
    return 0;
}
