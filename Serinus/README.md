Software Synthesizer Project. Codename: Serinus
===================

This is a test repository for the core of the digital synth.
Currently, it uses RtMidi and RtAudio libraries as HAL, so please refer to the projects for the license information:<br>
https://github.com/thestk/rtmidi <br>
https://github.com/thestk/rtaudio <br>

Some interesting files to look up: <br>
* Engine.cpp
* PathModule.h - a base class for each module
* Serinus.h - contains a work log of the project

TODO
===================
1. Switch to fixed point <8,24>
2. Rework serialization
3. Add xml import/export
4. Add sub-patches to the modules

Big Goals
===================

1. Multi-timbral support
2. Hardware IO registry
3. Filesystem functions
4. Scheduler
5. ...

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
