#include "Observer.h"
#include "Geometry.h"
#include "Assets.h"
#include "StringOperations.h"


vec4 Observer::getLocation(const Settings& settings)
{
	return interpolate<bool, vec4>(settings.doLorentz, true, false,
		worldLine->getLocationAtProperTime(currentProperTime),
		worldLine->getLocationAtAbsoluteTime(currentProperTime)
		);
}

vec4 Observer::getVelocity(const Settings& settings)
{
	return interpolate<bool, vec4>(settings.doLorentz, true, false,
		worldLine->getVelocityAtProperTime(currentProperTime),
		worldLine->getVelocityAtAbsoluteTime(currentProperTime)
		);
}

vec4 Observer::getLocationAtZero(const Settings& settings)
{
	vec4 location = getLocation(settings);
	vec4 velocity = getVelocity(settings);
	return location - velocity / velocity.w * location.w;
}

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

	vec4 tangent = normalize(worldLine->getVelocityAtProperTime(currentProperTime));
	vec4 lorentzNormal = vec4(-tangent.x, -tangent.y, -tangent.z, tangent.w);	// Simultaneity is relative.
	vec4 galileanNormal = vec4(0, 0, 0, 1);										// Simultaneity is absolute.
	vec4 normal = interpolate<bool, vec4>(settings.doLorentz, true, false, lorentzNormal, galileanNormal);
	return new Hyperplane(location, normal);
}


/*
* Returns interpolated light cone in absolute frame.
*/
LightCone* Observer::getLightCone(const Settings& settings)
{
	return new LightCone(getLocation(settings));
}

vec3 Observer::transformPosToDiagramSpace(vec4 absoluteFramePos4, const Settings& settings) {
	vec3 absoluteFramePos = vec3(absoluteFramePos4[settings.diagramX], absoluteFramePos4[settings.diagramY], absoluteFramePos4[settings.diagramZ]);
	vec4 properFramePos4 = vec4(0, 0, 0, currentProperTime);
	vec3 properFramePos = vec3(properFramePos4[settings.diagramX], properFramePos4[settings.diagramY], properFramePos4[settings.diagramZ]);
	return interpolate<bool, vec3>(settings.transformToProperFrame, true, false, properFramePos, absoluteFramePos);
}

inline vec3 Observer::transformNormalToDiagramSpace(vec4 absoluteFrameNormal4, const Settings& settings) {
	vec3 absoluteFrameNormal = normalize(vec3(absoluteFrameNormal4[settings.diagramX], absoluteFrameNormal4[settings.diagramY], absoluteFrameNormal4[settings.diagramZ]));
	vec4 properFrameNormal4 = vec4(0, 0, 0, 1);
	vec3 properFrameNormal = vec3(properFrameNormal4[settings.diagramX], properFrameNormal4[settings.diagramY], properFrameNormal4[settings.diagramZ]);
	return interpolate<bool, vec3>(settings.transformToProperFrame, true, false, properFrameNormal, absoluteFrameNormal);
}

vec3 Observer::transformConeAxisToDiagramSpace(const Settings& settings)
{
	vec4 absoluteFrameAxis4 = vec4(0, 0, 0, 1);
	vec3 absoluteFrameAxis = vec3(absoluteFrameAxis4[settings.diagramX], absoluteFrameAxis4[settings.diagramY], absoluteFrameAxis4[settings.diagramZ]);
	vec3 properFrameAxisLorentz = absoluteFrameAxis;
	vec3 properFrameAxisGalilean = normalize(RelPhysics::galileanTransformationOfVelocity(absoluteFrameAxis, RelPhysics::To3DVelocity(getVelocity(settings))));
	vec3 properFrameAxis = interpolate<bool, vec3>(settings.doLorentz, true, false, properFrameAxisLorentz, properFrameAxisGalilean);
	vec3 axis = interpolate<bool, vec3>(settings.transformToProperFrame, true, false, properFrameAxis, absoluteFrameAxis);
	return axis;
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
	Hyperplane* plane = getHyperplane(settings);

	vec3 pos = transformPosToDiagramSpace(plane->getLocation(), settings);
	vec3 normal = transformNormalToDiagramSpace(plane->getNormal(), settings);
	
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(pos) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(pos), "M");
	gpuProgram.setUniform(TranslateMatrix(-pos), "invM");
	gpuProgram.setUniform(false, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "textMode");
	gpuProgram.setUniform(true, "directRenderMode");		// !

	PlaneSurface* geometry = new PlaneSurface(normal, 100, 100);
	geometry->GenSurface(20, 20);
	glDisable(GL_CULL_FACE);
	geometry->Draw();
	glEnable(GL_CULL_FACE);
	delete geometry;
	delete plane;
}

void Observer::DrawLightCone(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
	Assets::getObserverMaterial()->loadOnGPU(gpuProgram);

	LightCone* cone = getLightCone(settings);
	vec3 pos = transformPosToDiagramSpace(cone->getLocation(), settings);
	vec3 axis = transformConeAxisToDiagramSpace(settings);
	
	LightConeSurface* geometry = new LightConeSurface(axis);
	geometry->GenSurface(100, 100);

	gpuProgram.setUniform(TranslateMatrix(pos) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(pos), "M");
	gpuProgram.setUniform(TranslateMatrix(-pos), "invM");
	gpuProgram.setUniform(false, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(false, "textMode");
	gpuProgram.setUniform(true, "directRenderMode");
	glDisable(GL_CULL_FACE);
	geometry->Draw();
	glEnable(GL_CULL_FACE);
	delete geometry;
	delete cone;
}

void Observer::DrawNode(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
	vec3 pos = transformPosToDiagramSpace(getLocation(settings), settings);
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
	(*diagramCaption)->setPos(interpolate<bool, vec3>(settings.transformToProperFrame,
		true,
		false,
		vec3(0 , 0, 0),
		vec3(observerProperties.locationAtZero[settings.diagramX],
			observerProperties.locationAtZero[settings.diagramY],
			observerProperties.locationAtZero[settings.diagramZ])
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

float Observer::changeTimeByDelta(float deltaTau, const Settings& settings)
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

void Observer::draggedTo(vec4 location)
{
	worldLine->draggedTo(location);
}

vec4 Observer::getReferenceLocation()
{
	return worldLine->getReferenceLocation();
}
