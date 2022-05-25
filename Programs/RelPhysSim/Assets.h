#pragma once

#include <string>
#include <filesystem>

#include "Geometry.h"
#include "Material.h"
#include "Font.h"
#include "Mesh.h"
#include "TextureCube.h"


/*
* Static getters for frequently used geomtries etc.
*/
class Assets {
	static SphereSurface* observerNodeGeometry;
	static LightConeSurface* lightConeGeometry;
	static Material* observerMaterial;
	static Material* selectedObjectMaterial;
	static Material* selectedWorldLineMaterial;
	static Material* hoveredObjectMaterial;
	static Material* hoveredWorldLineMaterial;
	static OBJGeometry* cubeGeometry;
	static OBJGeometry* spaceshipGeometry;
	static OBJGeometry* spikeGeometry;
	static std::string* geometryPath;
	static std::string* texturePath;
	static std::string* savesPath;
	
	static Texture* fontTexture;
	static TextureCube* seaBackgroundTexture;
	static Font* defaultFont;

public:

	static Mesh* getObserverNodeGeometry();

	static Mesh* getLightConeGeomtry();

	static Mesh* getCubeGeometry();

	static Mesh* getSpaceshipGeometry();

	static Mesh* getSpikeGeometry();

	static Material* getObserverMaterial();

	static Material* getSelectedObjectMaterial();

	static Material* getSelectedWorldLineMaterial();

	static Material* getHoveredObjectMaterial();

	static Material* getHoveredWorldLineMaterial();

	static std::string getGeometryPath();

	static std::string getTexturePath();

	static std::string getSavesPath();

	static Texture* getFontTexture();

	static TextureCube* getSeaBackgroundTexture();

	static Font* getDefaultFont();

	/*
	* Must be called befor the application is closed!
	*/
	static void cleanUp();
};

