#ifndef _CREATORIMPL_H_
#define _CREATORIMPL_H_

#include <string>

#include "Creator.h"

template <class T>
class CreatorImpl : public Creator
{
public:
    CreatorImpl<T>(const std::string& classname) : Creator(classname) {}
    virtual ~CreatorImpl<T>() {}

    virtual PatchModule* create() { return new T; }
};

#endif