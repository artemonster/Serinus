#include "Creator.h"
#include "Factory.h"

Creator::Creator(const std::string& classname)
{
    Factory::registerit(classname, this);
}
