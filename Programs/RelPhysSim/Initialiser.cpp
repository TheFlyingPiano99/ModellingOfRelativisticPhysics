#include "Initialiser.h"

bool Initialiser::finish(Object*& object, WorldLine*& worldLine)
{
    object = nullptr;
    worldLine = nullptr;
    if (objectType != RelTypes::ObjectType::none&& length(velocityAtZeroT) < 1.0f) {
        switch (worldLineType)
        {
        case WorldLine::Type::GeodeticLine:
            worldLine = new GeodeticLine(positionAtZeroT, velocityAtZeroT, "Geodetic line", "World line");
            break;
        case WorldLine::Type::CompositeLine:
            worldLine = new CompositeLine(positionAtZeroT, velocityAtZeroT, "Composite line", "World line");
            break;
        case WorldLine::Type::SpiralLine:
        {
            float d = dot(normalize(centerOfRotation - positionAtZeroT), normalize(velocityAtZeroT));
            if (length(velocityAtZeroT) > 0.0f
                && length(centerOfRotation - positionAtZeroT) > 0.0f
                && abs(d) < 0.001f) {
                worldLine = new SpiralLine(positionAtZeroT, centerOfRotation, velocityAtZeroT, "Spiral line", "World line");
            }
            break;
        }
        default:
            break;
        }

        if (worldLine == nullptr) {
            return false;
        }

        switch (objectType)
        {
        case RelTypes::ObjectType::dice:
            object = Object::createDice(worldLine);
            object->setName(name);
            break;
        case RelTypes::ObjectType::earth:
            object = Object::createEarth(worldLine);
            object->setName(name);
            break;
        case RelTypes::ObjectType::spaceship:
            object = Object::createSpaceship(worldLine);
            object->setName(name);
            break;
        case RelTypes::ObjectType::spike:
            object = Object::createSpike(worldLine);
            object->setName(name);
            break;
        default:
            break;
        }
        if (object == nullptr) {
            delete worldLine;
            worldLine = nullptr;
            return false;
        }
        return true;
    }
    return false;
}

RelTypes::ObjectType Initialiser::getObjectType() {
    return objectType;
}
