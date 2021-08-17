#include "Observer.h"
#include "Geometry.h"
#include "Assets.h"
#include "StringOperations.h"

vec4 Observer::getLocation()
{
    return worldLine->getLocationAtProperTime(currentProperTime);
}

vec4 Observer::getVelocity()
{
    return worldLine->getVelocityAtProperTime(currentProperTime);
}

vec4 Observer::getStartPos()
{
	return worldLine->getLocationAtAbsoluteTime(0.0f);
}

Hyperplane Observer::getHyperplane()
{
	return worldLine->getSimultaneousHyperplaneAtProperTime(currentProperTime);
}

void Observer::DrawDiagram(GPUProgram& gpuProgram, Camera& camera) {
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");

	worldLine->Draw();
}

void Observer::DrawHyperplane(GPUProgram& gpuProgram, Camera& camera)
{
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(getLocation().x, getLocation().y, getLocation().w)), "invM");
	gpuProgram.setUniform(false, "glow");
	gpuProgram.setUniform(true, "noTexture");
	PlaneSurface* plane = new PlaneSurface(normalize(vec3(-getVelocity().x, -getVelocity().y, getVelocity().w)), 100, 100);
	plane->GenSurface(20, 20);
	glDisable(GL_CULL_FACE);
	plane->Draw();
	glEnable(GL_CULL_FACE);
}

void Observer::DrawLightCone(GPUProgram& gpuProgram, Camera& camera)
{
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(getLocation().x, getLocation().y, getLocation().w)), "invM");
	gpuProgram.setUniform(false, "glow");
	gpuProgram.setUniform(true, "noTexture");
	glDisable(GL_CULL_FACE);
	Assets::getLightConeGeomtry()->Draw();
	glEnable(GL_CULL_FACE);
}

void Observer::DrawNode(GPUProgram& gpuProgram, Camera& camera)
{
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(getLocation().x, getLocation().y, getLocation().w)), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");
	Assets::getObserverNodeGeometry()->Draw();
}

void Observer::DrawExtras(GPUProgram& gpuProgram, Camera& camera)
{
	DrawLightCone(gpuProgram, camera);
	DrawHyperplane(gpuProgram, camera);
	DrawNode(gpuProgram, camera);
}

void Observer::setCurrentTimeAtAbsoluteTime(float t)
{
	currentProperTime = worldLine->getProperTimeAtAbsoluteTime(t);
}

float Observer::getAbsoluteTimeAtCurrentTime()
{
	return worldLine->getAbsoluteTimeAtProperTime(currentProperTime);
}

void Observer::increaseTimeByDelta(float deltaTau)
{
	currentProperTime += deltaTau;
}

void Observer::syncCamera(Camera* camera)
{
	camera->syncToObserver(
		getLocation(),
		getVelocity(),
		getStartPos());
}

void Observer::syncTimeToObserversSimultaneity(Observer& observer)
{
	Hyperplane plane = observer.getHyperplane();
	float t = worldLine->intersectHyperplane(plane);
	currentProperTime = worldLine->getProperTimeAtAbsoluteTime(t);
}

void Observer::loadOnGPU(GPUProgram& gpuProgram)
{
	gpuProgram.setUniform(getVelocity(), "observersVelocity");
	gpuProgram.setUniform(getLocation(), "observersLocation");
	gpuProgram.setUniform(getStartPos(), "observersStartPos");
}

std::string Observer::genSaveString()
{
	std::string str(
		"Observer\n"
		"ID " + std::to_string(getID()) + "\n"
		"name " + name + "\n"
		"description " + description + "\n"
		"worldLine " + std::to_string(worldLine->getID()) + "\n"
		"!Observer\n"
	);
	return str;
}

Observer* Observer::loadFromFile(std::ifstream& file)
{
	int _ID = -1;
	std::string _name;
	std::string _description;
	int _worldLine = -1;

	std::string line;
	while (getline(file, line)) {
		std::vector<std::string> words = split(line, ' ');
		if (words.empty()) {									// Empty line
			continue;
		}
		else if (words.at(0).at(0) == '#') {					// Comment
			continue;
		}
		else if (words.at(0).compare("!Observer") == 0) {	// End of declaration
			Observer* retVal = new Observer(NULL, _name, _description);
			retVal->setID(_ID);
			return retVal;
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
