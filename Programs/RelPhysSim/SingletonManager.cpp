#include "SingletonManager.h"

#include "ControlEventManager.h"
#include "Scene.h"

/*
* Needs to be called before closing program!
*/
void Singleton::destroyAllInstances() {
	ControlEventManager::destroyInstance();
	Scene::destroyInstance();
	Assets::cleanUp();
}
