#include "Object.h"

#include "Assets.h"
#include "StringOperations.h"

/*
* Factory for Earth-like object.
*/
Object* Object::createEarth(WorldLine* wrdln) {
	AdvancedTexture* texture = new AdvancedTexture(Assets::getTexturePath().append("earth_daymap.bmp").c_str(), Assets::getTexturePath().append("earth_normal_map.bmp").c_str() , "");
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
	Material* diagramM = new Material(vec3(3, 1.5, 1), vec3(0.5, 0.3, 0.2), vec3(5, 6, 20), 50);
	Object* obj = new Object(
		vec3(1, 1, 1),
		0.0f,
		0.0f,
		vec3(0, 0, 0),
		vec3(0, 0, 1),
		wrdln,
		Assets::getCubeGeometry(),
		new Material(vec3(3, 1.5, 1), vec3(10, 10, 10), vec3(5, 6, 20), 50),		// RealTime3D material
		diagramM,		// Diagram material
		new AdvancedTexture(Assets::getTexturePath().append("dice.bmp").c_str(), "", ""),
		"Dice",
		"It's a cube!");
	obj->setType(dice);
	return obj;
}

Object* Object::createSpaceship(WorldLine* wrdln)
{
	Material* diagramM = new Material(vec3(3, 1.5, 1), vec3(0.1, 0.5, 0.5), vec3(5, 6, 20), 50);
	Object* obj = new Object(
		vec3(1, 1, 1),
		0.0f,
		0.0f,
		vec3(0, 0, 0),
		vec3(0, 0, 1),
		wrdln,
		Assets::getSpaceshipGeometry(),
		new Material(vec3(3, 1.5, 1), vec3(10, 10, 10), vec3(5, 6, 20), 80),		// RealTime3D material
		diagramM,		// Diagram material
		new AdvancedTexture(Assets::getTexturePath().append("spaceship.bmp").c_str(), "", ""),
		"Spaceship",
		"Wruuuhm!!!");
	obj->setType(spaceship);
	return obj;
}

void Object::Animate(float dt) {
	//Todo
}

void Object::Draw(GPUProgram& gpuProgram, Camera& camera, Intersectable& intersectable, bool doLorentz) {
	//geometry->updateBeforeDraw(camera.getVelocityFV(), camera.getLocationFV(), *worldLine);

	worldLine->loadOnGPU(gpuProgram);
	if (selected) {
		Assets::getSelectedObjectMaterial()->loadOnGPU(gpuProgram);
	}
	else if (hovered) {
		Assets::getHoveredObjectMaterial()->loadOnGPU(gpuProgram);
	}
	else {
		material->loadOnGPU(gpuProgram);
	}
	if (texture != nullptr) {
		texture->loadOnGPU(gpuProgram);
	}
	gpuProgram.setUniform(camera.V() * camera.P(), "MVP");
	//gpuProgram.setUniform(M(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(texture == nullptr, "noTexture");
	gpuProgram.setUniform(false, "outline");
	gpuProgram.setUniform(false, "directRenderMode");

	geometry->Draw();

	// Caption:
	if (selected || hovered) {		
		vec3 pos = perceivedPosition(&intersectable, doLorentz, camera.getLocationFV(), camera.getStartPosVF(), camera.getVelocityFV());
		(*diagramCaption)->setPos(pos + normalize(camera.getRight() + camera.getUp()) * (geometry->getOverallRadius() + 0.3f));
		(*diagramCaption)->setVisible(true);
	}
	else {
		(*diagramCaption)->setVisible(false);
	}
}

void Object::DrawDiagram(GPUProgram& gpuProgram, Camera& camera, const Intersectable& intersectable, const ObserverProperties& observerProperties, const Settings& settings) {
	worldLine->loadOnGPU(gpuProgram);
	if (selected) {
		Assets::getSelectedWorldLineMaterial()->loadOnGPU(gpuProgram);
	}
	else if (hovered) {
		Assets::getHoveredWorldLineMaterial()->loadOnGPU(gpuProgram);
	}
	else {
		diagramMaterial->loadOnGPU(gpuProgram);
	}

	gpuProgram.setUniform(camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "outline");
	gpuProgram.setUniform(false, "directRenderMode");
	gpuProgram.setUniform(true, "glow");

	worldLine->DrawDiagram();
	
	float t = worldLine->intersect(intersectable);
	vec4 pos;
	if (settings.transformToProperFrame) {
		if (settings.doLorentz) {
			pos = RelPhysics::lorentzTransformation(
				worldLine->getLocationAtAbsoluteTime(t) - observerProperties.locationAtZero,
				RelPhysics::To3DVelocity(observerProperties.velocity));
		}
		else {
			pos = RelPhysics::galileanTransformation(
				worldLine->getLocationAtAbsoluteTime(t) - observerProperties.locationAtZero,
				RelPhysics::To3DVelocity(observerProperties.velocity));
		}
	}
	else {
		pos = worldLine->getLocationAtAbsoluteTime(t);
	}

	vec3 pos3 = vec3(pos[settings.diagramX], pos[settings.diagramY], pos[settings.diagramZ]);
	gpuProgram.setUniform(ScaleMatrix(vec3(0.5f, 0.5f, 0.5f)) * TranslateMatrix(pos3) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(ScaleMatrix(vec3(0.5f, 0.5f, 0.5f)) * TranslateMatrix(pos3), "M");
	gpuProgram.setUniform(TranslateMatrix(-pos3), "invM");
	gpuProgram.setUniform(true, "directRenderMode");
	gpuProgram.setUniform(false, "glow");

	Assets::getObserverNodeGeometry()->Draw();
	if (selected || hovered) {
		(*diagramCaption)->setPos(pos3);
		(*diagramCaption)->setVisible(true);
	}
	else {
		(*diagramCaption)->setVisible(false);
	}
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
			case spaceship:
				retVal = createSpaceship(NULL);
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

float Object::rayDistanceToObject(const Ray& ray, Intersectable* intersectable, bool doLorentz, vec4 observerCurrentLocation, vec4 observerLocationAtZero, vec4 observersCurrentVelocity)
{
	vec3 locationInProperFrame = perceivedPosition(intersectable, doLorentz, observerCurrentLocation, observerLocationAtZero, observersCurrentVelocity);
	vec3 rayPos = vec3(0, 0, 0);		// We use origo instead of the position given in absolute frame, because it would be transformed to origo anyway.
	float d = length(locationInProperFrame - rayPos - dot(ray.dir, locationInProperFrame - rayPos) * ray.dir);
	return (dot(locationInProperFrame - rayPos, ray.dir) > 0) ? d : -1;		// If it's behod the camera, than return -1.
}

vec3 Object::perceivedPosition(Intersectable* intersectable, bool doLorentz, vec4 observerCurrentLocation, vec4 observerLocationAtZero, vec4 observersCurrentVelocity)
{
	//Intersect:
	float t = 0;		// absolute time parametre

	t = worldLine->intersect(*intersectable);

	vec3 locationInProperFrame;

	if (doLorentz) {
		vec4 shiftedOrigoFrame = worldLine->getLocationAtAbsoluteTime(t) - observerLocationAtZero;	// The absolute observers frame, but with shifted origo.
		vec4 location4D = RelPhysics::lorentzTransformation(shiftedOrigoFrame, RelPhysics::To3DVelocity(observersCurrentVelocity));	// In the current observer's frame.
		locationInProperFrame = vec3(location4D.x, location4D.y, location4D.z);
	}
	else {			// Euclidean transformation
		vec4 location4D = RelPhysics::galileanTransformation(worldLine->getLocationAtAbsoluteTime(t) - observerCurrentLocation, RelPhysics::To3DVelocity(observersCurrentVelocity));
		locationInProperFrame = vec3(location4D.x, location4D.y, location4D.z);
	}

	return locationInProperFrame;
}

void Object::hover()
{
}

