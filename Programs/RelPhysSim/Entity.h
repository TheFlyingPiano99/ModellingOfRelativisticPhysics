#pragma once


#include <string>

#include "Ray.h"
#include "EnumTypes.h"

/*
* Base class of all named and descriptable entity.
* Has an ID.
*/
class Entity
{
	int ID;
	static int maxID;

protected:

	bool selected = false;
	bool hovered = false;
	std::string name;
	std::string description;

	Entity(std::string _name = "", std::string _desc = "") : name(_name), description(_desc) {
		ID = ++maxID;
	}

	/*
	* Ensures, that the nextID is always greater than the ID of any object.
	*/
	void setID(int id) {
		ID = id;
		if (ID >= maxID) {
			maxID = ID;
		}
	}

public:

	int getID() {
		return ID;
	}

	const int getID() const {
		return ID;
	}

	std::string getName() {
		return name;
	}

	const std::string getName() const {
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

	/*
	* Entity gets selected.
	*/
	void select();
	void deselect();

	/*
	* The cursor is hovering over the entity.
	*/
	void hover();
	void endHover();

	/*
	* Entity is dragged by user.
	* Receives current mouse position on camera plane.
	*/
	virtual void draggedTo(vec4 location) = 0;

	/*
	* Return location in absolute frame, that is ideal to be a position on a plane that will contain the edited location.
	*/
	virtual vec4 getClosestLocation(const Ray& ray, const RelTypes::ObserverProperties& observerProperties, const RelTypes::Settings& settings) = 0;
};

