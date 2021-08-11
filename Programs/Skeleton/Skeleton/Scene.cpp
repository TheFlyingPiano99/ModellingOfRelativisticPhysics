#include "Scene.h"
#include "Geometry.h"
#include "Material.h"

void Scene::Create()
{
	//Camera:
	camera = new Camera();
	camera->initBasic(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowHeight / (float)windowWidth, 0.02f, 2.0f);

	//Observers:-------------------------------------------------
	//1.:
	WorldLine* wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "Obs1's world line");
	Observer* observer = new Observer(wrdln, "Obs1", "An observer");
	observers.push_back(observer);

	//2.:
	wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f), "Obs1's world line");
	observer = new Observer(wrdln, "Obs2", "An observer");
	observers.push_back(observer);

	//3.:
	wrdln = new GeodeticLine(vec3(0.0f, -20.0f, 0.0f), vec3(0.5f, 0.4f, 0.0f), "Obs1's world line");
	observer = new Observer(wrdln, "Obs3", "An observer");
	observers.push_back(observer);

	//Objects:----------------------------------------------------
	AdvancedTexture* texture = new AdvancedTexture("../../../Resources/lowres/earth_daymap.bmp", "../../../Resources/lowres/earth_normal_map.bmp", "");
	if (texture->getTextureId() == 0) {
		texture = NULL;
	}
	vec3 color = vec3(1, 1, 1);
	ParamSurface* pSurface = new SphereSurface(0.5f);
	pSurface->GenSurface(25, 25);
	wrdln = new GeodeticLine(vec3(3.0f, -20.0f, 0.0f), vec3(0.5f, 0.4f, 0.0f), "Obj1's world line");
	Object* obj = new Object(
		vec3(1.0f, 1.0f, 1.0f),
		0.0f,
		0.01f,
		vec3(2.5f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		wrdln,
		pSurface,
		new Material(color, color, color, 50),
		texture,
		"Obj1",
		"Test object"
	);
	objects.push_back(obj);

	//Background:------------------------------------------------------
	background = new Background();
	
	//Other:
	toggleCurrentObserver();


	//Test:----------------------------------
	/*
	LightCone cone(vec4(0,0,1,3));
	GeodeticLine line(vec3(4.6,5,835), vec3(0,0,0), "Test line", "");
	float t = line.intersectLightCone(cone);
	vec4 intersection = line.getLocationAtAbsoluteTime(t);
	std::cout << "Test intersection = " << intersection.x << ", " << intersection.y << ", " << intersection.z << ", " << intersection.w << std::endl;
	*/
}
