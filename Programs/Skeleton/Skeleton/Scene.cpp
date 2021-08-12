#include "Scene.h"
#include "Geometry.h"
#include "Material.h"

void Scene::Create()
{
	//Camera:
	camera = new Camera();
	camera->initBasic(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowHeight / (float)windowWidth, 0.02f, 3.0f);

	//LightSources:----------------------------------------------
	lights.push_back(new LightSource(vec3(0, 0, 0), vec3(1000, 1000, 1000), 0));
	lights.push_back(new LightSource(vec3(0, 50, 50), vec3(1000, 1000, 1000), 0));

	//Observers:-------------------------------------------------
	//1.:
	WorldLine* wrdln = new GeodeticLine(vec3(0.0f, -6.0f, 0.0f), vec3(0.0f, 0.93f, 0.0f), "Obs1's world line");
	Observer* observer = new Observer(wrdln, "Obs1", "An observer");
	observers.push_back(observer);

	//2.:
	wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "Obs1's world line");
	observer = new Observer(wrdln, "Obs2", "An observer");
	observers.push_back(observer);

	//3.:
	wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f), "Obs1's world line");
	observer = new Observer(wrdln, "Obs3", "An observer");
	observers.push_back(observer);

	//Objects:----------------------------------------------------
	wrdln = new GeodeticLine(vec3(3.0f, -6.0f, 0.0f), vec3(0.0f, 0.93f, 0.0f), "Obj1's world line");
	objects.push_back(Object::createEarth(wrdln));

	//Background:------------------------------------------------------
	background = new Background();
	
	//Other:
	toggleCurrentObserver();

}
