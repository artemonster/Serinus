#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <string>
#include <map>
#include "CreatorImpl.h"

class PatchModule;
class Creator;

class Factory {
public:
    static PatchModule* create(const std::string& classname, int maxPoly, int bufferSize);
    static void registerit(const std::string& classname, Creator* creator);
private:
    static std::map<std::string, Creator*>& get_table();
};


#endif
