Software Synthesizer Project. Codename: Serinus
===================

This is a test repository for the core of the digital synth.
Currently, it uses portaudio and RtMidi libraries (will be discarded later).

TODO
===================
1. Decide on patch configuration format
2. Decide on module configuration format
3. Add command class and implement handlers (maybe also class-specific, directly embedded in respective class)
4. Populate some modules so we have something to play with (weighted mixers, effects, lfo, oscillators)
5. Add midi device

Latency Table
===================
| What | Latency |
|---------|-------|
| vocals | 3 ms |
| drums | 6 ms  |
| guitar  | 12 ms  |
| keys  | 10-20 ms  |

Installation
===================
0. Install MinGW (base, g++)
1. Install Eclipse CDT
2. Import a project from GIT
3. Download portaudio library
4.  Copy \lib\to mingw\lib and \include to \include\

Useful guidelines
===================
* Always include header guards.
* Do not declare variables in header files unless they are constants. Header files should generally only be used for declarations.
* Do not define functions in header files unless they are trivial. Doing so makes your header files harder for humans to read.
* Each header file should have a specific job, and be as independent as possible. For example, you might put all your declarations related to functionality A in A.h and all your declarations related to functionality B in B.h. That way if you only care about A later, you can just include A.h and not get any of the stuff related to B.
* Try to include as few other header files as possible in your header files.
