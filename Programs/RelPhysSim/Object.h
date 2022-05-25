#pragma once

#include <map>
#include <memory>

#include "Entity.h"
#include "WorldLine.h"
#include "WorldLineView.h"
#include "Material.h"
#include "Geometry.h"
#include "Mesh.h"
#include "Camera.h"
#include "AdvancedTexture.h"
#include "EnumTypes.h"
#include "Observer.h"
#include "RelPhysics.h"
#include "Caption.h"

class Object : public Entity
{

public:
	static const char* typeNames[5];

	Object(
		vec3 _scale,
		float _rotaionAngle,
		float _rotationSpeed,
		vec3 _translation,
		vec3 _rotationAxis,
		WorldLine* _worldLine,
		Mesh* _geometry,
		Material* _material,
		Material* _diagramMaterial,
		AdvancedTexture* _texture,
		std::string _name = "",
		std::string _desc = ""
	)
		: Entity(_name, _desc),
		scale(_scale),
		rotationAngle(_rotaionAngle),
		rotationSpeed(_rotationSpeed),
		rotationAxis(_rotationAxis),
		translation(_translation),
		worldLine(_worldLine),
		geometry(_geometry),
		material(_material),
		diagramMaterial(_diagramMaterial),
		texture(_texture)
	{
		if (worldLine != nullptr) {
			worldLineView = (WorldLineView*)worldLine->createView();
		}
		diagramCaption = Caption::createNormalCaption(vec3(0, 0, 0), name.c_str());
	}

	~Object() {
		delete worldLine;
		delete worldLineView;
		(*diagramCaption)->erease();
	}

	void setType(RelTypes::ObjectType _type) {
		type = _type;
	}

	/*
	* Factory for Earth-like object.
	*/
	static Object* createEarth(WorldLine* wrdln);

	/*
	* Factory
	*/
	static Object* createDice(WorldLine* wrdln);

	/*
	* Factory
	*/
	static Object* createSpaceship(WorldLine* wrdln);

	/*
	* Factory
	*/
	static Object* createSpike(WorldLine* wrdln);

	mat4 getModellMatrix();

	mat4 getInverseModellMatrix();

	void Control(float dt, float absoluteTimeSpent) {
		//Todo
	}

	/*
	* Return vec3 diagram intersection position.
	*/
	vec3 getDiagramPos(
		const LightCone& lightCone,
		const Hyperplane& hyperplane,
		const RelTypes::Settings& settings,
		const RelTypes::ObserverProperties& observerProperties);

	void animate(float dt);

	void draw(GPUProgram& gpuProgram, 
		const Camera& camera,
		const LightCone& lightCone, 
		const Hyperplane& hyperplane, 
		const RelTypes::ObserverProperties& observerProperties,
		const RelTypes::Settings& settings);

	void drawDiagram(GPUProgram& gpuProgram, 
		const Camera& camera, 
		const LightCone& cone,
		const Hyperplane& plane, 
		const RelTypes::ObserverProperties& observerProperties, 
		const RelTypes::Settings& settings);

	std::string genSaveString();

	/*
	* Returns the loaded object.
	*/
	static Object* loadFromFile(std::ifstream& file);

	void setWorldLineID(const int id) {
		worldLineID = id;
	}

	int getWorldLineID() {
		return worldLineID;
	}

	void setWorldLine(std::map<int, WorldLine*>& worldLines);

	/*
	* Returns shortest distance between given ray and the object's world line's diagram representation.
	*/
	float rayDistanceToDiagram(const Ray& ray,
		RelTypes::ObserverProperties observerProperties,
		const RelTypes::Settings& settings);

	/*
	* Returns shortest distance between center of the object in current world space with current relativistic transformations and the given ray.
	*/
	float rayDistanceToObject(const Ray& ray,
		const LightCone& lightCone, 
		const Hyperplane& hyperplane,
		const RelTypes::Settings& settings,
		const RelTypes::ObserverProperties& observerProperties);

	/*
	* Returns location perceived by the observer expressed in observer's proper frame.
	* Interpolates between intersection modes and transformation modes.
	*/
	vec4 intersect(const LightCone& lightCone, 
		const Hyperplane& hyperplane, 
		const RelTypes::Settings& settings,
		const RelTypes::ObserverProperties& observerProperties);

	/*
	* Sphere model for collision detection.
	*/
	float getOverallRadius() {
		return geometry->getOverallRadius();
	}

	/*
	* Tell object, that mouse cursor hovered over image of object.
	*/
	void hover();

	// Inherited via Entity
	virtual void draggedTo(vec4 location) override;
	vec4 getClosestLocation(const Ray& ray,
		const RelTypes::ObserverProperties& observerProperties,
		const RelTypes::Settings& settings) override;

	void addPointWordlLine(const vec3 point, const RelTypes::Settings& settings, const vec3 color = vec3(0.5f, 0.5f, 0.5f)) {
		AdditionalWorldLineData* data = new AdditionalWorldLineData();
		data->point = point;
		data->line = WorldLine::createWorldLineOfObjectPoint(point, *worldLine, settings);
		data->view = (WorldLineView*)data->line->createView();
		data->view->setColor(color);
		additionalWorldLines.push_back(data);
	}

	void recreatePointWorldLines(const RelTypes::Settings& settings) {
		std::vector<AdditionalWorldLineData*> oldData = additionalWorldLines;
		additionalWorldLines.clear();
		for (auto data : oldData) {
			addPointWordlLine(data->point, settings, data->view->getColor());
			delete data;
		}
	}

	private:
		float rotationAngle, rotationSpeed;
		vec3 scale, rotationAxis, translation;
		WorldLine* worldLine = NULL;
		WorldLineView* worldLineView = NULL;
		Mesh* geometry;
		Material* material = NULL;
		Material* diagramMaterial = NULL;
		AdvancedTexture* texture = NULL;
		vec4 locationFV, velocityFV = vec4(0, 0, 0, 1);
		RelTypes::ObjectType type = RelTypes::ObjectType::none;
		int worldLineID = 0;
		std::shared_ptr<Caption*> diagramCaption;
		struct AdditionalWorldLineData {
			vec3 point;
			WorldLine* line;
			WorldLineView* view;
			~AdditionalWorldLineData() {
				if (nullptr != line) {
					delete line;
				}
				if (nullptr != view) {
					delete view;
				}
			}
		};
		std::vector<AdditionalWorldLineData*> additionalWorldLines;

};

