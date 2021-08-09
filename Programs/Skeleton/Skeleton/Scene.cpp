#include "Scene.h"
#include "Geometry.h"
#include "Material.h"

void Scene::Create()
{
	camera = new Camera();
	camera->initBasic(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowWidth / (float)windowHeight, 0.02f, 2.0f);

	WorldLine* wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.1f, 0.0f, 0.0f), "Obs1's world line");
	Observer* observer = new Observer(wrdln, "Obs1", "An observer");
	observers.push_back(observer);

	vec3 color = vec3(0.1, 0.2, 0.9);
	ParamSurface* pSurface = new SphereSurface(0.5f);
	pSurface->GenSurface(100, 100);
	wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f), "Obj1's world line");
	Object* obj = new Object(
		vec3(1.0f, 1.0f, 1.0f),
		0.0f,
		vec3(2.5f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		wrdln,
		pSurface,
		new Material(color, color, color, 50),
		"Obj1",
		"Test object"
	);
	objects.push_back(obj);

	
	toggleCurrentObserver();
}
