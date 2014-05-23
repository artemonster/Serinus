#include "Factory.h"
#include "Creator.h"

PatchModule* Factory::create(const std::string& classname, int maxPoly, int bufferSize) {
    std::map<std::string, Creator*>::iterator i;
    i = get_table().find(classname);

    if (i != get_table().end())
        return i->second->create(maxPoly, bufferSize);
    else
        return (PatchModule*)NULL;
}

void Factory::registerit(const std::string& classname, Creator* creator)
{
    get_table()[classname] = creator;
}

std::map<std::string, Creator*>& Factory::get_table()
{
    static std::map<std::string, Creator*> table;
    return table;
}
