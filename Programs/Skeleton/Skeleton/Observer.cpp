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

vec4 Observer::getStartPos()	// Must be fixed!
{
	return worldLine->getLocationAtAbsoluteTime(0.0f);
}

Hyperplane* Observer::getHyperplane()
{
	return worldLine->getSimultaneousHyperplaneAtProperTime(currentProperTime);
}



LightCone* Observer::getLightCone()
{
	return worldLine->getLigtConeAtProperTime(currentProperTime);
}

void Observer::Draw(GPUProgram& gpuProgram, Camera& camera)
{
	(*diagramCaption)->setVisible(false);
	(*timerCaption)->setVisible(false);
}

void Observer::DrawDiagram(GPUProgram& gpuProgram, Camera& camera) {
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "outline");

	(*diagramCaption)->setVisible(false);
	(*timerCaption)->setVisible(false);

	worldLine->Draw();
}

void Observer::DrawHyperplane(GPUProgram& gpuProgram, Camera& camera)
{
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(0, 0, currentProperTime)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(0, 0, currentProperTime)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(0, 0, currentProperTime)), "invM");
	gpuProgram.setUniform(false, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "textMode");
	gpuProgram.setUniform(true, "directRenderMode");
	PlaneSurface* plane = new PlaneSurface(normalize(vec3(0, 0, 1)), 100, 100);
	plane->GenSurface(20, 20);
	glDisable(GL_CULL_FACE);
	plane->Draw();
	glEnable(GL_CULL_FACE);
}

void Observer::DrawLightCone(GPUProgram& gpuProgram, Camera& camera)
{
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(0, 0, currentProperTime)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(0, 0, currentProperTime)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(0, 0, currentProperTime)), "invM");
	gpuProgram.setUniform(false, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "textMode");
	gpuProgram.setUniform(true, "directRenderMode");
	glDisable(GL_CULL_FACE);
	Assets::getLightConeGeomtry()->Draw();
	glEnable(GL_CULL_FACE);
}

void Observer::DrawNode(GPUProgram& gpuProgram, Camera& camera)
{
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(0, 0, currentProperTime)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(0, 0, currentProperTime)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(0, 0, currentProperTime)), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(true, "directRenderMode");
	Assets::getObserverNodeGeometry()->Draw();
}

void Observer::DrawExtras(GPUProgram& gpuProgram, Camera& camera, IntersectionMode intersectionMode)
{
	if (intersectionMode == IntersectionMode::lightCone) {
		DrawLightCone(gpuProgram, camera);
	}
	else if (intersectionMode == IntersectionMode::hyperplane) {
		DrawHyperplane(gpuProgram, camera);
	}
	DrawNode(gpuProgram, camera);
	static int updateCounter = 0;
	static float prevTau = 0.0f;
	if (updateCounter++ > 30) {		// Reduced refresh rate.
		prevTau = currentProperTime;
		updateCounter = 0;
		(*timerCaption)->changeText(std::string("tau = ").append(std::to_string(prevTau)).append(" m\n")
			.append("t = ").append(std::to_string(worldLine->getAbsoluteTimeAtProperTime(currentProperTime))).append(" m").c_str());
	}
	vec4 pos = vec4(0,0, currentProperTime, 0);
	(*timerCaption)->setPos(vec3(pos.x, pos.y, pos.w) + camera.getRight() * 11 + camera.getPrefUp() * 1);
	(*timerCaption)->setVisible(selected);
	pos = getStartPos();
	(*diagramCaption)->setPos(vec3(pos.x, pos.y, pos.w));
	(*diagramCaption)->setVisible(true);
	(*timerCaption)->setVisible(true);
}

void Observer::setCurrentTimeAtAbsoluteTime(float t)
{
	currentProperTime = worldLine->getProperTimeAtAbsoluteTime(t);
}

float Observer::getAbsoluteTimeAtCurrentTime()
{
	return worldLine->getAbsoluteTimeAtProperTime(currentProperTime);
}

float Observer::increaseTimeByDelta(float deltaTau)
{
	if (currentProperTime + deltaTau < 0.0f) {		// Crop delta
		float realDelta = -currentProperTime;
		currentProperTime = 0.0f;
		return realDelta;
	}
	else {
		currentProperTime += deltaTau;
		return deltaTau;
	}
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
	Hyperplane* plane = observer.getHyperplane();
	float t = worldLine->intersectHyperplane(*plane);
	currentProperTime = worldLine->getProperTimeAtAbsoluteTime(t);
	delete plane;
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
		worldLine->genSaveString() + "\n"
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
			retVal->worldLineID = _worldLine;
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

void Observer::hover()
{
}
