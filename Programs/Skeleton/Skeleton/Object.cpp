#include "Object.h"

#include "Assets.h"
#include "StringOperations.h"

/*
* Factory for Earth-like object.
*/

Object* Object::createEarth(WorldLine* wrdln) {
	AdvancedTexture* texture = new AdvancedTexture("../../../Resources/lowres/earth_daymap.bmp", "../../../Resources/lowres/earth_normal_map.bmp", "");
	if (texture->getTextureId() == 0) {
		texture = NULL;
	}
	Material* diagramM = new Material(vec3(3, 1.5, 1), vec3(0, 0.3, 0.6), vec3(5, 6, 20), 50);
	diagramM->setGlow(true);
	ParamSurface* pSurface = new SphereSurface(0.5f);
	pSurface->GenSurface(100, 100);
	Object* obj = new Object(
		vec3(1.0f, 1.0f, 1.0f),
		0.0f,
		0.0f,
		vec3(2.5f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		wrdln,
		pSurface,
		new Material(vec3(3, 1.5, 1), vec3(10, 10, 10), vec3(5, 6, 20), 50),		// RealTime3D material
		diagramM,		// Diagram material
		texture,
		"Earth",
		"Inhabited planet"
	);
	obj->setType(earth);
	return obj;
}

Object* Object::createDice(WorldLine* wrdln)
{
	 Object* obj = new Object(
		vec3(1, 1, 1),
		0.0f,
		0.0f,
		vec3(0, 0, 0),
		vec3(0, 0, 1),
		wrdln,
		Assets::getCubeGeometry(),
		new Material(vec3(3, 1.5, 1), vec3(10, 10, 10), vec3(5, 6, 20), 50),		// RealTime3D material
		new Material(vec3(0.5f, 0.5f, 0.5f), vec3(0.8f, 0.8f, 0.8f), vec3(0.5f, 0.5f, 0.5f), 40, 1.0f),		// Diagram material
		new AdvancedTexture("../../../Resources/lowRes/dice.bmp", "", ""),
		"Dice",
		"It's a cube!");
	obj->setType(dice);
	return obj;
}

void Object::Animate(float dt) {
	//Todo
}

void Object::Draw(GPUProgram& gpuProgram, Camera& camera) {
	//geometry->updateBeforeDraw(camera.getVelocityFV(), camera.getLocationFV(), *worldLine);

	worldLine->loadOnGPU(gpuProgram);
	material->loadOnGPU(gpuProgram);
	if (texture != nullptr) {
		texture->loadOnGPU(gpuProgram);
	}
	gpuProgram.setUniform(camera.V() * camera.P(), "MVP");
	//gpuProgram.setUniform(M(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(texture == nullptr, "noTexture");

	geometry->Draw();
}

void Object::DrawDiagram(GPUProgram& gpuProgram, Camera& camera) {
	if (selected) {
		Assets::getSelectedObjectMaterial()->loadOnGPU(gpuProgram);
	}
	else {
		diagramMaterial->loadOnGPU(gpuProgram);
	}

	gpuProgram.setUniform(camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(true, "noTexture");

	worldLine->Draw();

}

std::string Object::genSaveString()
{
	std::string str(
		worldLine->genSaveString() + "\n"
		"Object\n"
		"type " + std::to_string(type) + "\n"
		"ID " + std::to_string(getID()) + "\n"
		"name " + name + "\n"
		"description " + description + "\n"
		"worldLine " + std::to_string(worldLine->getID()) + "\n"
		"!Object\n"
	);
	return str;
}

Object* Object::loadFromFile(std::ifstream& file)
{
	int _ID = -1;
	std::string _name;
	std::string _description;
	int _worldLine = -1;
	ObjectType _type;

	std::string line;
	while (getline(file, line)) {
		std::vector<std::string> words = split(line, ' ');
		if (words.empty()) {									// Empty line
			continue;
		}
		else if (words.at(0).at(0) == '#') {					// Comment
			continue;
		}
		else if (words.at(0).compare("!Object") == 0) {	// End of declaration
			Object* retVal = NULL;
			switch (_type)
			{
			case earth:
				retVal = createEarth(NULL);
				break;
			case dice:
				retVal = createDice(NULL);
				break;
			case none:
				break;
			default:
				break;
			}
			retVal->setID(_ID);
			retVal->setName(_name);
			retVal->setDescription(_description);
			retVal->worldLineID = _worldLine;
			return retVal;
		}
		else if (words.at(0).compare("type") == 0) {              // type
			_type = (ObjectType)std::stoi(words.at(1));
		}
		else if (words.at(0).compare("ID") == 0) {              // ID
			_ID = std::stoi(words.at(1));
		}
		else if (words.at(0).compare("name") == 0) {              // name
			_name = join(words, 1);
		}
		else if (words.at(0).compare("description") == 0) {         // description
			_description = join(words, 1);
		}
		else if (words.at(0).compare("worldLine") == 0) {         // worldLine
			_worldLine = stoi(words[1]);
		}
	}
	return nullptr;
}
