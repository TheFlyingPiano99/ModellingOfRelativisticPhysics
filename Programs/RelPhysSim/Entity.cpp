#include "Entity.h"

int Entity::nextID = 0;

void Entity::select()
{
	selected = true;
}

void Entity::deselect()
{
	selected = false;
}

void Entity::hover()
{
	hovered = true;
}

void Entity::endHover()
{
	hovered = false;
}
