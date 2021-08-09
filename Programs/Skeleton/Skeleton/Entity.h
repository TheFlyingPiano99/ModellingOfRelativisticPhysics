#pragma once

#include <string>


/*
* Base class of all named and descriptable entity.
* Has an ID.
*/
class Entity
{
	int ID;
	static int nextID;
	std::string name;
	std::string description;
protected:
	Entity(std::string _name = "", std::string _desc = "") : name(_name), description(_desc) {
		ID = nextID++;
	}

	int getID() {
		return ID;
	}

	const int getID() const {
		return ID;
	}

	std::string getTytle() {
		return name;
	}

	const std::string getTytle() const {
		return name;
	}

	std::string getDescription() {
		return description;
	}

	const std::string getDescription() const {
		return description;
	}

	void setDescription(std::string desc) {
		description = desc;
	}
};

