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

protected:

	std::string name;
	std::string description;

	Entity(std::string _name = "", std::string _desc = "") : name(_name), description(_desc) {
		ID = nextID++;
	}

	/*
	* Ensures, that the nextID is always greater than the ID of any object.
	*/
	void setID(int id) {
		ID = id;
		if (ID >= nextID) {
			nextID = ID + 1;
		}
	}

public:

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
		this->description = desc;
	}

	void setName(std::string name) {
		this->name = name;
	}

	virtual std::string genSaveString() = 0;
};

