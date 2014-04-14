Software Synthesizer Project. Codename: Serinus
===================

This is a test repository for the core of the digital synth.
Currently, it uses RtMidi and RtAudio libraries as HAL, so please refer to the projects for the license information:
https://github.com/thestk/rtmidi
https://github.com/thestk/rtaudio


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

Useful guidelines
===================
* Always include header guards.
* Do not declare variables in header files unless they are constants. Header files should generally only be used for declarations.
* Do not define functions in header files unless they are trivial. Doing so makes your header files harder for humans to read.
* Each header file should have a specific job, and be as independent as possible. For example, you might put all your declarations related to functionality A in A.h and all your declarations related to functionality B in B.h. That way if you only care about A later, you can just include A.h and not get any of the stuff related to B.
* Try to include as few other header files as possible in your header files.
