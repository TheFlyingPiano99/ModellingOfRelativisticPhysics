#include "Assets.h"

SphereSurface* Assets::observerNodeGeometry = NULL;
ConeSurface* Assets::lightConeGeometry = NULL;
OBJGeometry* Assets::cubeGeometry = NULL;
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
