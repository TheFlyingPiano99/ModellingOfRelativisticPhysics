#include "Event.h"

void Event::hover()
{
}

std::string Event::genSaveString()
{
    return std::string();
}

void Event::draggedTo(vec4 location)
{
    this->location = location;
}

vec4 Event::getReferenceLocation()
{
    return location;
}
