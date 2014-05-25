#include "RtMidi.h"
#include "signal.h"
#include <vector>
#include <chrono>
#include <thread>
static bool isDone = false;
void siginthandler(int param) { isDone = true; }

int main() {
    RtMidiOut *midiout = new RtMidiOut();
    std::vector<unsigned char> message;
    std::string portName;
    int nPorts = midiout->getPortCount();
    for ( unsigned int i=0; i<nPorts; i++ ) {
    try {
        portName = midiout->getPortName(i);
    } catch (RtMidiError &error) {
        error.printMessage();
    }
        std::cout << "  Output Port #" << i << ": " << portName << '\n';
    }
    std::cout <<"\n Please enter the number of LoopMidi port to open:";
    int port; 
    std::cin>>port;
    midiout->openPort( port );
    std::cout <<"\n Press Ctrl+C to exit";
    signal(SIGINT, siginthandler);
    while (!isDone) {
        std::cout <<"\n Press n to send note on, press f to send note off\n";
        char in;
        std::cin >> in;
        if (in == 'n') {
            message.push_back(144);
            message.push_back(64);
            message.push_back(90);
            midiout->sendMessage( &message );
            message.clear();
        } else if (in == 'f') {
            message.push_back(128);
            message.push_back(64);
            message.push_back(40);
            midiout->sendMessage( &message );
            message.clear();
        } else {
            std::cout <<"Unrecognized input\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
    delete midiout;
    return 0;
}

