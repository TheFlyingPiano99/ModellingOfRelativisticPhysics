#include "Object.h"

#include "Assets.h"
#include "StringOperations.h"


const char* Object::typeNames[5] = {
	"earth",
	"dice",
	"spaceship",
	"spike",
	"none"
};


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
	obj->setType(RelTypes::ObjectType::earth);
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
	obj->setType(RelTypes::ObjectType::dice);
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
		new Material(vec3(3, 3, 3), vec3(10, 10, 10), vec3(5, 6, 5), 80),		// RealTime3D material
		diagramM,		// Diagram material
		new AdvancedTexture(Assets::getTexturePath().append("spaceship.bmp").c_str(), "", ""),
		"Spaceship",
		"Wruuuhm!!!");
	obj->setType(RelTypes::ObjectType::spaceship);
	return obj;
}

Object* Object::createSpike(WorldLine* wrdln)
{
	Material* diagramM = new Material(vec3(3, 1.5, 1), vec3(0.5, 0.3, 0.2), vec3(5, 6, 20), 50);
	Object* obj = new Object(
		vec3(1, 1, 1),
		0.0f,
		0.0f,
		vec3(0, 0, 0),
		vec3(0, 0, 1),
		wrdln,
		Assets::getSpikeGeometry(),
		new Material(vec3(1, 1, 1), vec3(10, 10, 10), vec3(5, 5, 5), 50),		// RealTime3D material
		diagramM,		// Diagram material
		new AdvancedTexture(Assets::getTexturePath().append("spike.bmp").c_str(), "", ""),
		"Spike",
		"Used to visualise Wigner rotation.");
	obj->setType(RelTypes::ObjectType::spike);
	return obj;
}

mat4 Object::getModellMatrix() {
	return ScaleMatrix(scale)
		* RotationMatrix(rotationAngle, rotationAxis)
		* TranslateMatrix(translation);
}

mat4 Object::getInverseModellMatrix() {
	return TranslateMatrix(-translation)
		* RotationMatrix(-rotationAngle, rotationAxis)
		* ScaleMatrix(vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
}

vec3 Object::getDiagramPos(const LightCone& lightCone, const Hyperplane& hyperplane, const RelTypes::Settings& settings, const RelTypes::ObserverProperties& observerProperties) {
	// Transformation mode interpolation:
	vec4 absoluteFramePos4 = intersect(lightCone, hyperplane, settings,
		observerProperties);

	vec3 absoluteFramePos = vec3(absoluteFramePos4[settings.diagramX], absoluteFramePos4[settings.diagramY], absoluteFramePos4[settings.diagramZ]);
	vec4 properFramePos4 = intersect(lightCone,
		hyperplane, 
		settings,
		observerProperties);
	vec3 properFramePos = 
		vec3(properFramePos4[settings.diagramX],
		properFramePos4[settings.diagramY],
		properFramePos4[settings.diagramZ]);

	// "Transformation to proper frame" interpolation:
	vec3 pos = interpolate<bool, vec3>(settings.transformToProperFrame, true, false, properFramePos, absoluteFramePos);
	return pos;
}

void Object::animate(float dt) {
	//Todo
}

void Object::draw(GPUProgram& gpuProgram,
	const Camera& camera, 
	const LightCone& lightCone,
	const Hyperplane& hyperplane, const RelTypes::ObserverProperties& observerProperties,
	const RelTypes::Settings& settings) {
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
	gpuProgram.setUniform(camera.getViewMatrix() * camera.getActiveProjectionMatrix(), "MVP");
	//gpuProgram.setUniform(M(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(texture == nullptr, "noTexture");
	gpuProgram.setUniform(false, "outline");
	gpuProgram.setUniform(false, "directRenderMode");

	geometry->draw();

	// Caption:
	if (selected || hovered) {		
		vec4 pos4 = intersect(lightCone,
			hyperplane,
			settings, 
			observerProperties);
		vec3 pos = vec3(pos4.x, pos4.y, pos4.z);
		(*diagramCaption)->setPos(pos + normalize(camera.getRight() + camera.getUp()) * (geometry->getOverallRadius() + 0.3f));
		(*diagramCaption)->setVisible(true);
		float t;
		if (settings.intersectionMode.get() == RelTypes::IntersectionMode::lightCone) {
			t = worldLine->intersect(lightCone);
		}
		else if (settings.intersectionMode.get() == RelTypes::IntersectionMode::hyperplane) {
			t = worldLine->intersect(hyperplane);
		}
		float tau;
		if (settings.doLorentz.get()) {
			tau = worldLine->getProperTimeAtAbsoluteTime(t);
		}
		else {
			tau = t;
		}
		float vLorentz = length(RelPhysics::lorentzTransformationOfVelocity(RelPhysics::To3DVelocity(worldLine->getVelocityAtAbsoluteTime(t)), RelPhysics::To3DVelocity(observerProperties.velocity)));
		float vGalilean = length(RelPhysics::galileanTransformationOfVelocity(RelPhysics::To3DVelocity(worldLine->getVelocityAtAbsoluteTime(t)), RelPhysics::To3DVelocity(observerProperties.velocity)));
		float v = interpolate(settings.doLorentz, true, false, vLorentz, vGalilean);
		(*diagramCaption)->changeText(getName().append("\n").append("age = ").append(std::to_string(tau)).append(" m\n").append("v = ").append(std::to_string(v)).append("c").c_str());
	}
	else {
		(*diagramCaption)->setVisible(false);
	}
	if (worldLineView != nullptr) {
		worldLineView->disableEditorInfo(gpuProgram, camera, settings);
	}
}

void Object::drawDiagram(GPUProgram& gpuProgram, const Camera& camera, const LightCone& lightCone, const Hyperplane& hyperplane, const RelTypes::ObserverProperties& observerProperties, const RelTypes::Settings& settings) {
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

	gpuProgram.setUniform(camera.getTranslationMatrix() * camera.getViewMatrix() * camera.getActiveProjectionMatrix(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "outline");
	gpuProgram.setUniform(false, "directRenderMode");
	gpuProgram.setUniform(true, "glow");

	if (worldLineView != nullptr) {
		worldLineView->drawDiagram();
	}
	
	
	vec3 pos = getDiagramPos(lightCone, hyperplane, settings, observerProperties);
	gpuProgram.setUniform(ScaleMatrix(vec3(0.5f, 0.5f, 0.5f)) * TranslateMatrix(pos) * camera.getTranslationMatrix() * camera.getViewMatrix() 
		* camera.getActiveProjectionMatrix(), "MVP");
	gpuProgram.setUniform(ScaleMatrix(vec3(0.5f, 0.5f, 0.5f)) * TranslateMatrix(pos), "M");
	gpuProgram.setUniform(TranslateMatrix(-pos), "invM");
	gpuProgram.setUniform(true, "directRenderMode");
	gpuProgram.setUniform(false, "glow");

	Assets::getObserverNodeGeometry()->draw();
	if (selected || hovered) {
		(*diagramCaption)->setPos(pos);
		(*diagramCaption)->setVisible(true);
		float t;
		if (settings.intersectionMode.get() == RelTypes::IntersectionMode::lightCone) {
			t = worldLine->intersect(lightCone);
		}
		else if (settings.intersectionMode.get() == RelTypes::IntersectionMode::hyperplane) {
			t = worldLine->intersect(hyperplane);
		}
		float tau;
		if (settings.doLorentz.get()) {
			tau = worldLine->getProperTimeAtAbsoluteTime(t);
		}
		else {
			tau = t;
		}
		float v = length(RelPhysics::To3DVelocity(observerProperties.velocity) - RelPhysics::To3DVelocity(worldLine->getVelocityAtAbsoluteTime(t)));
		(*diagramCaption)->changeText(getName().append("\n").append("age = ").append(std::to_string(tau)).append(" m\n").append("v = ").append(std::to_string(v)).append("c").c_str());
	}
	else {
		(*diagramCaption)->setVisible(false);
	}
	if (settings.editorMode && selected && worldLineView != nullptr) {
		worldLineView->drawEditorInfoDiagram(gpuProgram, camera, settings);
	}
	else if (worldLineView != nullptr) {
		worldLineView->disableEditorInfo(gpuProgram, camera, settings);
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
	RelTypes::ObjectType _type;

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
			case RelTypes::ObjectType::earth:
				retVal = createEarth(NULL);
				break;
			case RelTypes::ObjectType::dice:
				retVal = createDice(NULL);
				break;
			case RelTypes::ObjectType::spaceship:
				retVal = createSpaceship(NULL);
				break;
			case RelTypes::ObjectType::spike:
				retVal = createSpike(NULL);
				break;
			case RelTypes::ObjectType::none:
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
			_type = (RelTypes::ObjectType)std::stoi(words.at(1));
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

void Object::setWorldLine(std::map<int, WorldLine*>& worldLines) {
	worldLine = worldLines.at(worldLineID);
	if (worldLine != nullptr) {
		worldLineView = (WorldLineView*)worldLine->createView();
	}
}

/*
* Returns shortest distance between given ray and the object's world line's diagram representation.
*/

float Object::rayDistanceToDiagram(const Ray& ray, RelTypes::ObserverProperties observerProperties, const RelTypes::Settings& settings) {
	return worldLine->distanceBetweenRayAndDiagram(ray, observerProperties, settings);
}

float Object::rayDistanceToObject(const Ray& ray, 
	const LightCone& lightCone, 
	const Hyperplane& hyperplane,
	const RelTypes::Settings& settings,
	const RelTypes::ObserverProperties& observerProperties)
{
	vec4 location4 = intersect(lightCone, hyperplane, settings, observerProperties);
	vec3 wPos = vec3(location4.x, location4.y, location4.z);	// only space coordinates!
	vec3 rayPos = ray.pos - vec3(observerProperties.location.x, observerProperties.location.y, observerProperties.location.z);
	float d = length(wPos - rayPos - dot(ray.dir, wPos - rayPos) * ray.dir);
	return (dot(wPos - rayPos, ray.dir) > 0) ? d : -1;		// If it's behod the camera, than return -1.
}

vec4 Object::intersect(const LightCone& lightCone, const Hyperplane& hyperplane, const RelTypes::Settings& settings, const RelTypes::ObserverProperties& observerProperties)
{
	float t = 0;		// absolute time parametre
	
	float tLightCone = worldLine->intersect(lightCone);
	float tHyperplane = worldLine->intersect(hyperplane);
	t = interpolate<RelTypes::IntersectionMode, float>(
		settings.intersectionMode,
		RelTypes::IntersectionMode::lightCone,
		RelTypes::IntersectionMode::hyperplane,
		tLightCone,
		tHyperplane
		);

	return interpolate<bool, vec4>(
		settings.doLorentz,
		true,
		false,
		RelPhysics::lorentzTransformation(worldLine->getLocationAtAbsoluteTime(t) - observerProperties.locationAtZero, RelPhysics::To3DVelocity(observerProperties.velocity)),
		RelPhysics::galileanTransformation(worldLine->getLocationAtAbsoluteTime(t) - observerProperties.locationAtZero, RelPhysics::To3DVelocity(observerProperties.velocity))
		);
}

void Object::hover()
{
}

void Object::draggedTo(vec4 location)
{
	worldLine->draggedTo(location);
	worldLineView->update();
	worldLineView->updateGeometry();
}

vec4 Object::getClosestLocation(const Ray& ray, const RelTypes::ObserverProperties& observerProperties, const RelTypes::Settings& settings)
{
	return worldLine->getClosestLocation(ray, observerProperties, settings);
}

