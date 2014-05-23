#ifndef _CREATOR_H_
#define _CREATOR_H_

#include <string>

class PatchModule;

class Creator {
public:
    Creator(const std::string& classname);
    virtual ~Creator() {};
    virtual PatchModule* create(int maxPoly, int bufferSize) = 0;
};

#endif //_CREATOR_H_

