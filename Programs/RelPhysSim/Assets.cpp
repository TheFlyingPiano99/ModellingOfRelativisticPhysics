#include "Assets.h"

SphereSurface* Assets::observerNodeGeometry = NULL;
LightConeSurface* Assets::lightConeGeometry = NULL;
OBJGeometry* Assets::cubeGeometry = NULL;
OBJGeometry* Assets::spaceshipGeometry = NULL;
Material* Assets::observerMaterial = NULL;
Material* Assets::selectedObjectMaterial = NULL;
Material* Assets::selectedWorldLineMaterial = NULL;
std::string* Assets::geometryPath = NULL;
std::string* Assets::texturePath = NULL;
std::string* Assets::savesPath = NULL;
Texture* Assets::fontTexture = NULL;
Font* Assets::defaultFont = NULL;
Material* Assets::hoveredObjectMaterial = NULL;
Material* Assets::hoveredWorldLineMaterial = NULL;

Mesh* Assets::getObserverNodeGeometry() {
	if (observerNodeGeometry == nullptr) {
		observerNodeGeometry = new SphereSurface(0.5f);
		observerNodeGeometry->GenSurface(10, 10);
	}
	return observerNodeGeometry;
}

Mesh* Assets::getLightConeGeomtry() {
	if (lightConeGeometry == nullptr) {
		lightConeGeometry = new LightConeSurface(vec3(0, 0, 1));
		lightConeGeometry->GenSurface(50, 50);
	}
	return lightConeGeometry;
}

Mesh* Assets::getCubeGeometry() {
	if (cubeGeometry == nullptr) {
		cubeGeometry = new OBJGeometry();
		cubeGeometry->load(getGeometryPath().append("cube.obj").c_str());

	}
	return cubeGeometry;
}

Mesh* Assets::getSpaceshipGeometry() {
	if (spaceshipGeometry == nullptr) {
		spaceshipGeometry = new OBJGeometry();
		spaceshipGeometry->load(getGeometryPath().append("spaceShip.obj").c_str());

	}
	return spaceshipGeometry;
}


Material* Assets::getObserverMaterial() {
	if (observerMaterial == nullptr) {
		observerMaterial = new Material(vec3(3, 1.5, 1), vec3(1, 0, 0.5), vec3(0.2, 0.2, 0.2), 3, 0.6f);
	}
	return observerMaterial;
}


Material* Assets::getSelectedObjectMaterial() {
	if (selectedObjectMaterial == nullptr) {
		selectedObjectMaterial = new Material(vec3(3.0f, 3.0f, 3.0f), vec3(8.0f, 14.0f, 8.0f), vec3(2.0f, 2.0f, 2.0f), 3.0f, 1.0f);
	}
	return selectedObjectMaterial;
}


Material* Assets::getSelectedWorldLineMaterial() {
	if (selectedWorldLineMaterial == nullptr) {
		selectedWorldLineMaterial = new Material(vec3(3.0f, 1.5f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(0.2f, 0.2f, 0.2f), 3, 1.0f);
		selectedWorldLineMaterial->setGlow(true);
	}
	return selectedWorldLineMaterial;
}


Material* Assets::getHoveredObjectMaterial() {
	if (hoveredObjectMaterial == nullptr) {
		hoveredObjectMaterial = new Material(vec3(3, 3, 3), vec3(7, 8, 7), vec3(2, 2, 2), 3, 1.0f);
	}
	return hoveredObjectMaterial;
}

Material* Assets::getHoveredWorldLineMaterial() {
	if (hoveredWorldLineMaterial == nullptr) {
		hoveredWorldLineMaterial = new Material(vec3(3, 1.5, 1), vec3(0.7f, 0.7f, 0.7f), vec3(0.2, 0.2, 0.2), 3, 1.0f);
		hoveredWorldLineMaterial->setGlow(true);
	}
	return hoveredWorldLineMaterial;
}

std::string& Assets::getGeometryPath() {
	if (geometryPath == nullptr) {
		int n = 0;
		while (n < 5) {

			geometryPath = new std::string("");
			for (int i = 0; i < n; i++) {
				geometryPath->append("../");
			}
			geometryPath->append("Resources/Geometry/");
			std::filesystem::path p = std::filesystem::path(geometryPath->c_str());
			if (std::filesystem::is_directory(p)) {
				break;
			}
			n++;
			delete geometryPath;
		}
		if (geometryPath == nullptr) {
			throw std::exception("Resources/Geometry folder not found!");
		}
	}

	return *geometryPath;
}

std::string Assets::getTexturePath() {
	if (texturePath == nullptr) {
		int n = 0;
		while (n < 5) {

			texturePath = new std::string("");
			for (int i = 0; i < n; i++) {
				texturePath->append("../");
			}
			texturePath->append("Resources/Texture/");
			std::filesystem::path p = std::filesystem::path(texturePath->c_str());
			if (std::filesystem::is_directory(p)) {
				break;
			}
			n++;
			delete texturePath;
		}
		if (texturePath == nullptr) {
			throw std::exception("Resources/Texture folder not found!");
		}
	}
	return *texturePath;
}

std::string Assets::getSavesPath() {
	if (savesPath == nullptr) {
		int n = 0;
		while (n < 5) {

			savesPath = new std::string("");
			for (int i = 0; i < n; i++) {
				savesPath->append("../");
			}
			savesPath->append("Saves/");
			std::filesystem::path p = std::filesystem::path(savesPath->c_str());
			if (std::filesystem::is_directory(p)) {
				break;
			}
			n++;
			delete savesPath;
		}
		if (savesPath == nullptr) {
			throw std::exception("Saves folder not found!");
		}
	}
	return *savesPath;
}

Texture* Assets::getFontTexture() {
	if (fontTexture == nullptr) {
		fontTexture = new Texture(getTexturePath().append("font.bmp"));
	}
	return fontTexture;
}

Font* Assets::getDefaultFont() {
	if (defaultFont == nullptr) {
		defaultFont = new Font(getFontTexture());
	}
	return defaultFont;
}

/*
* Must be called befor the application is closed!
*/
void Assets::cleanUp() {
	delete observerNodeGeometry;
	delete lightConeGeometry;
	delete observerMaterial;
	delete selectedObjectMaterial;
	delete selectedWorldLineMaterial;
	delete cubeGeometry;
	delete spaceshipGeometry;
	delete geometryPath;
	delete texturePath;
	delete savesPath;
	delete fontTexture;
	delete defaultFont;
	delete hoveredObjectMaterial;
	delete hoveredWorldLineMaterial;
}
