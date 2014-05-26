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
1. Switch to fixed point <8,24> and add constant, non-buffered signals (generator and controller devision)
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
| What | Latency | Buffer size |
|---------|---------|---------|
| vocals | 3 ms | 256 |
| drums | 6 ms  | 512 |
| guitar  | 12 ms  | 1024 |
| keys  | 10-20 ms  | 1024-2048 |
