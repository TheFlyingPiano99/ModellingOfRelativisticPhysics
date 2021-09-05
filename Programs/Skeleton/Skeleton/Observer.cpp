#include "Observer.h"
#include "Geometry.h"
#include "Assets.h"
#include "StringOperations.h"


vec4 Observer::getLocation(const Settings& settings)
{
	vec4 lorentz = worldLine->getLocationAtProperTime(currentProperTime);
	vec4 galilean = worldLine->getLocationAtAbsoluteTime(currentProperTime);
	vec4 retVal;
	if (settings.doLorentz.interpolating()) {
		if (settings.doLorentz.get()) {
			retVal = lerp<vec4>(galilean, lorentz, settings.doLorentz.getFraction());
		}
		else {
			retVal = lerp<vec4>(lorentz, galilean, settings.doLorentz.getFraction());
		}
	}
	else {
		if (settings.doLorentz.get()) {
			retVal = lorentz;
		}
		else {
			retVal = galilean;
		}
	}
	return retVal;
}

vec4 Observer::getVelocity(const Settings& settings)
{
	vec4 lorentz = worldLine->getVelocityAtProperTime(currentProperTime);
	vec4 galilean = worldLine->getVelocityAtAbsoluteTime(currentProperTime);
	vec4 retVal;
	if (settings.doLorentz.interpolating()) {
		if (settings.doLorentz.get()) {
			retVal = lerp<vec4>(galilean, lorentz, settings.doLorentz.getFraction());
		}
		else {
			retVal = lerp<vec4>(lorentz, galilean, settings.doLorentz.getFraction());
		}
	}
	else {
		if (settings.doLorentz.get()) {
			retVal = lorentz;
		}
		else {
			retVal = galilean;
		}
	}
	return retVal;
}

vec4 Observer::getLocationAtZero(const Settings& settings)	// Must be fixed!
{
	vec4 location = getLocation(settings);
	vec4 velocity = getVelocity(settings);
	return location - velocity / velocity.w * location.w;
}

/*
* Return all necessary properties.
*/

ObserverProperties Observer::getProperties(const Settings& settings) {
	ObserverProperties properties;
	properties.velocity = getVelocity(settings);
	properties.location = getLocation(settings);
	properties.locationAtZero = getLocationAtZero(settings);
	return properties;
}

Hyperplane* Observer::getHyperplane(const Settings& settings)
{
	vec4 location = getLocation(settings);
	vec4 tangent = worldLine->getVelocityAtProperTime(currentProperTime);
	vec4 lorentzNormal = vec4(-tangent.x, -tangent.y, -tangent.z, tangent.w);
	vec4 galileanNormal = vec4(0,0,0,1);

	vec4 normal;
	if (settings.doLorentz.interpolating()) {
		if (settings.doLorentz.get()) {
			normal = lerp<vec4>(galileanNormal, lorentzNormal, settings.doLorentz.getFraction());
		}
		else {
			normal = lerp<vec4>(lorentzNormal, galileanNormal, settings.doLorentz.getFraction());
		}
	}
	else {
		if (settings.doLorentz.get()) {
			normal = lorentzNormal;
		}
		else {
			normal = galileanNormal;
		}
	}
	return new Hyperplane(location, normal);
}



LightCone* Observer::getLightCone(const Settings& settings)
{
	return new LightCone(getLocation(settings));
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
	gpuProgram.setUniform(false, "directRenderMode");

	(*diagramCaption)->setVisible(false);
	(*timerCaption)->setVisible(false);

	worldLine->DrawDiagram();
}

void Observer::DrawHyperplane(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
	vec3 pos;
	if (settings.transformToProperFrame.get()) {
		pos = vec3(0, 0, currentProperTime);
	}
	else {
		vec4 pos4 = this->getLocation(settings);
		pos = vec3(pos4[settings.diagramX], pos4[settings.diagramY], pos4[settings.diagramZ]);
	}
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(pos) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(pos), "M");
	gpuProgram.setUniform(TranslateMatrix(-pos), "invM");
	gpuProgram.setUniform(false, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "textMode");
	gpuProgram.setUniform(true, "directRenderMode");
	
	vec4 v = getVelocity(settings);
	vec3 planeNormal = (settings.transformToProperFrame.get() || !settings.doLorentz.get()) ?
		vec3(0, 0, 1)
		: normalize(vec3(-v.x, -v.y, v.w));

	PlaneSurface* plane = new PlaneSurface(planeNormal,	100, 100);
	plane->GenSurface(20, 20);
	glDisable(GL_CULL_FACE);
	plane->Draw();
	glEnable(GL_CULL_FACE);
}

void Observer::DrawLightCone(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	vec3 pos;
	if (settings.transformToProperFrame.get()) {
		pos = vec3(0, 0, currentProperTime);
	}
	else {
		vec4 pos4 = this->getLocation(settings);
		pos = vec3(pos4[settings.diagramX], pos4[settings.diagramY], pos4[settings.diagramZ]);
	}
	gpuProgram.setUniform(TranslateMatrix(pos) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(pos), "M");
	gpuProgram.setUniform(TranslateMatrix(-pos), "invM");
	gpuProgram.setUniform(false, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "textMode");
	gpuProgram.setUniform(true, "directRenderMode");
	glDisable(GL_CULL_FACE);
	Assets::getLightConeGeomtry()->Draw();
	glEnable(GL_CULL_FACE);
}

void Observer::DrawNode(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
	vec3 pos;
	if (settings.transformToProperFrame.get()) {
		pos = vec3(0, 0, currentProperTime);
	}
	else {
		vec4 pos4 = this->getLocation(settings);
		pos = vec3(pos4[settings.diagramX], pos4[settings.diagramY], pos4[settings.diagramZ]);
	}
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(pos) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(pos), "M");
	gpuProgram.setUniform(TranslateMatrix(-pos), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(true, "directRenderMode");
	Assets::getObserverNodeGeometry()->Draw();
}

void Observer::DrawExtras(GPUProgram& gpuProgram, Camera& camera, const ObserverProperties& observerProperties, const Settings& settings)
{
	if (settings.intersectionMode.get() == IntersectionMode::lightCone) {
		DrawLightCone(gpuProgram, camera, settings);
	}
	else if (settings.intersectionMode.get() == IntersectionMode::hyperplane) {
		DrawHyperplane(gpuProgram, camera, settings);
	}
	DrawNode(gpuProgram, camera, settings);
	static int updateCounter = 0;
	static float prevTau = 0.0f;
	if (updateCounter++ > 30) {		// Reduced refresh rate.
		prevTau = currentProperTime;
		updateCounter = 0;
		(*timerCaption)->changeText(std::string("tau = ").append(std::to_string(prevTau)).append(" m\n")
			.append("t = ").append(std::to_string((settings.doLorentz.get()) ? worldLine->getAbsoluteTimeAtProperTime(currentProperTime) : currentProperTime)).append(" m").c_str());
	}
	vec3 pos;
	if (settings.transformToProperFrame.get()) {
		pos = vec3(0, 0, currentProperTime);
	}
	else {
		vec4 pos4 = this->getLocation(settings);
		pos = vec3(pos4[settings.diagramX], pos4[settings.diagramY], pos4[settings.diagramZ]);
	}
	(*timerCaption)->setPos(pos + camera.getRight() * 11 + camera.getUp() * 1);
	(*timerCaption)->setVisible(selected);
	(*diagramCaption)->setPos((settings.transformToProperFrame.get())? vec3(0 , 0, 0)
		: vec3(observerProperties.locationAtZero[settings.diagramX],
			observerProperties.locationAtZero[settings.diagramY],
			observerProperties.locationAtZero[settings.diagramZ]
			));
	(*diagramCaption)->setVisible(true);
	(*timerCaption)->setVisible(true);
}

void Observer::setCurrentTimeAtAbsoluteTime(float t, const Settings& settings)
{
	if (settings.doLorentz.get()) {
		currentProperTime = worldLine->getProperTimeAtAbsoluteTime(t);
	}
	else {
		currentProperTime = t;
	}
}

float Observer::getAbsoluteTimeAtCurrentTime(const Settings& settings)
{
	if (settings.doLorentz.get()) {
		return worldLine->getAbsoluteTimeAtProperTime(currentProperTime);
	}
	else {
		return currentProperTime;
	}
}

float Observer::increaseTimeByDelta(float deltaTau, const Settings& settings)
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

void Observer::syncCamera(Camera* camera, const Settings& settings)
{
	camera->syncToObserver(getProperties(settings));
}

void Observer::syncTimeToObserversSimultaneity(Observer& observer, const Settings& settings)
{
	Hyperplane* plane = observer.getHyperplane(settings);
	float t = worldLine->intersectHyperplane(*plane);
	if (settings.doLorentz.get()) {
		currentProperTime = worldLine->getProperTimeAtAbsoluteTime(t);
	}
	else {
		currentProperTime = t;
	}
	delete plane;
}

void Observer::loadOnGPU(GPUProgram& gpuProgram, const Settings& settings)
{
	gpuProgram.setUniform(getVelocity(settings), "observersVelocity");
	gpuProgram.setUniform(getLocation(settings), "observersLocation");
	gpuProgram.setUniform(getLocationAtZero(settings), "observersStartPos");
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
