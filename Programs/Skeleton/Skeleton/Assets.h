#pragma once
#include "Geometry.h"
#include "Material.h"
#include <string>
#include <filesystem>
#include "Font.h"


/*
* Static getters for frequently used geomtries etc.
*/
class Assets {
	static SphereSurface* observerNodeGeometry;
	static ConeSurface* lightConeGeometry;
	static Material* observerMaterial;
	static Material* selectedObjectMaterial;
	static Material* selectedWorldLineMaterial;
	static Material* hoveredObjectMaterial;
	static Material* hoveredWorldLineMaterial;
	static OBJGeometry* cubeGeometry;
	static std::string* geometryPath;
	static std::string* texturePath;
	static std::string* savesPath;
	
	static Texture* fontTexture;
	static Font* defaultFont;

public:

	static Geometry* getObserverNodeGeometry() {
		if (observerNodeGeometry == nullptr) {
			observerNodeGeometry = new SphereSurface(0.5f);
			observerNodeGeometry->GenSurface(10, 10);
		}
		return observerNodeGeometry;
	}

	static Geometry* getLightConeGeomtry() {
		if (lightConeGeometry == nullptr) {
			lightConeGeometry = new ConeSurface();
			lightConeGeometry->GenSurface(50, 50);
		}
		return lightConeGeometry;
	}

	static Geometry* getCubeGeometry() {
		if (cubeGeometry == nullptr) {
			cubeGeometry = new OBJGeometry();
			cubeGeometry->load(getGeometryPath().append("cube.obj").c_str());

		}
		return cubeGeometry;
	}

	static Material* getObserverMaterial() {
		if (observerMaterial == nullptr) {
			observerMaterial = new Material(vec3(3, 1.5, 1), vec3(1, 0, 0.5), vec3(0.2, 0.2, 0.2), 3, 0.4f);
		}
		return observerMaterial;
	}

	static Material* getSelectedObjectMaterial() {
		if (selectedObjectMaterial == nullptr) {
			selectedObjectMaterial = new Material(vec3(3, 3, 3), vec3(8, 14, 8), vec3(2, 2, 2), 3, 1.0f);
		}
		return selectedObjectMaterial;
	}

	static Material* getSelectedWorldLineMaterial() {
		if (selectedWorldLineMaterial == nullptr) {
			selectedWorldLineMaterial = new Material(vec3(3, 1.5, 1), vec3(1, 1, 1), vec3(0.2, 0.2, 0.2), 3, 1.0f);
			selectedWorldLineMaterial->setGlow(true);
		}
		return selectedWorldLineMaterial;
	}

	static Material* getHoveredObjectMaterial() {
		if (hoveredObjectMaterial == nullptr) {
			hoveredObjectMaterial = new Material(vec3(3, 3, 3), vec3(7, 8, 7), vec3(2, 2, 2), 3, 1.0f);
		}
		return hoveredObjectMaterial;
	}

	static Material* getHoveredWorldLineMaterial() {
		if (hoveredWorldLineMaterial == nullptr) {
			hoveredWorldLineMaterial = new Material(vec3(3, 1.5, 1), vec3(0.7f, 0.7f, 0.7f), vec3(0.2, 0.2, 0.2), 3, 1.0f);
			hoveredWorldLineMaterial->setGlow(true);
		}
		return hoveredWorldLineMaterial;
	}

	static std::string getGeometryPath() {
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

	static std::string getTexturePath() {
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

	static std::string getSavesPath() {
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

	static Texture* getFontTexture() {
		if (fontTexture == nullptr) {
			fontTexture = new Texture(getTexturePath().append("font.bmp"));
		}
		return fontTexture;
	}

	static Font* getDefaultFont() {
		if (defaultFont == nullptr) {
			defaultFont = new Font(getFontTexture());
		}
		return defaultFont;
	}



	/*
	* Must be called befor the application is closed!
	*/
	static void cleanUp() {
		delete observerNodeGeometry;
		delete lightConeGeometry;
		delete observerMaterial;
		delete selectedObjectMaterial;
		delete selectedWorldLineMaterial;
		delete cubeGeometry;
		delete geometryPath;
		delete texturePath;
		delete savesPath;
		delete fontTexture;
		delete defaultFont;
		delete hoveredObjectMaterial;
		delete hoveredWorldLineMaterial;
	}
};

